/*
 * File:    mms_transaction.c
 * Author:  Li XianJing <lixianjing@broncho.cn>
 * Brief:   a simple mms client.
 *
 * Copyright (c) 2006  topwise, Inc.
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2006-12-13 Li XianJing <lixianjing@broncho.cn> created
 *
 */

#include "mms_util.h"
#include "mms_header_codec.h"
#include "mms_transaction.h"
#include "http_transaction.h"

struct _MmsTransaction
{
	MmsHeader*  resp_header;
	Mms*        recv_mms;

	HttpTransaction* http_trans;
};

MmsTransaction* mms_transaction_create(char* gateway, int gateway_port, char* mmsc, int mmsc_port, int timeout_usec)
{
	MmsTransaction* thiz = NULL;
	
	mms_return_val_if_fail(mmsc != NULL && mmsc_port > 0, NULL);
	mms_return_val_if_fail(gateway != NULL && gateway_port > 0, NULL);

	thiz = (MmsTransaction*)mms_calloc(1, sizeof(MmsTransaction));

	if(thiz != NULL)
	{
		thiz->recv_mms    = mms_create();
		thiz->resp_header = mms_header_create(mms_header_unpack_item, mms_header_pack_item);
    	mms_header_set_unpack_end_at_content_type(thiz->resp_header, 1);
		
		if ((thiz->http_trans  = http_transaction_create(gateway, gateway_port, timeout_usec)) != NULL)
		{
			http_transaction_set_header(thiz->http_trans, "User-Agent",      MMS_USER_AGENT);
			http_transaction_set_header(thiz->http_trans, "Accept",          "*/*");
			http_transaction_set_header(thiz->http_trans, "Accept-Language", "zh");
			http_transaction_set_header(thiz->http_trans, "Profile", "http://www.htcmms.com.tw/gen/wizard-1.0.xml");
			http_transaction_set_header(thiz->http_trans, "Content-Type",  "application/vnd.wap.mms-message");
			http_transaction_set_header(thiz->http_trans, "Accept-Charset", "ISO-8859-1");
			http_transaction_set_header(thiz->http_trans, "Progma", "no-cache");
		}
		else
		{
			mms_transaction_destroy (thiz);
			thiz = NULL;
		}
	}

	return thiz;
}

MmsResult mms_transaction_set_user_agent(MmsTransaction* thiz, char* user_agent)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	return http_transaction_set_user_agent(thiz->http_trans, user_agent);
}

MmsResult mms_transaction_set_header(MmsTransaction* thiz, char* name, char* value)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	return http_transaction_set_header(thiz->http_trans, name, value);
}

void            mms_transaction_destroy(MmsTransaction* thiz)
{
	mms_return_if_fail (thiz != NULL);
	
	if (thiz->http_trans != NULL)
	{
		http_transaction_destroy(thiz->http_trans);
		thiz->http_trans = NULL;
	}

	if (thiz->resp_header != NULL)
	{
		mms_header_destroy(thiz->resp_header);
		thiz->resp_header = NULL;
	}

	if (thiz->recv_mms != NULL)
	{
		mms_destroy(thiz->recv_mms);
		thiz->recv_mms = NULL;
	}
	mms_free(thiz);

	return;
}

static MmsResult mms_transaction_parse_send_conf(MmsTransaction* thiz, MmsHeader** resp_header)
{
	MmsHeader*      header = NULL;
	MmsInputBuffer* input = NULL;
	size_t length = http_transaction_get_resp_content_length(thiz->http_trans);
	char*  body   = http_transaction_get_resp_content(thiz->http_trans);
	char* content_type = http_transaction_get_resp_header(thiz->http_trans, "Content-Type");

	mms_return_val_if_fail(content_type != NULL && length > 0 && resp_header != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(strcmp(content_type, "application/vnd.wap.mms-message") == 0, MMS_RESULT_FAIL);

	input = mms_input_buffer_create(body, length);
	mms_header_unpack(thiz->resp_header, input);
	mms_input_buffer_destroy(input);
	
	*resp_header = thiz->resp_header;

	return MMS_RESULT_OK;
}

MmsResult mms_transaction_send_buffer(MmsTransaction* thiz, char* url, void* data, size_t length, MmsHeader** resp_header)
{
	MmsResult        ret         = MMS_RESULT_FAIL;
	char			 host[MMS_MAX_PATH + 1] = {0};
	
	mms_return_val_if_fail(thiz != NULL && url != NULL && data != NULL, MMS_RESULT_FAIL);

	mms_get_host_from_url (url, host);
	http_transaction_set_header(thiz->http_trans, "Host", host);
	ret = http_transaction_post_request(thiz->http_trans, url, data, length);

	mms_return_val_if_fail(ret == MMS_RESULT_OK, ret);

	return mms_transaction_parse_send_conf(thiz, resp_header);
}

MmsResult mms_transaction_send(MmsTransaction* thiz, char* url, Mms* mms, MmsHeader** resp_header)
{
	MmsResult        ret         = MMS_RESULT_FAIL;
	MmsOutputBuffer* output      = NULL;
	MmsHeader*       header      = NULL;
	char*            data	     = NULL;
	size_t           data_length = 0;
	char             host[MMS_MAX_PATH + 1] = {0};
	
	mms_return_val_if_fail(thiz != NULL && url != NULL && mms != NULL, MMS_RESULT_FAIL);

	mms_get_host_from_url (url, host);
	http_transaction_set_header(thiz->http_trans, "Host", host);
	mms_fix_nokia(mms);
	output = mms_output_buffer_create(1024);
	mms_pack(mms, output);
	data = mms_output_buffer_get_buffer(output);
	data_length = mms_output_buffer_get_buffer_length(output);
	ret = http_transaction_post_request(thiz->http_trans, url, data, data_length);
	mms_output_buffer_destroy(output);

	mms_return_val_if_fail(ret == MMS_RESULT_OK, ret);

	return mms_transaction_parse_send_conf(thiz, resp_header);
}


MmsResult mms_transaction_send_file(MmsTransaction* thiz, char* url, char* filename, MmsHeader** resp_header)
{
	MmsResult ret    = MMS_RESULT_FAIL;
	char*     buffer = NULL;
	size_t    length = 0;
	char      host[MMS_MAX_PATH + 1] = {0};
	
	mms_return_val_if_fail(thiz != NULL && url != NULL && filename != NULL, MMS_RESULT_FAIL);

	mms_get_host_from_url (url, host);
	http_transaction_set_header(thiz->http_trans, "Host", host);
	buffer = mms_read_binary_file(filename, &length);
	mms_return_val_if_fail(buffer != NULL, MMS_RESULT_FAIL);
	
	ret = http_transaction_post_request(thiz->http_trans, url, buffer, length);
	mms_free(buffer);

	mms_return_val_if_fail(ret == MMS_RESULT_OK, ret);

	return mms_transaction_parse_send_conf(thiz, resp_header);
}

static MmsResult mms_transaction_parse_retrieve_conf(MmsTransaction* thiz, Mms** mms)
{
	char* data = NULL;
	size_t content_length = http_transaction_get_resp_content_length(thiz->http_trans);
	char* content_type    = http_transaction_get_resp_header(thiz->http_trans, "Content-Type");
	char* content         = http_transaction_get_resp_content(thiz->http_trans);
	MmsInputBuffer* input = NULL;
	
	mms_return_val_if_fail(content_type != NULL && content_length > 0, MMS_RESULT_FAIL);
	mms_return_val_if_fail(strcmp(content_type, "application/vnd.wap.mms-message") == 0, MMS_RESULT_FAIL);

	input = mms_input_buffer_create(content, content_length);
	mms_unpack(thiz->recv_mms, input);
	mms_input_buffer_destroy(input);
	
	*mms = thiz->recv_mms;

	return MMS_RESULT_OK;
}

MmsResult mms_transaction_receive(MmsTransaction* thiz, char* url, Mms** mms)
{
	Mms* new_mms = NULL;
	MmsResult ret = MMS_RESULT_FAIL;
	char host[MMS_MAX_PATH + 1] = {0};
	
	mms_return_val_if_fail(thiz != NULL && url != NULL && mms != NULL, MMS_RESULT_FAIL);

	mms_get_host_from_url (url, host);
	http_transaction_set_header(thiz->http_trans, "Host", host);
	ret = http_transaction_get_request(thiz->http_trans, url);
	mms_return_val_if_fail(ret == MMS_RESULT_OK, ret);
	
	ret = mms_transaction_parse_retrieve_conf(thiz, mms);

	return ret;
}

int       mms_transaction_get_resp_code(MmsTransaction* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);

	return http_transaction_get_resp_code(thiz->http_trans);
}

#ifdef MMS_TRANSACTION_TEST
#include "mms_util.h"

static Mms* construct_fake_mms(void)
{
	size_t par_index = 0;
	Mms* mms = mms_create_send_req(0);
	//mms_set_to(mms, "13928462789/TYPE=PLMN", CHARSET_GBK);
	mms_set_to(mms, "13928462789/TYPE=PLMN", CHARSET_GBK);
	mms_set_subject(mms, "\xd6\xd0\xb9\xfa", CHARSET_GBK);

	mms_set_text_region(mms, SMIL_TO_PERCENT(0), SMIL_TO_PERCENT(0), 
		SMIL_TO_PERCENT(100), SMIL_TO_PERCENT(20));
	mms_set_image_region(mms, SMIL_TO_PERCENT(0), SMIL_TO_PERCENT(20), 
		SMIL_TO_PERCENT(100), SMIL_TO_PERCENT(80));

	par_index = mms_add_par(mms, 10000);
	mms_par_set_text_buffer(mms, par_index, "hello world", 0);
	mms_par_set_image(mms, par_index, "testdata/gol.gif");
	mms_par_set_audio(mms, par_index, "testdata/flourish.mid");

	mms_save_to_file(mms, "fake_gbk.mms");

	mms_set_to(mms, "13928462789/TYPE=PLMN", CHARSET_UTF_8);
	mms_set_subject(mms, "hello world", CHARSET_UTF_8);
	mms_save_to_file(mms, "fake_utf8.mms");

	mms_set_to(mms, "13928462789/TYPE=PLMN", CHARSET_GB2312);
	mms_set_subject(mms, "\xd6\xd0\xb9\xfa", CHARSET_GB2312);
	mms_save_to_file(mms, "fake_gb2312.mms");
	
	return mms;
}

static void test_recv(char* host, int port, char* url, char* filename)
{
	Mms* mms = NULL;
	MmsTransaction* thiz = NULL;
	
	thiz = mms_transaction_create(host, port, "mmsc.monternet.com", 80, 0);

	if(mms_transaction_receive(thiz, url, &mms) == MMS_RESULT_OK)
	{
		MmsOutputBuffer* output = mms_output_buffer_create(1024);
		mms_dump(mms, NULL);
		mms_pack(mms, output);
		mms_write_binary_file(filename, mms_output_buffer_get_buffer(output), mms_output_buffer_get_buffer_length(output));
		mms_output_buffer_destroy(output);
	}
	MMS_PRINTF("HTTP/1.1 %d\n", mms_transaction_get_resp_code(thiz));
	
	mms_transaction_destroy(thiz);

	return;
}

static void test_send(char* host, int port, char* url, char* filename)
{
	char*  buffer = NULL;
	size_t length = 0;
	Mms* mms = NULL;
	MmsHeader* header = NULL;
	MmsTransaction* thiz = NULL;
	MmsInputBuffer* input = NULL;

	if((buffer = mms_read_binary_file(filename, &length)) != NULL)
	{
		mms = mms_create();
		input = mms_input_buffer_create(buffer, length);
		mms_unpack(mms, input);
		mms_dump(mms, NULL);
		mms_fix_nokia(mms);
		thiz = mms_transaction_create(host, port, "mmsc.monternet.com", 80, 0);
		mms_transaction_send(thiz, url, mms, &header);
		MMS_PRINTF("HTTP/1.1 %d\n", mms_transaction_get_resp_code(thiz));
		mms_header_dump(header, NULL);
		mms_input_buffer_destroy(input);
		mms_destroy(mms);
		mms_free(buffer);
		mms_transaction_destroy(thiz);
	}

	return;
}

static void test_send_file(char* host, int port, char* url, char* filename)
{
	MmsHeader* header = NULL;
	MmsTransaction* thiz = NULL;

	thiz = mms_transaction_create(host, port, "mmsc.monternet.com", 80, 0);
	mms_transaction_send_file(thiz, url, filename, &header);
	
	MMS_PRINTF("HTTP/1.1 %d\n", mms_transaction_get_resp_code(thiz));
	mms_header_dump(header, NULL);
	mms_transaction_destroy(thiz);

	return;
}

static void test_send_fake(char* host, int port, char* url)
{
	MmsHeader* header = NULL;
	MmsTransaction* thiz = NULL;
	Mms* mms = construct_fake_mms();
	mms_dump(mms, NULL);
	thiz = mms_transaction_create(host, port, "mmsc.monternet.com", 80, 0);
	mms_transaction_send(thiz, url, mms, &header);
	mms_destroy(mms);

	MMS_PRINTF("HTTP/1.1 %d\n", mms_transaction_get_resp_code(thiz));
	mms_header_dump(header, NULL);
	mms_transaction_destroy(thiz);

	return;
}

int main(int argc, char* argv[])
{
	char* host     = NULL;
	int   port     = 0;
	char* url      = NULL;
	char* filename = NULL;
	
	if(argc < 2 || strcmp(argv[1], "--help") == 0)
	{
		MMS_PRINTF("usage: %s recv host port url file\n", argv[0]);
		MMS_PRINTF("       %s recv 10.0.0.172 80 http://211.139.144.165/LOAsR9qunOKB recv.mms\n", argv[0]);
		MMS_PRINTF("usage: %s send host port url file\n", argv[0]);
		MMS_PRINTF("       %s send 10.0.0.172 80 http://mmsc.monternet.com:80 ./testdata/tosendok.mms\n", argv[0]);
		MMS_PRINTF("usage: %s sendfile host port url file\n", argv[0]);
		MMS_PRINTF("usage: %s sendfake host port url file\n", argv[0]);
		MMS_PRINTF("usage: %s createfake\n", argv[0]);

		return -1;
	}

	host     = argv[2];
	port     = atoi(argv[3]);
	url      = argv[4];
	filename = argv[5];

	if(strcmp(argv[1], "recv") == 0)
	{
		test_recv(host, port, url, filename);
	}
	else if(strcmp(argv[1], "send") == 0)
	{
		test_send(host, port, url, filename);
	}
	else if(strcmp(argv[1], "sendfile") == 0)
	{
		test_send_file(host, port, url, filename);
	}
	else if(strcmp(argv[1], "sendfake") == 0)
	{
		test_send_fake(host, port, url);
	}
	else if(strcmp(argv[1], "createfake") == 0)
	{
		mms_destroy(construct_fake_mms());
	}

	return 0;
}

#endif/*MMS_TRANSACTION_TEST*/
