/*
 * File:    mms_header.c
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

#include "mms_list.h"
#include "mms_util.h"
#include "mms_header.h"
#include "mms_content_type.h"
#include "mms_header_codec.h"

struct _MmsHeader
{
	MmsList* items;
	header_pack_item_func   pack;
	header_unpack_item_func unpack;

	int unpack_end_at_content_type;
	
	/*temp vars*/
	MmsInputBuffer*  input;
	MmsOutputBuffer* output;
};

static int       mms_header_compare_by_name(void* data, void* name);

MmsHeader*       mms_header_create(header_unpack_item_func unpack, header_pack_item_func pack)
{
	MmsHeader* thiz = NULL;
	
	mms_return_val_if_fail(unpack != NULL && pack != NULL, NULL);

	thiz = (MmsHeader*)mms_calloc(1, sizeof(MmsHeader));

	if(thiz != NULL)
	{
		thiz->unpack_end_at_content_type = 0;
		thiz->items  = mms_list_create((mms_destroy_func)mms_header_item_destroy);
		thiz->pack   = pack;
		thiz->unpack = unpack;
	}
	
	return thiz;
}

void             mms_header_destroy(MmsHeader* thiz)
{
	if(thiz != NULL)
	{
		mms_list_destroy(thiz->items);
		thiz->items = NULL;

		mms_free(thiz);
	}

	return;
}

MmsResult        mms_header_set_unpack_end_at_content_type(MmsHeader* thiz, int unpack_end_at_content_type)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	thiz->unpack_end_at_content_type = unpack_end_at_content_type;

	return MMS_RESULT_OK;
}

static MmsResult mms_header_reset(MmsHeader* thiz)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	if(mms_list_length(thiz->items) > 0)
	{
		mms_list_destroy(thiz->items);
		thiz->items = mms_list_create((mms_destroy_func)mms_header_item_destroy);
	}

	return MMS_RESULT_OK;
}

MmsResult        mms_header_unpack(MmsHeader* thiz, MmsInputBuffer* input)
{
	HeaderItem* item = NULL;

	mms_return_val_if_fail(thiz != NULL && input != NULL, MMS_RESULT_FAIL);

	mms_header_reset(thiz);

	while((item = thiz->unpack(input)) != NULL)
	{
		mms_list_append(thiz->items, item);

		if(mms_content_type_is_content_type(item->name) && thiz->unpack_end_at_content_type)
		{
			break;
		}
	}

	return mms_list_length(thiz->items) > 0 ? MMS_RESULT_OK : MMS_RESULT_FAIL;
}

MmsResult        mms_header_pack(MmsHeader* thiz, MmsOutputBuffer* output)
{
	mms_return_val_if_fail(thiz != NULL && output != NULL, MMS_RESULT_FAIL);

	thiz->output = output;

	if(thiz->unpack_end_at_content_type)
	{
		HeaderItem* content_type_item = NULL;
		
		mms_list_remove(thiz->items, mms_header_compare_by_name, "Content-Type", (void**)&content_type_item);
		if(content_type_item != NULL)
		{
			mms_list_append(thiz->items, content_type_item);
		}
	}

	mms_list_foreach(thiz->items, (mms_visit_func)thiz->pack, output);
	thiz->output = NULL;

	return MMS_RESULT_OK;
}

static int       mms_header_compare_by_name(void* data, void* name)
{
	HeaderItem* item = (HeaderItem*)data;

	return strcasecmp(item->name, name);
}

MmsResult        mms_header_set_string(MmsHeader* thiz, const char* name, const char* value)
{
	HeaderItem* item = NULL;
	mms_return_val_if_fail(thiz != NULL && name != NULL && value != NULL && *value != '\0', MMS_RESULT_FAIL);

	if((item = mms_list_find(thiz->items, mms_header_compare_by_name, (void*)name)) != NULL
		&& item->type == MMS_TYPE_STRING)
	{
		mms_free(item->value.string);
		item->value.string = mms_strdup(value);
	}
	else
	{
		mms_list_erase(thiz->items, mms_header_compare_by_name, (void*)name);
		item = mms_header_item_string_create(name, value);
		mms_list_append(thiz->items, item);
	}

	return MMS_RESULT_OK;
}

MmsResult  mms_header_set_encoded_string(MmsHeader* thiz, const char* name, void* value, int charset)
{
	HeaderItem* item = NULL;
	mms_return_val_if_fail(thiz != NULL && name != NULL 
		&& value != NULL  && *(char*)value != '\0' && charset > 0, MMS_RESULT_FAIL);
	
	if((item = mms_list_find(thiz->items, mms_header_compare_by_name, (void*)name)) != NULL
		&& item->type == MMS_TYPE_ENCODED_STRING)
	{
		mms_free(item->value.encoded_string.string);
		item->value.encoded_string.string  = mms_strdup(value);
		item->value.encoded_string.charset = charset;
	}
	else
	{
		mms_list_erase(thiz->items, mms_header_compare_by_name, (void*)name);
		item = mms_header_item_encoded_string_create(name, value, charset);
		mms_list_append(thiz->items, item);
	}

	return MMS_RESULT_OK;
}

MmsResult        mms_header_set_long(MmsHeader* thiz, const char* name, unsigned long value)
{
	HeaderItem* item = NULL;
	mms_return_val_if_fail(thiz != NULL && name != NULL, MMS_RESULT_FAIL);

	if((item = mms_list_find(thiz->items, mms_header_compare_by_name, (void*)name)) != NULL
		&& item->type == MMS_TYPE_LONG_INT)
	{
		item->value.long_int = value;

		return MMS_RESULT_OK;
	}
	else
	{
		mms_list_erase(thiz->items, mms_header_compare_by_name, (void*)name);
		item = mms_header_item_long_create(name, value);
		mms_list_append(thiz->items, item);
	}

	return MMS_RESULT_OK;
}

MmsResult        mms_header_set_time(MmsHeader* thiz, const char* name, int is_relative, unsigned long value)
{
	HeaderItem* item = NULL;
	unsigned char rel_abs = is_relative ? MMS_VAL_RELATIVE_TOKEN : MMS_VAL_ABSOLUTE_TOKEN;
	mms_return_val_if_fail(thiz != NULL && name != NULL, MMS_RESULT_FAIL);

	if((item = mms_list_find(thiz->items, mms_header_compare_by_name, (void*)name)) != NULL
		&& item->type == MMS_TYPE_TIME)
	{
		item->value.time.time = value;
		item->value.time.rel_abs = rel_abs;

		return MMS_RESULT_OK;
	}
	else
	{
		mms_list_erase(thiz->items, mms_header_compare_by_name, (void*)name);
		item = mms_header_item_time_create(name, rel_abs, value);
		mms_list_append(thiz->items, item);
	}

	return MMS_RESULT_OK;

}

MmsResult        mms_header_set_content_type(MmsHeader* thiz, MmsContentType* content_type)
{
	HeaderItem* item = NULL;
	mms_return_val_if_fail(thiz != NULL && content_type != NULL, MMS_RESULT_FAIL);

	mms_list_erase(thiz->items, mms_header_compare_by_name,	(void*)MMS_CONTENT_TYPE);
	item = mms_header_item_data_create(MMS_CONTENT_TYPE, content_type, (mms_destroy_func)mms_content_type_destroy);
	mms_list_append(thiz->items, item);

	return MMS_RESULT_OK;
}

MmsResult        mms_header_erase(MmsHeader* thiz, const char* name)
{
	mms_return_val_if_fail(thiz != NULL && name != NULL, MMS_RESULT_FAIL);

	mms_list_erase(thiz->items, mms_header_compare_by_name, (void*)name);

	return MMS_RESULT_OK;
}

const char*      mms_header_get_string(MmsHeader* thiz, const char* name)
{
	HeaderItem* item = NULL;
	mms_return_val_if_fail(thiz != NULL && name != NULL, NULL);

	item = mms_list_find(thiz->items, mms_header_compare_by_name, (void*)name);

	mms_return_val_if_fail(item != NULL && item->type == MMS_TYPE_STRING, NULL);

	return item->value.string;	
}

const char*      mms_header_get_encoded_string(MmsHeader* thiz, const char* name, int* charset)
{
	HeaderItem* item = NULL;
	mms_return_val_if_fail(thiz != NULL && name != NULL, NULL);
	
	item = mms_list_find(thiz->items, mms_header_compare_by_name, (void*)name);
	mms_return_val_if_fail(item != NULL && item->type == MMS_TYPE_ENCODED_STRING, NULL);
	
	if(charset != NULL)
	{
		*charset = item->value.encoded_string.charset;
	}

	return item->value.encoded_string.string;
}

unsigned long              mms_header_get_long(MmsHeader* thiz, const char* name)
{
	HeaderItem* item = NULL;
	mms_return_val_if_fail(thiz != NULL && name != NULL, 0);

	item = mms_list_find(thiz->items, mms_header_compare_by_name, (void*)name);

	mms_return_val_if_fail(item != NULL && item->type == MMS_TYPE_LONG_INT, 0);

	return item->value.long_int;
}

unsigned long    mms_header_get_time(MmsHeader* thiz, const char* name, int* is_relative)
{
	HeaderItem* item = NULL;
	mms_return_val_if_fail(thiz != NULL && name != NULL, 0);

	item = mms_list_find(thiz->items, mms_header_compare_by_name, (void*)name);

	mms_return_val_if_fail(item != NULL && item->type == MMS_TYPE_TIME, 0);
		
	if(is_relative != NULL)
	{
		*is_relative = item->value.time.rel_abs == MMS_VAL_RELATIVE_TOKEN;
	}
		
	return item->value.time.time;
}

MmsContentType*  mms_header_get_content_type(MmsHeader* thiz)
{
	HeaderItem* item = NULL;
	mms_return_val_if_fail(thiz != NULL, NULL);

	item = mms_list_find(thiz->items, mms_header_compare_by_name, (void*)MMS_CONTENT_TYPE);
	
	if(item != NULL)
	{
		return (MmsContentType*)item->value.data.data;
	}
	else
	{
		return NULL;
	}
}

MmsResult        mms_header_clear(MmsHeader* thiz)
{
	return mms_header_reset(thiz);
}

MmsResult mms_header_dump(MmsHeader* thiz, void* unused)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	mms_list_foreach(thiz->items, (mms_visit_func)mms_header_item_dump, NULL);

	MMS_PRINTF("=======================================\n");
	
	return MMS_RESULT_OK;
}

MmsDataType      mms_header_get_item_type(MmsHeader* thiz, const char* name)
{
	HeaderItem* item = NULL;
	mms_return_val_if_fail(thiz != NULL && name != NULL, MMS_TYPE_NONE);

	item = mms_list_find(thiz->items, mms_header_compare_by_name, (void*)name);

	mms_return_val_if_fail(item != NULL, MMS_TYPE_NONE);

	return item->type;
}

#ifdef MMS_HEADER_TEST

static void test_encoded_string(MmsHeader* thiz, char* name, char* value, int charset)
{
	size_t length = 0;
	int    ret_charset = 0;
	
	mms_assert(mms_header_set_encoded_string(thiz, name, value, charset) == MMS_RESULT_OK);
	mms_assert(strcmp(mms_header_get_encoded_string(thiz, name, &ret_charset), value) == 0);
	mms_assert(charset == ret_charset);
	mms_assert(mms_header_get_item_type(thiz, name) == MMS_TYPE_ENCODED_STRING);

	return;
}
static void test_get_set(void)
{
	int i = 0;
	int is_relative = 0;
	int charset = 0;
	size_t length = 0;
	
	MmsHeader* thiz = mms_header_create(mms_header_unpack_item, mms_header_pack_item);

	for(i = 0; i < 10; i++)
	{
		mms_assert(mms_header_set_string(thiz, "string", "string") == MMS_RESULT_OK);
		mms_assert(strcmp(mms_header_get_string(thiz, "string"), "string") == 0);
		mms_assert(mms_header_get_item_type(thiz, "string") == MMS_TYPE_STRING);

		mms_assert(mms_header_set_long(thiz, "long", 1234) == MMS_RESULT_OK);
		mms_assert(mms_header_get_long(thiz, "long") ==  1234);
		mms_assert(mms_header_get_item_type(thiz, "long") == MMS_TYPE_LONG_INT);

		mms_assert(mms_header_set_time(thiz, "expire", 1, 1234) == MMS_RESULT_OK);
		mms_assert(mms_header_get_time(thiz, "expire", &is_relative) == 1234 && is_relative);
		mms_assert(mms_header_get_item_type(thiz, "expire") == MMS_TYPE_TIME);

		mms_assert(mms_header_set_time(thiz, "time", 0, 1234) == MMS_RESULT_OK);
		mms_assert(mms_header_get_time(thiz, "time", &is_relative) == 1234 && !is_relative);

		test_encoded_string(thiz, "Subject", "Hello world", 106);
		test_encoded_string(thiz, "Cc", "1392447823", 113);
		test_encoded_string(thiz, "To", "asdfasd", 114);
	}
	mms_header_dump(thiz, NULL);

	mms_header_destroy(thiz);

	return;
}

static void test_unpack_pack(const char* filename)
{
	size_t length = 0;
	char*  buffer = NULL;
	HeaderItem* item = NULL;
	MmsInputBuffer* input = NULL;
	MmsOutputBuffer* output = mms_output_buffer_create(1024);

	buffer = mms_read_binary_file(filename, &length);
	if((input = mms_input_buffer_create(buffer, length)) != NULL)
	{
		MmsHeader* thiz = mms_header_create(mms_header_unpack_item, mms_header_pack_item);

    	mms_header_set_unpack_end_at_content_type(thiz, 1);
		mms_header_unpack(thiz, input);
		mms_header_pack(thiz, output);
		mms_header_dump(thiz, NULL);
		
		mms_write_binary_file("lxj.head", mms_output_buffer_get_buffer(output), 
			mms_output_buffer_get_buffer_length(output));

		mms_header_destroy(thiz);
		mms_input_buffer_destroy(input);
		mms_output_buffer_destroy(output);

		mms_free(buffer);
	}

	return;
}

int main(int argc, char* argv[])
{
	if(argc < 2 || strcmp(argv[1], "--help") == 0)
	{
		MMS_PRINTF("usage: %s test_get_set\n", argv[0]);
		MMS_PRINTF("usage: %s test_unpack_pack mms\n", argv[0]);

		return -1;
	}
	
	if(strcmp(argv[1], "test_get_set") == 0)
	{
		test_get_set();
	}
	else if(strcmp(argv[1], "test_unpack_pack") == 0)
	{
		test_unpack_pack(argv[2]);
	}

	return 0;
}
#endif/*MMS_HEADER_TEST*/

