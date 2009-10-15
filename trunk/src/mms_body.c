/*
 * File:    mms_body.c
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
 * 2007-4-19  Li XianJing <lixianjing@broncho.cn> fixed a bug in mms_body_pack,
 *            if(mms_smil_is_empty(thiz->smil) && length == 0), don't pack it.
 */

#include "mms_util.h"
#include "mms_body.h"
#include "mms_list.h"
#include "mms_input_buffer.h"
#include "mms_output_buffer.h"

struct _MmsBody
{
	MmsSmil* smil;
	MmsList* body_parts;
};

MmsBody*        mms_body_create(void)
{
	MmsBody* thiz = (MmsBody*)mms_calloc(1, sizeof(MmsBody));

	if(thiz != NULL)
	{
		thiz->smil       = mms_smil_create();
		thiz->body_parts = mms_list_create((mms_destroy_func)mms_body_part_destroy);
	}

	return thiz;
}

void            mms_body_destroy(MmsBody* thiz)
{
	if(thiz != NULL)
	{
		mms_smil_destroy(thiz->smil);
		thiz->smil = NULL;
		
		mms_list_destroy(thiz->body_parts);
		thiz->body_parts = NULL;
		
		mms_free(thiz);
	}

	return;
}

static MmsResult mms_body_reset(MmsBody* thiz)
{
	mms_list_destroy(thiz->body_parts);
	thiz->body_parts = mms_list_create((mms_destroy_func)mms_body_part_destroy);
	
	return MMS_RESULT_OK;
}

MmsResult		mms_body_fix_smil_filename(MmsBodyPart* part, MmsBody* thiz)
{
	const char* filename = mms_body_part_get_name(part);
	const char* cid = mms_body_part_get_cid (part);

	mms_smil_fix_filename_by_cid (thiz->smil, cid, filename);

	return MMS_RESULT_OK;
}

MmsResult        mms_body_unpack(MmsBody* thiz, MmsInputBuffer* input)
{
	long i  = 0;
	long nr = 0;
	MmsBodyPart* body_part = NULL;
	MmsContentType* content_type = NULL;
	const char* content_type_name = NULL;

	mms_return_val_if_fail(thiz != NULL && input != NULL, MMS_RESULT_FAIL);

	mms_body_reset(thiz);

	nr = mms_input_buffer_get_var_int(input);

	for(i = 0; i < nr && !mms_input_buffer_is_eof(input); i++)
	{
		body_part = mms_body_part_create();	
		mms_body_part_unpack(body_part, input);
	
		/*unpack smil*/
		content_type = mms_header_get_content_type(mms_body_part_get_header(body_part));
		if((content_type_name = mms_content_type_get_content_type_name(content_type)) != NULL
			&& strcmp(content_type_name, "application/smil") == 0)
		{
			MmsInputBuffer* smil_input = mms_input_buffer_create(mms_body_part_get_data(body_part),
											mms_body_part_get_data_length(body_part));
			mms_smil_unpack(thiz->smil, smil_input);
			
			mms_smil_set_header(thiz->smil, mms_body_part_get_header(body_part));
			mms_body_part_detach_header(body_part);

			mms_input_buffer_destroy(smil_input);
			mms_body_part_destroy(body_part);
		}
		else
		{
			mms_list_append(thiz->body_parts, body_part);
		}
	}
	mms_list_foreach (thiz->body_parts, (mms_visit_func)mms_body_fix_smil_filename, thiz);

	return MMS_RESULT_OK;
}

MmsResult        mms_body_pack(MmsBody* thiz, MmsOutputBuffer* output)
{
	size_t length = 0;
	mms_return_val_if_fail(thiz != NULL && output != NULL, MMS_RESULT_FAIL);
	
	length = mms_list_length(thiz->body_parts);

	if(!mms_smil_is_empty(thiz->smil))
	{
		length++;
		mms_output_buffer_put_value_length(output, length);
		mms_smil_pack_with_header(thiz->smil, output);
		mms_list_foreach(thiz->body_parts, (mms_visit_func)mms_body_part_pack, output);
	}
	else if(length > 0)
	{
		mms_output_buffer_put_value_length(output, length);
		mms_list_foreach(thiz->body_parts, (mms_visit_func)mms_body_part_pack, output);
	}
	
	return MMS_RESULT_OK;
}

MmsResult        mms_body_set_smil(MmsBody* thiz, MmsSmil* smil)
{
	mms_return_val_if_fail(thiz != NULL && thiz->smil != smil, MMS_RESULT_FAIL);

	if(thiz->smil != NULL)
	{
		mms_smil_destroy(thiz->smil);
		thiz->smil = NULL;
	}

	thiz->smil = smil;

	return MMS_RESULT_OK;
}

MmsSmil*         mms_body_get_smil(MmsBody* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);

	return thiz->smil;
}

MmsResult        mms_body_add_body_part(MmsBody* thiz, MmsBodyPart* body_part)
{
	mms_return_val_if_fail(thiz != NULL && body_part != NULL, MMS_RESULT_FAIL);

	return mms_list_append(thiz->body_parts, body_part);
}

MmsResult        mms_body_erase_body_part(MmsBody* thiz, const char* filename)
{
	mms_return_val_if_fail(thiz != NULL && filename != NULL, MMS_RESULT_FAIL);

	return mms_list_erase(thiz->body_parts, (mms_compare_func)mms_body_part_compare_by_name, (void*)filename);
}

MmsBodyPart*     mms_body_find_body_part(MmsBody* thiz, const char* filename)
{
	mms_return_val_if_fail(thiz != NULL && filename != NULL, NULL);

	return (MmsBodyPart*)mms_list_find(thiz->body_parts, (mms_compare_func)mms_body_part_compare_by_name, (void*)filename);
}

size_t          mms_body_get_body_part_count(MmsBody* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);

	return mms_list_length(thiz->body_parts);
}

MmsResult        mms_body_foreach(MmsBody* thiz, mms_visit_func visit, void* user_data)
{
	mms_return_val_if_fail(thiz != NULL && visit != NULL, MMS_RESULT_FAIL);

	return mms_list_foreach(thiz->body_parts, visit, user_data);
}

MmsResult        mms_body_dump(MmsBody* thiz, void* unused)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	mms_smil_dump(thiz->smil, NULL);

	mms_list_foreach(thiz->body_parts, (mms_visit_func)mms_body_part_dump, NULL);

	return MMS_RESULT_OK;
}

#ifdef MMS_BODY_TEST
#include "mms_util.h"
#include "mms_header.h"
#include "mms_header_codec.h"

static void test_unpack_pack(const char* filename)
{
    size_t length = 0;
    char*  buffer = NULL;
    HeaderItem* item = NULL;
    MmsInputBuffer* input = NULL;

    buffer = mms_read_binary_file(filename, &length);
    if((input = mms_input_buffer_create(buffer, length)) != NULL)
    {
    	MmsOutputBuffer* output = mms_output_buffer_create(1000);
    	MmsHeader* header = mms_header_create(mms_header_unpack_item, mms_header_pack_item);
		MmsBody* thiz = mms_body_create();

    	mms_header_set_unpack_end_at_content_type(header, 1);
		mms_header_unpack(header, input);
		mms_header_pack(header, output);

		mms_header_dump(header, NULL);
		mms_header_destroy(header);

		mms_body_unpack(thiz, input);
		mms_body_pack(thiz, output);
		mms_body_dump(thiz, NULL);

		mms_smil_dump_xml(mms_body_get_smil(thiz), output);

		MMS_PRINTF("=============================\n");
		MMS_PRINTF("%s\n", mms_output_buffer_get_buffer(output));
		MMS_PRINTF("=============================\n");
		mms_body_destroy(thiz);
		
		mms_write_binary_file("lxj.body", mms_output_buffer_get_buffer(output),
			mms_output_buffer_get_buffer_length(output));

		mms_output_buffer_destroy(output);
		mms_input_buffer_destroy(input);
		mms_free(buffer);
	}

	return;
}	

static void test_get_set(void)
{
	MmsBody* thiz = mms_body_create();
	MmsBodyPart* body_part = mms_body_part_create();
	MmsSmil* smil = mms_smil_create();

	mms_assert(mms_body_set_smil(thiz, smil) == MMS_RESULT_OK);
	mms_assert(mms_body_get_smil(thiz) == smil);

	mms_assert(mms_body_add_body_part(thiz, body_part) == MMS_RESULT_OK);
	
	mms_body_destroy(thiz);

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

	if(strcmp(argv[1], "test_unpack_pack") == 0)
	{
		test_unpack_pack(argv[2]);
	}
	else if(strcmp(argv[1], "test_get_set") == 0)
	{
		test_get_set();
	}

	return 0;
}
#endif/*MMS_BODY_TEST*/

