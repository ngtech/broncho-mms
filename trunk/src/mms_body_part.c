/*
 * File:    mms_body_part.c
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

#include "mms_util.h"
#include "mms_list.h"
#include "wsp_header_codec.h"
#include "mms_header_codec.h"
#include "mms_body_part.h"

struct _MmsBodyPart
{
	MmsHeader* header;
	void*      data;
	size_t     length;

	MmsList*   auto_release;

	/*for debug info*/
	size_t header_offset;
	size_t data_offset;
};

MmsBodyPart* mms_body_part_create(void)
{
	MmsBodyPart* thiz = (MmsBodyPart*)mms_calloc(1, sizeof(MmsBodyPart));
	
	if(thiz != NULL)
	{	
		thiz->header = mms_header_create(wsp_header_unpack_item, wsp_header_pack_item);
	}

	return thiz;
}

void         mms_body_part_destroy(MmsBodyPart* thiz)
{
	if(thiz != NULL)
	{
		if(thiz->header != NULL)
		{
			mms_header_destroy(thiz->header);
			thiz->header = NULL;
		}
	
		if(thiz->auto_release != NULL)
		{
			mms_list_destroy(thiz->auto_release);
			thiz->auto_release = NULL;
		}
		
		mms_free(thiz);
	}

	return;
}

static void  mms_body_part_add_auto_release(MmsBodyPart* thiz, void* data)
{
	mms_return_if_fail(thiz != NULL && data != NULL);

	if(thiz->auto_release == NULL)
	{
		thiz->auto_release = mms_list_create((mms_destroy_func)mms_free);
	}

	mms_list_append(thiz->auto_release, data);

	return;
}

MmsResult    mms_body_part_unpack(MmsBodyPart* thiz, MmsInputBuffer* input)
{
	size_t header_length = 0;
	size_t body_length   = 0;
	MmsContentType* content_type = NULL;
	mms_return_val_if_fail(thiz != NULL && input != NULL, MMS_RESULT_FAIL);

	header_length = mms_input_buffer_get_var_int(input);
	body_length   = mms_input_buffer_get_var_int(input);

	thiz->header_offset = mms_input_buffer_tell(input);	
	
	content_type = mms_content_type_create();
	mms_input_buffer_push_limit(input, mms_input_buffer_tell(input) + header_length);

	/*the first item in body part header is content-type, it is diff with the normal header.*/
	mms_content_type_unpack(content_type, input);
	mms_header_unpack(thiz->header, input);	
	mms_header_set_content_type(thiz->header, content_type);
	
	mms_input_buffer_pop_limit(input);

	thiz->data_offset = mms_input_buffer_tell(input);	
	thiz->data   = mms_input_buffer_get_data(input, body_length);
	thiz->length = body_length;

	return MMS_RESULT_OK;
}

MmsResult    mms_body_part_pack(MmsBodyPart* thiz, MmsOutputBuffer* output)
{
	size_t header_length = 0;
	size_t data_length   = 0;
	
	MmsContentType*  content_type = NULL;
	MmsOutputBuffer* temp_output = NULL;

	mms_return_val_if_fail(thiz != NULL && output != NULL, MMS_RESULT_FAIL);

	content_type = mms_header_get_content_type(thiz->header);
	
	temp_output = mms_output_buffer_create(2048);
	mms_content_type_pack(content_type, temp_output);
	mms_header_pack(thiz->header, temp_output);
	header_length = mms_output_buffer_get_buffer_length(temp_output);

	if(thiz->data != NULL && thiz->length > 0)
	{
		mms_output_buffer_put_data(temp_output, thiz->data, thiz->length);
		data_length = mms_output_buffer_get_buffer_length(temp_output) - header_length;
	}

	mms_output_buffer_put_var_int(output, header_length);
	mms_output_buffer_put_var_int(output, data_length);

	mms_output_buffer_put_data(output, mms_output_buffer_get_buffer(temp_output), 
		mms_output_buffer_get_buffer_length(temp_output));

	mms_output_buffer_destroy(temp_output);

	return MMS_RESULT_OK;
}

MmsHeader*   mms_body_part_get_header(MmsBodyPart* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);

	return thiz->header;
}

MmsResult    mms_body_part_set_header(MmsBodyPart* thiz, MmsHeader* header)
{
	mms_return_val_if_fail(thiz != NULL && thiz->header != header, MMS_RESULT_FAIL);

	if(thiz->header != NULL)
	{
		mms_header_destroy(thiz->header);
		thiz->header = NULL;
	}
	
	thiz->header = header;

	return MMS_RESULT_OK;
}

MmsResult    mms_body_part_detach_header(MmsBodyPart* thiz)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	thiz->header = NULL;

	return MMS_RESULT_OK;
}

MmsResult    mms_body_part_set_data(MmsBodyPart* thiz, void* data, size_t length)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	thiz->data   = data;
	thiz->length = length;
	
	return MMS_RESULT_OK;
}

MmsResult    mms_body_part_set_name(MmsBodyPart* thiz, const char* filename)
{
	char value[MMS_VALUE_LENGTH + 1] = {0};
	MmsContentType* content_type = NULL;

	mms_return_val_if_fail(thiz != NULL && filename != NULL, MMS_RESULT_FAIL);

	if((content_type = mms_header_get_content_type(thiz->header)) == NULL)
	{
		content_type = mms_content_type_create();
	}

	mms_content_type_set_content_type_by_name(content_type, (char*)mms_get_file_content_type(filename));
	mms_content_type_set_param_name(content_type, (char*)mms_get_file_name(filename));
	
	mms_header_set_string(thiz->header, "Content-Location", mms_get_file_name(filename));
	sprintf(value, "<CID%u>", mms_get_unique_id());
	mms_header_set_string(thiz->header, "Content-ID", value);
	snprintf(value, MMS_VALUE_LENGTH, "http://orig.host/%s", mms_get_file_name(filename));
	mms_header_set_string(thiz->header, "X-Wap-Content-URI", value);

	if(mms_header_get_content_type(thiz->header) == NULL)
	{
		mms_header_set_content_type(thiz->header, content_type);
	}

	return MMS_RESULT_OK;
}

const char*  mms_body_part_get_name(MmsBodyPart* thiz)
{
	const char* name = NULL;
	MmsContentType* content_type = NULL;
	
	mms_return_val_if_fail(thiz != NULL, NULL);
	
	if((name = mms_header_get_string(thiz->header, "Content-Location")) != NULL)
	{
		return name;
	}

	content_type = mms_header_get_content_type(thiz->header);
	mms_return_val_if_fail(content_type != NULL, NULL);

	if((name = mms_content_type_get_param_name(content_type)) != NULL)
	{
		return name;
	}
	
	if((name = mms_content_type_get_param_filename(content_type)) != NULL)
	{
		return name;
	}
	
	if((name = mms_header_get_string(thiz->header, "Content-ID")) != NULL)
	{
		return name;
	}

	return NULL;
}

const char* mms_body_part_get_cid(MmsBodyPart* thiz)
{
	return mms_header_get_string(thiz->header, "Content-ID");
}

const char*  mms_body_part_get_data(MmsBodyPart* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);

	return thiz->data;
}

size_t       mms_body_part_get_data_length(MmsBodyPart* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);

	return thiz->length;
}

int          mms_body_part_compare_by_name(MmsBodyPart* thiz, const char* filename)
{
	const char* name = NULL;
	MmsContentType* content_type = NULL;
	
	filename = mms_get_file_name(filename);
	mms_return_val_if_fail(thiz != NULL && filename != NULL, -1);

	if((name = mms_header_get_string(thiz->header, "Content-ID")) != NULL && strcmp(name, filename) == 0)
	{
		return 0;
	}
	
	if((name = mms_header_get_string(thiz->header, "Content-Location")) != NULL && strcmp(name, filename) == 0)
	{
		return 0;
	}

	content_type = mms_header_get_content_type(thiz->header);
	mms_return_val_if_fail(content_type != NULL, 0);

	if((name = mms_content_type_get_param_name(content_type)) != NULL && strcmp(name, filename) == 0)
	{
		return 0;
	}
	
	if((name = mms_content_type_get_param_filename(content_type)) != NULL && strcmp(name, filename) == 0)
	{
		return 0;
	}
	
	return -1;
}

MmsResult  mms_body_part_dump(MmsBodyPart* thiz, void* unused)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	mms_header_dump(thiz->header, NULL);

	MMS_PRINTF("0x%04x data=%p data_length=%d\n", thiz->data_offset, thiz->data, thiz->length);

	return MMS_RESULT_OK;
}

const char*  mms_body_part_get_content_type(MmsBodyPart* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);

	return mms_content_type_get_content_type_name(mms_header_get_content_type(thiz->header));
}

MmsBodyPart* mms_body_part_create_from_buffer(const char* filename, void* buffer, size_t length, int callee_release)
{
	MmsBodyPart* thiz = NULL;

	mms_return_val_if_fail(filename != NULL && buffer != NULL && length > 0, NULL);
	
	thiz = mms_body_part_create();
		
	mms_body_part_set_name(thiz, filename);

	thiz->data   = buffer;
	thiz->length = length;

	if(callee_release)
	{
		mms_body_part_add_auto_release(thiz, buffer);
	}

	return thiz;
}

MmsBodyPart* mms_body_part_create_from_file(const char* filename)
{
	MmsBodyPart* thiz = NULL;
	char* buffer  = NULL;
	size_t length = 0;

	mms_return_val_if_fail(filename != NULL, NULL);
	if((buffer = mms_read_binary_file(filename, &length)) != NULL)
	{
		thiz = mms_body_part_create_from_buffer(filename, buffer, length, 1);
	}

	return thiz;
}

MmsResult    mms_body_part_save_to_file(MmsBodyPart* thiz, const char* filename)
{
	mms_return_val_if_fail(thiz != NULL && filename != NULL, MMS_RESULT_FAIL);

	return mms_write_binary_file(filename, thiz->data, thiz->length);
}

#ifdef MMS_BODY_PART_TEST
#include "mms_util.h"

static void test_from_file(const char* filename)
{
	MmsBodyPart* thiz = mms_body_part_create_from_file(filename);
	mms_assert(mms_body_part_compare_by_name(thiz, mms_get_file_name(filename)) == 0);
	mms_assert(strcmp(mms_body_part_get_name(thiz), mms_get_file_name(filename)) == 0);
	mms_body_part_dump(thiz, NULL);
	mms_body_part_destroy(thiz);

	return;
}

static void test_unpack(const char* filename)
{
    size_t length = 0;
    char*  buffer = NULL;
    HeaderItem* item = NULL;
    MmsInputBuffer* input = NULL;
    buffer = mms_read_binary_file(filename, &length);
    if((input = mms_input_buffer_create(buffer, length)) != NULL)
    {
    	long  i  = 0;
    	long  nr = 0;
    	MmsHeader*   header = mms_header_create(mms_header_unpack_item, mms_header_pack_item);
		MmsBodyPart* thiz = NULL;
	
    	mms_header_set_unpack_end_at_content_type(header, 1);
		mms_header_unpack(header, input);
		mms_header_dump(header, NULL);
		mms_header_destroy(header);

		nr = mms_input_buffer_get_var_int(input);

		thiz = mms_body_part_create();
		mms_body_part_unpack(thiz, input);
		mms_body_part_dump(thiz, NULL);
		mms_body_part_destroy(thiz);

		mms_input_buffer_destroy(input);
		mms_free(buffer);
	}

	return;
}

static void test_get_set(void)
{
	char buffer[1024] = {"aaaa"};

	MmsHeader* header = mms_header_create(wsp_header_unpack_item, wsp_header_pack_item);
	MmsBodyPart* thiz = mms_body_part_create();

	mms_assert(mms_body_part_get_header(thiz) != NULL);
	mms_assert(mms_body_part_set_header(thiz, header) == MMS_RESULT_OK);
	mms_assert(mms_body_part_get_header(thiz) == header);

	mms_assert(mms_body_part_set_data(thiz, buffer, sizeof(buffer)) == MMS_RESULT_OK);
	mms_assert(mms_body_part_get_data(thiz) == buffer);
	mms_assert(mms_body_part_get_data_length(thiz) == sizeof(buffer));
	
	mms_body_part_destroy(thiz);

	return;
}

int main(int argc, char* argv[])
{

	if(argc < 2 || strcmp(argv[1], "--help") == 0)
	{
		MMS_PRINTF("usage: %s test_from_file file\n", argv[0]);
		MMS_PRINTF("usage: %s test_unpack mms\n", argv[0]);
		MMS_PRINTF("usage: %s test_get_set\n", argv[0]);
		
		return -1;
	}

	if(strcmp(argv[1], "test_get_set") == 0)
	{
		test_get_set();
	}
	else if(strcmp(argv[1], "test_unpack") == 0)
	{
		test_unpack(argv[2]);
	}
	else if(strcmp(argv[1], "test_from_file") == 0)
	{
		test_from_file(argv[2]);
	}

	return 0;
}
#endif/*MMS_BODY_PART_TEST*/

