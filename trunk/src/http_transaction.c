/*
 * File:    http_transaction.c
 * Author:  Li XianJing <lixianjing@broncho.cn>
 * Brief:   a simple http client.
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
#include "mms_header.h"
#include "mms_socket.h"
#include "wsp_header_codec.h"
#include "http_transaction.h"

struct _HttpTransaction
{
	MmsSocket*       socket;
	MmsHeader*       req_header;
	MmsHeader*       resp_header;
	MmsOutputBuffer* req_buffer;
	MmsOutputBuffer* resp_buffer;

	int        resp_code;
	char*      resp_content;
	size_t     resp_content_length;
	
	size_t     timeout_usec;
};

#define HTTP_BUFFER_LENGTH 2048

HttpTransaction* http_transaction_create(char* host, int port, int timeout_usec)
{
	HttpTransaction* thiz = NULL;

	mms_return_val_if_fail(host != NULL && port > 0, NULL);

	thiz = (HttpTransaction*)mms_calloc(1, sizeof(HttpTransaction));

	if(thiz != NULL)
	{
		thiz->timeout_usec = timeout_usec > 0 ? timeout_usec : 10000;

		thiz->req_header  = mms_header_create(http_header_unpack_item, http_header_pack_item);
		thiz->resp_header = mms_header_create(http_header_unpack_item, http_header_pack_item);
		thiz->req_buffer  = mms_output_buffer_create(HTTP_BUFFER_LENGTH);
		thiz->resp_buffer = mms_output_buffer_create(HTTP_BUFFER_LENGTH);
		
		mms_header_set_unpack_end_at_content_type(thiz->resp_header, 0);
		mms_header_set_string(thiz->req_header, "Connection", "Close");

		thiz->socket = mms_socket_create();
		if (mms_socket_connect(thiz->socket, host, port) != MMS_RESULT_OK)
		{
			http_transaction_destroy (thiz);
			thiz = NULL;
		}
	}
	
	return thiz;
}

void       http_transaction_destroy(HttpTransaction* thiz)
{
	mms_return_if_fail (thiz != NULL);
	
	if (thiz->req_header != NULL)	
	{
		mms_header_destroy(thiz->req_header);
		thiz->req_header = NULL;
	}

	if (thiz->resp_header != NULL)
	{
		mms_header_destroy(thiz->resp_header);
		thiz->resp_header = NULL;
	}

	if (thiz->req_buffer != NULL)
	{
		mms_output_buffer_destroy(thiz->req_buffer);
		thiz->req_buffer = NULL;
	}

	if (thiz->resp_buffer != NULL)
	{
		mms_output_buffer_destroy(thiz->resp_buffer);
		thiz->resp_buffer = NULL;
	}
	
	if (thiz->socket != NULL)
	{
		mms_socket_destroy(thiz->socket);
		thiz->socket = NULL;
	}
	mms_free(thiz);

	return;
}

MmsResult  http_transaction_set_user_agent(HttpTransaction* thiz, char* user_agent)
{
	mms_return_val_if_fail(thiz != NULL && user_agent != NULL, MMS_RESULT_FAIL);

	return mms_header_set_string(thiz->req_header, "User-Agent", user_agent);
}

MmsResult  http_transaction_set_header(HttpTransaction* thiz, char* name, char* value)
{
	mms_return_val_if_fail(thiz != NULL && name != NULL, MMS_RESULT_FAIL);

	return mms_header_set_string(thiz->req_header, name, value);
}

static MmsResult http_transaction_clear(HttpTransaction* thiz)
{
	thiz->resp_code = 0;
	thiz->resp_content = NULL;
	thiz->resp_content_length = 0;

	mms_output_buffer_clear(thiz->req_buffer);
	mms_output_buffer_clear(thiz->resp_buffer);
	mms_header_clear(thiz->resp_header);

	return MMS_RESULT_OK;
}

static MmsResult http_transaction_parse_resp(HttpTransaction* thiz)
{
	size_t head_length = 0;
	char*  head_start = NULL;
	char*  body_start = NULL;
	char*  buffer = mms_output_buffer_get_buffer(thiz->resp_buffer);
	MmsInputBuffer* input = NULL;

	sscanf(buffer, "HTTP/1.1 %d", &(thiz->resp_code));

	if((head_start = strstr(buffer, "\r\n")) == NULL)
	{
		if((head_start = strstr(buffer, "\n")) == NULL)
		{
			mms_return_val_if_fail(head_start != NULL, MMS_RESULT_FAIL);
		}
		else
		{
			head_start++;
		}
	}
	else
	{
		head_start += 2;
	}

	if((body_start = strstr(head_start, "\r\n\r\n")) == NULL)
	{
		if((body_start = strstr(head_start, "\n\n")) == NULL)
		{
			mms_return_val_if_fail(head_start != NULL, MMS_RESULT_FAIL);
		}
	}
	else
	{
		body_start += 4;
	}

	head_length = body_start - head_start;
	
	input = mms_input_buffer_create(head_start, head_length);
	mms_header_unpack(thiz->resp_header, input);
	mms_input_buffer_destroy(input);

	thiz->resp_content = body_start;
	thiz->resp_content_length = mms_output_buffer_get_buffer_length(thiz->resp_buffer) - (body_start - buffer);

#ifdef _DEBUG
	mms_write_binary_file("httpresp.bin", thiz->resp_content, thiz->resp_content_length);
#endif
	return MMS_RESULT_OK;
}

static int http_transaction_get_remaining_size(const char* buffer, size_t length)
{
	int ret = 0;
	int clen = 0;
	const char* body = NULL;
	const char* clength = NULL;

	mms_return_val_if_fail(buffer != NULL, -1);

	clength = strstr(buffer, "Content-Length");
	mms_return_val_if_fail(clength != NULL, -1);
	
	clength += sizeof("Content-Length");
	if((body = strstr(clength, "\r\n\r\n")) == NULL)
	{
		if((body = strstr(clength, "\n\n")) == NULL)
		{
			return -1;
		}
		else
		{
			body += 2;
		}
	}
	else
	{
		body += 4;
	}
	
	while(clength[0] != '\0' 
		&& (clength[0] == ' ' || clength[0] == ':'))
	{
		clength++;
	}
	
	ret = sscanf(clength, "%d", &clen);
	mms_return_val_if_fail(ret == 1 && clen >= 0, -1);

	ret = clen - (length - (body - buffer));

	return ret;
}

static MmsResult  http_transaction_read_response(HttpTransaction* thiz)
{
	int rlength = 0;
	const char* all = NULL;
	char buffer[HTTP_BUFFER_LENGTH + 1] = {0};

	if(mms_socket_wait_for_data(thiz->socket, thiz->timeout_usec) == MMS_RESULT_OK)
	{
		int length = mms_socket_recv(thiz->socket, buffer, HTTP_BUFFER_LENGTH);

		while(length > 0)
		{
			mms_output_buffer_put_data(thiz->resp_buffer, buffer, length);
			
			rlength = http_transaction_get_remaining_size(
				mms_output_buffer_get_buffer(thiz->resp_buffer),
				mms_output_buffer_get_buffer_length(thiz->resp_buffer));
			rlength = rlength < 0 ? HTTP_BUFFER_LENGTH : rlength;

			if(rlength == 0) 
			{
				length = rlength;
				break;
			}

			if(mms_socket_wait_for_data(thiz->socket, thiz->timeout_usec) == MMS_RESULT_OK)
			{
				length = mms_socket_recv(thiz->socket, buffer, HTTP_BUFFER_LENGTH);
			}
			else
			{
				break;
			}
		}

		if(length > 0)
		{
			mms_output_buffer_put_data(thiz->resp_buffer, buffer, length);
		}

		return MMS_RESULT_OK;
	}
	else
	{
		return MMS_RESULT_FAIL;
	}
}

static MmsResult  http_transaction_get_response(HttpTransaction* thiz)
{
	if(http_transaction_read_response(thiz) == MMS_RESULT_OK)
	{
		return http_transaction_parse_resp(thiz);
	}
	else
	{
		return MMS_RESULT_FAIL;
	}
}

MmsResult  http_transaction_request(HttpTransaction* thiz, char* method, char* url, void* data, size_t data_length)
{
	MmsResult        ret = MMS_RESULT_FAIL;
	MmsOutputBuffer* req = thiz->req_buffer;
	size_t			send_bytes = 0;

	http_transaction_clear(thiz);
	
	mms_output_buffer_put_string_without_null(req, method);
	mms_output_buffer_put_string_without_null(req, " ");
	mms_output_buffer_put_string_without_null(req, url);
	mms_output_buffer_put_string_without_null(req, " HTTP/1.1\r\n");

	mms_header_set_long(thiz->req_header, "Content-Length", data_length);
	mms_header_pack(thiz->req_header, req);
	mms_output_buffer_put_string_without_null(req, "\r\n");

	if(data != NULL)
	{
		mms_output_buffer_put_data(req, data, data_length);
	}

	send_bytes = mms_socket_send(thiz->socket, (void*)mms_output_buffer_get_buffer(req), mms_output_buffer_get_buffer_length(req));

	mms_return_val_if_fail(send_bytes == mms_output_buffer_get_buffer_length(req), MMS_RESULT_FAIL);

	return http_transaction_get_response(thiz);
}

MmsResult  http_transaction_post_request(HttpTransaction* thiz, char* url, void* data, size_t length)
{
	mms_return_val_if_fail(thiz != NULL && url != NULL && data != NULL, MMS_RESULT_FAIL);

	return http_transaction_request(thiz, "POST", url, data, length);
}

MmsResult  http_transaction_get_request (HttpTransaction* thiz, char* url)
{
	return http_transaction_request(thiz, "GET", url, NULL, 0);
}

MmsResult  http_transaction_send_file(HttpTransaction* thiz, const char* filename)
{
	char* buffer = NULL;
	size_t length = 0;
	MmsResult ret = MMS_RESULT_FAIL;
	
	mms_return_val_if_fail(thiz != NULL && filename != NULL, MMS_RESULT_FAIL);
	buffer = mms_read_binary_file(filename, &length);
	mms_return_val_if_fail(buffer != NULL, MMS_RESULT_FAIL);
	ret = mms_socket_send(thiz->socket, buffer, length);
	mms_free(buffer);
	mms_return_val_if_fail(ret > 0, MMS_RESULT_FAIL);

	return http_transaction_get_response(thiz);;
}

int        http_transaction_get_resp_code(HttpTransaction* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);

	return thiz->resp_code;
}

char*      http_transaction_get_resp_header(HttpTransaction* thiz, char* key)
{
	mms_return_val_if_fail(thiz != NULL && key != NULL, NULL);

	return (char*)mms_header_get_string(thiz->resp_header, key);
}

size_t     http_transaction_get_resp_content_length(HttpTransaction* thiz)
{
	size_t content_length = 0;
	char* content_length_ptr = http_transaction_get_resp_header(thiz, "Content-Length");
	
	mms_return_val_if_fail(content_length_ptr != NULL, thiz->resp_content_length);
	
	sscanf(content_length_ptr, "%u", &content_length);

	//mms_assert(thiz->resp_content_length == content_length);
	
	return content_length;
}

char*      http_transaction_get_resp_content(HttpTransaction* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);

	return thiz->resp_content;
}

#ifdef HTTP_TRANSACTION_TEST

int main(int argc, char* argv[])
{
	char* buffer = NULL;
	size_t length = 0;
	HttpTransaction* thiz = NULL;
	char* method = NULL;
	char* host = NULL;
	int port = 0;
	char* url = NULL;
	char* filename = NULL;
	
	if(argc < 2 || strcmp(argv[1], "--help") == 0)
	{
		MMS_PRINTF("usage: %s get host port url filename\n", argv[0]);
		MMS_PRINTF("usage: %s post host port url filename\n", argv[0]);
		MMS_PRINTF("usage: %s file host port url filename\n", argv[0]);
		return -1;
	}

	method   = argv[1];
	host     = argv[2];
	port     = atoi(argv[3]);
	url      = argv[4];
	filename = argv[5];
	
	thiz   = http_transaction_create(host, port, 0);

	if(thiz != NULL)
	{
		if(strcmp(method, "get") == 0)
		{
			http_transaction_get_request(thiz, url);
		}
		else if(strcmp(method, "post") == 0)
		{
			buffer = mms_read_binary_file(filename, &length);
			if(buffer != NULL)
			{
				http_transaction_post_request(thiz, url, buffer, length);
				mms_free(buffer);
			}
		}
		else if(strcmp(method, "file"	) == 0)
		{
			http_transaction_send_file(thiz, filename);
		}

		MMS_PRINTF("HTTP/1.1 %d\n", http_transaction_get_resp_code(thiz));
		MMS_PRINTF("Content-Type: %s\n", http_transaction_get_resp_header(thiz, "Content-Type"));
		MMS_PRINTF("Content-Length: %d\n", http_transaction_get_resp_content_length(thiz));
		MMS_PRINTF("============================\n %s", http_transaction_get_resp_content(thiz));
		http_transaction_destroy(thiz);
	}
	
	return 0;
}
#endif/*HTTP_TRANSACTION_TEST*/

