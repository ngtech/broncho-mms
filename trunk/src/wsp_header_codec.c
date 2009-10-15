/*
 * File:    wsp_header_codec.c
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
 * 2006-12-13 Li XianJing <lixianjing@broncho.cn> created
 *
 */

#include <assert.h>
#include <ctype.h>
#include "mms_util.h"
#include "mms_input_buffer.h"
#include "mms_output_buffer.h"
#include "wsp_header_codec.h"
#include "mms_content_type.h"

#undef WSP_HEADER_DEF
#define WSP_HEADER_DEF(id, value, name) {name, id},
static const NameIdPair g_wsp_headers[] =
{
	WSP_HEADERS
	{NULL, -1}
};
#undef WSP_HEADER_DEF

int              wsp_header_name_to_id(const char* name)
{
	return mms_name_to_id(g_wsp_headers, name);
}

const char*      wsp_header_id_to_name(int id)
{
	return mms_id_to_name(g_wsp_headers, id);
}

HeaderItem*      wsp_header_unpack_item(MmsInputBuffer* input)
{
	int           offset   = 0;
	long          long_int = 0;
	short         short_int= 0;
	long          length   = 0;
	char*         str      = NULL;
	unsigned char uchar    = 0;
	unsigned char id       = 0;
	HeaderItem*   item     = NULL;
	const char*   name     = NULL;

	mms_return_val_if_fail(input != NULL, NULL);

	if(mms_input_buffer_is_eof(input))
	{
		return NULL;
	}

	if(mms_input_buffer_is_string(input))
	{
		const char* value = NULL;
		name = mms_input_buffer_get_string(input);
		value = mms_input_buffer_get_string(input);

		return mms_header_item_string_create(name, value);
	}
	
	offset = mms_input_buffer_tell(input);
	id   = mms_input_buffer_get_int8(input) - 0x80;
	name = wsp_header_id_to_name(id);
	mms_assert(name != NULL);
	
	switch(id)
	{
		case WSP_HEADER_X_WAP_CONTENT_URI:
		case WSP_HEADER_CONTENT_LOCATION:
		{
			str = (char*)mms_input_buffer_get_string(input);
			item = mms_header_item_string_create(name, str);
			break;
		}
		case WSP_HEADER_DATE:
		{
			if(mms_input_buffer_is_string(input))
			{
				str = (char*)mms_input_buffer_get_string(input);
				item = mms_header_item_string_create(name, str);
			}
			else
			{
				long_int = mms_input_buffer_get_long_int(input);
				item = mms_header_item_long_create(name, long_int);
			}
			break;
		}
		default:
		{
			if(mms_input_buffer_is_string(input))
			{
				str = (char*)mms_input_buffer_get_string(input);
				item = mms_header_item_string_create(name, str);
			}
			else
			{
				long_int = mms_input_buffer_get_int(input);
				item = mms_header_item_long_create(name, long_int);
			}
			break;
		}
	};

	if(item != NULL)
	{
		item->offset = offset;
	}
	
	return item;
}

MmsResult        wsp_header_pack_item(HeaderItem* item, MmsOutputBuffer* output)
{
	int id = 0;
	unsigned long length = 0;

	mms_return_val_if_fail(item != NULL && item->name != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(output != NULL, MMS_RESULT_FAIL);

	if(mms_content_type_is_content_type(item->name))
	{
		/*XXX: pack in body_part*/	
		return MMS_RESULT_OK;
	}

	id = wsp_header_name_to_id(item->name);

	if(id < 0 || id == WSP_HEADER_CONTENT_ID)
	{
		mms_assert(item->type == MMS_TYPE_STRING);
		mms_output_buffer_put_string(output, item->name);
		mms_output_buffer_put_string(output, item->value.string);

		return MMS_RESULT_OK;
	}
	else
	{
		mms_output_buffer_put_short_int(output, id);
	}

	switch(id)
	{
		case WSP_HEADER_X_WAP_CONTENT_URI:
		case WSP_HEADER_CONTENT_LOCATION:
		{
			mms_output_buffer_put_string(output, item->value.string);
			break;
		}
		case WSP_HEADER_DATE:
		{
			if(item->type == MMS_TYPE_STRING)
			{
				mms_output_buffer_put_string(output, item->value.string);
			}
			else if(item->type == MMS_TYPE_LONG_INT)
			{
				mms_output_buffer_put_long_int(output, item->value.long_int);
			}
			break;
		}
		default:
		{
			if(item->type == MMS_TYPE_STRING)
			{
				mms_output_buffer_put_string(output, item->value.string);	
			}
			else if(item->type == MMS_TYPE_LONG_INT)
			{
				mms_output_buffer_put_long_int(output, item->value.long_int);
			}
			else
			{
				mms_assert(!"invalid header");
			}
			
			break;
		}
	};

	return MMS_RESULT_OK;
}

HeaderItem*      http_header_unpack_item(MmsInputBuffer* input)
{
	char* name  = NULL;
	char* value = NULL;
	HeaderItem* item = NULL;

	mms_return_val_if_fail(input != NULL, NULL);

	if(mms_input_buffer_is_eof(input))
	{
		return NULL;
	}
	name  = mms_input_buffer_get_string_end_with(input, ':');
	if(mms_input_buffer_is_eof(input))
	{
		return NULL;
	}
	value = mms_input_buffer_get_string_end_with(input, '\n'); 

	if(name != NULL && value != NULL)
	{
		size_t value_length = 0;
		while(isspace(name[0]) && name[0]) name++;
		while(isspace(value[0]) && value[0]) value++;
	
		value_length = strlen(value);
		if(value_length > 0 && value[value_length - 1] == '\r')
		{
			value[value_length - 1] = '\0';	
		}

		item = mms_header_item_string_create(name, value);
	}
	
	return item;
}

MmsResult        http_header_pack_item(HeaderItem* item, MmsOutputBuffer* output)
{
	char buffer[32] = {0};

	mms_return_val_if_fail(item != NULL && output != NULL, MMS_RESULT_FAIL);

	mms_output_buffer_put_string_without_null(output, item->name);
	mms_output_buffer_put_string_without_null(output, ": ");
	switch(item->type)
	{
		case MMS_TYPE_STRING:
		{
			mms_output_buffer_put_string_without_null(output, item->value.string);
			mms_output_buffer_put_string_without_null(output, "\r\n");
			break;
		}
		case MMS_TYPE_LONG_INT:
		{
			sprintf(buffer, "%d", item->value.long_int);
			mms_output_buffer_put_string_without_null(output, buffer);
			mms_output_buffer_put_string_without_null(output, "\r\n");
			break;
		}
		default:break;
	}

	return MMS_RESULT_OK;
}

#ifdef WSP_HEADER_CODEC_TEST
static void test_http()
{
	HeaderItem* item = NULL;
	char str[] = {"Content-Length: 1234\nContent-Type: text/html\n"};
	MmsInputBuffer* input = mms_input_buffer_create(str, strlen(str));
	MmsOutputBuffer* output = mms_output_buffer_create(1024);

	while((item = http_header_unpack_item(input)) != NULL)
	{
		http_header_pack_item(item, output);

		mms_header_item_destroy(item);
	}
	
	MMS_PRINTF("%s\n", mms_output_buffer_get_buffer(output));
	
	mms_output_buffer_destroy(output);
	mms_input_buffer_destroy(input);

	return;
}

int main(int argc, char* argv[])
{
	test_http();

	return 0;
}
#endif/*WSP_HEADER_CODEC_TEST*/
