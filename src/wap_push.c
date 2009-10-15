/*
 * File:    wap_push.c
 * Author:  Li XianJing <lixianjing@broncho.cn>
 * Brief:   
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
 * 2008-4-24 Li XianJing <lixianjing@broncho.cn> created
 *
 */

#include "mms_header.h"
#include "wap_push.h"
#include "mms_header_codec.h"
#include "wsp_header_codec.h"

struct _WapPush
{
	void*  buffer;
	size_t length;
	
	int    push_id;
	int    pdu_type;
	char*  content;
	size_t content_length;
	MmsContentType* content_type;
};

static MmsResult   wap_push_unpack(WapPush* thiz, MmsInputBuffer* input);

WapPush*  wap_push_create(void* buffer, size_t length, int callee_release)
{
	WapPush* thiz = NULL;
	
	mms_return_val_if_fail(buffer != NULL && length > 0, NULL);

	if((thiz = (WapPush*)mms_calloc(1, sizeof(WapPush))) != NULL)
	{
		int offset = 0;
		MmsInputBuffer* input = mms_input_buffer_create(buffer, length);

		wap_push_unpack(thiz, input);
		offset = mms_input_buffer_tell(input);
		mms_input_buffer_destroy(input);

		thiz->content_length = length - offset;
		if(callee_release)
		{
			thiz->buffer  = buffer;
			thiz->length  = length;
			thiz->content = buffer + offset;
		}
		else
		{
			thiz->content = mms_malloc(thiz->content_length + 1);
			memcpy(thiz->content, buffer + offset, thiz->content_length);
			thiz->content[thiz->content_length] = '\0';
		}
	}
	
	return thiz;
}

void        wap_push_destroy(WapPush* thiz)
{
	if(thiz != NULL)
	{
		if(thiz->buffer != NULL)
		{
			mms_free(thiz->buffer);
			thiz->buffer = NULL;
		}
		else
		{
			mms_free(thiz->content);
			thiz->content = NULL;
		}
		mms_content_type_destroy(thiz->content_type);
		thiz->content_type = NULL;
		mms_free(thiz);
	}

	return;
}

static MmsResult   wap_push_unpack(WapPush* thiz, MmsInputBuffer* input)
{
	size_t  wsp_header_length = 0;

	mms_return_val_if_fail(thiz != NULL && input != NULL, MMS_RESULT_FAIL);

	thiz->push_id = mms_input_buffer_get_int8(input);
	thiz->pdu_type = mms_input_buffer_get_int8(input);
	
	mms_return_val_if_fail(thiz->pdu_type == 0x06/*push*/, MMS_RESULT_FAIL);

	MmsHeader* wsp_header = mms_header_create(wsp_header_unpack_item, wsp_header_pack_item);
	wsp_header_length = mms_input_buffer_get_var_int(input);

	mms_input_buffer_push_limit(input, mms_input_buffer_tell(input) + wsp_header_length);
	thiz->content_type = mms_content_type_create();
	mms_content_type_unpack(thiz->content_type, input);
	mms_header_unpack(wsp_header, input);

	mms_input_buffer_pop_limit(input);
	
	mms_header_destroy(wsp_header);

	return MMS_RESULT_OK;
}

MmsResult   wap_push_dump(WapPush* thiz, void* unused)
{
	if(thiz != NULL)
	{
		MMS_PRINTF("push_id=%d\n", wap_push_get_push_id(thiz));
		MMS_PRINTF("content_type=%s\n", mms_content_type_get_content_type_name((MmsContentType*)wap_push_get_content_type(thiz)));
		MMS_PRINTF("content_length=%d\n", wap_push_get_content_length(thiz));
		MMS_PRINTF("content=%s\n", wap_push_get_content(thiz));
	}

	return MMS_RESULT_OK;
}

int                   wap_push_get_push_id(WapPush* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);

	return thiz->push_id;
}

int                   wap_push_get_pdu_type(WapPush* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);

	return thiz->pdu_type;
}

size_t                wap_push_get_content_length(WapPush* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);

	return thiz->content_length;
}

const char*           wap_push_get_content(WapPush* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);

	return thiz->content;
}

const MmsContentType* wap_push_get_content_type(WapPush* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);

	return thiz->content_type;
}

#ifdef WAP_PUSH_TEST
#include "mms_util.h"

int main(int argc, char* argv[])
{
	char* buffer = NULL;
	size_t length = 0;
	WapPush* thiz = NULL;
	MmsInputBuffer* input = NULL;
	
	if(argc != 2)
	{
		MMS_PRINTF("%s notify\n", argv[0]);
		return 0;
	}
	
	buffer = mms_read_binary_file(argv[1], &length);
	if(buffer != NULL)
	{
		thiz = wap_push_create(buffer, length, 1);

		wap_push_dump(thiz, NULL);

		mms_write_binary_file("wap_push.dat", (char*)wap_push_get_content(thiz), wap_push_get_content_length(thiz));
		wap_push_destroy(thiz);
	}

	return 0;
}
#endif/*WAP_PUSH_TEST*/

