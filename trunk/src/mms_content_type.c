/*
 * File:    mms_content_type.c
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
#include "mms_input_buffer.h"
#include "mms_output_buffer.h"
#include "mms_content_type.h"

#undef  MMS_CODEC_DEF
#define MMS_CODEC_DEF(id, value, name) {name, id},
static NameIdPair g_content_types[] =
{
	MMS_CONTENT_TYPES
	{NULL, -1}
};
#undef MMS_CODEC_DEF

struct _MmsContentType
{
	int   id;
	char name[MMS_NAME_LENGTH + 1];
	
	char param_type[MMS_NAME_LENGTH + 1];
	char param_name[MMS_MAX_PATH + 1];
	char param_filename[MMS_MAX_PATH + 1];
	char param_start[MMS_NAME_LENGTH + 1];
	char param_charset[MMS_NAME_LENGTH + 1];
};

MmsContentType* mms_content_type_create(void)
{
	MmsContentType* thiz = (MmsContentType*)mms_calloc(1, sizeof(MmsContentType));

	if(thiz != NULL)
	{
		thiz->id = -1;
	}

	return thiz;
}

void            mms_content_type_destroy(MmsContentType* thiz)
{
	if(thiz != NULL)
	{
		mms_free(thiz);
	}

	return;
}

static int             mms_content_type_has_params(MmsContentType* thiz)
{
	return thiz->param_name[0]
		|| thiz->param_type[0]
		|| thiz->param_filename[0]
		|| thiz->param_start[0];
}

MmsResult       mms_content_type_unpack(MmsContentType* thiz, MmsInputBuffer* input)
{
	size_t length = 0;
	size_t end    = 0;
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(input != NULL, MMS_RESULT_FAIL);

	if(mms_input_buffer_is_short_int(input))
	{
		int id = mms_input_buffer_get_short_int(input);
		mms_content_type_set_content_type_by_id(thiz, id);
	}
	else if(mms_input_buffer_is_string(input))
	{
		char* name = mms_input_buffer_get_string(input);
		mms_content_type_set_content_type_by_name(thiz, name);
	}
	else
	{
		length = mms_input_buffer_get_value_length(input);
		end = mms_input_buffer_tell(input) + length;
		
		if(mms_input_buffer_is_short_int(input))
		{
			int id = mms_input_buffer_get_short_int(input);
			mms_content_type_set_content_type_by_id(thiz, id);
		}
		else if(mms_input_buffer_is_string(input))
		{
			char* name = mms_input_buffer_get_string(input);
			mms_content_type_set_content_type_by_name(thiz, name);
		}
		else
		{
			mms_assert(!"invalid content type");
		}
		while(mms_input_buffer_tell(input) < end)
		{
			int opt = mms_input_buffer_get_short_int(input);

			switch(opt)
			{
				case CONTENT_TYPE_PARAM_NAME:
				{
					char* name = mms_input_buffer_get_string(input);
					mms_content_type_set_param_name(thiz, name);
					break;
				}
				case CONTENT_TYPE_PARAM_CHARSET:
				{
					int charset = mms_input_buffer_get_int(input);

					break;
				}
				case CONTENT_TYPE_PARAM_TYPE_RELATED:
				{
					if(mms_input_buffer_is_short_int(input))
					{
						int type = mms_input_buffer_get_short_int(input);

						mms_assert(type >=0 && type < CONTENT_TYPE_NR);
						if(type >=0 && type < CONTENT_TYPE_NR)
						{
							mms_content_type_set_param_type(thiz, g_content_types[type].name);
						}
					}
					else
					{
						char* type = mms_input_buffer_get_string(input);
						mms_content_type_set_param_type(thiz, type);		
					}
					break;
				}
				case CONTENT_TYPE_PARAM_START:
				{
					char* start = mms_input_buffer_get_string(input);
					mms_content_type_set_param_start(thiz, start);
					break;
				}
				default:
				{
					if(mms_input_buffer_is_string(input))
					{
						mms_input_buffer_get_string(input);
					}
					else
					{
						mms_input_buffer_get_int(input);
					}
					break;
				}
			}
		}
	}	
	
	return MMS_RESULT_OK;
}

MmsResult       mms_content_type_pack(MmsContentType* thiz, MmsOutputBuffer* output)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	
	if(mms_content_type_has_params(thiz))
	{
		MmsOutputBuffer* temp = mms_output_buffer_create(256);
		if(thiz->id >= 0 && thiz->id < CONTENT_TYPE_NR)
		{
			mms_output_buffer_put_short_int(temp, (unsigned char)thiz->id);
		}
		else if(thiz->name[0])
		{
			mms_output_buffer_put_string(temp, thiz->name);
		}
		else
		{
			mms_assert(!"invalid content-type");
		}
		
		if(thiz->param_type[0])
		{
			int id = mms_name_to_id(g_content_types,  thiz->param_type);
			
			mms_output_buffer_put_short_int(temp, (unsigned char)CONTENT_TYPE_PARAM_TYPE_RELATED);
			if(id >= 0 && id < CONTENT_TYPE_NR)
			{
				mms_output_buffer_put_short_int(temp, id);
			}
			else
			{
				mms_output_buffer_put_string(temp, thiz->param_type);
			}
		}

		if(thiz->param_name[0])
		{
			mms_output_buffer_put_short_int(temp, (unsigned char)CONTENT_TYPE_PARAM_NAME);
			mms_output_buffer_put_string(temp, thiz->param_name);
		}
		
		if(thiz->param_filename[0])
		{
			mms_output_buffer_put_short_int(temp, (unsigned char)CONTENT_TYPE_PARAM_FILENAME);
			mms_output_buffer_put_string(temp, thiz->param_filename);
		}

		if(thiz->param_start[0])
		{
			mms_output_buffer_put_short_int(temp, (unsigned char)CONTENT_TYPE_PARAM_START);
			mms_output_buffer_put_string(temp, thiz->param_start);
		}

		//mms_output_buffer_put_int8(output, 0x1f); // esc
		mms_output_buffer_put_value_length(output, mms_output_buffer_get_buffer_length(temp));
		mms_output_buffer_put_data(output, mms_output_buffer_get_buffer(temp), mms_output_buffer_get_buffer_length(temp));

		mms_output_buffer_destroy(temp);
	}
	else
	{
		if(thiz->id >= 0 && thiz->id < CONTENT_TYPE_NR)
		{
			mms_output_buffer_put_short_int(output, thiz->id);
		}
		else if(thiz->name != NULL)
		{
			mms_output_buffer_put_string(output, thiz->name);
		}
		else
		{
			mms_assert(!"invalid content-type");
		}
	}


	
	return MMS_RESULT_OK;
}

int             mms_content_type_is_multi_part(MmsContentType* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);
	mms_return_val_if_fail(thiz->id < CONTENT_TYPE_NR, 0);

	if(thiz->id < 0)
	{
		if(thiz->name != NULL && strncmp(thiz->name, "multipart/", strlen("multipart/")) == 0)
		{
			return !0;
		}
	}
	else
	{
		if(thiz->name == NULL)
		{
			if(strncmp(thiz->name,  "multipart", strlen("multipart")) == 0
				|| strncmp(thiz->name,  "application/vnd.wap.multipart", strlen("application/vnd.wap.multipart")) == 0)
			{
				return !0;
			}
		}
	}

	return 0;
}

MmsResult       mms_content_type_set_content_type_by_id(MmsContentType* thiz, int id)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(id >= 0 && id < CONTENT_TYPE_NR, MMS_RESULT_FAIL);

	thiz->id   = id;
	strncpy(thiz->name, g_content_types[id].name, MMS_NAME_LENGTH);

	return MMS_RESULT_OK;
}

int             mms_content_type_get_content_type_id(MmsContentType* thiz)
{
	mms_return_val_if_fail(thiz != NULL, -1);

	return thiz->id;
}

MmsResult       mms_content_type_set_content_type_by_name(MmsContentType* thiz, char* name)
{
	mms_return_val_if_fail(thiz != NULL && name != NULL, MMS_RESULT_FAIL);

	thiz->id   = mms_name_to_id(g_content_types, name);
	strncpy(thiz->name, name, MMS_NAME_LENGTH);

	return MMS_RESULT_OK;
}

MmsResult       mms_content_type_set_param_name(MmsContentType* thiz, char* name)
{
	mms_return_val_if_fail(thiz != NULL && name != NULL, MMS_RESULT_FAIL);

	strncpy(thiz->param_name, name, MMS_MAX_PATH);

	return MMS_RESULT_OK;
}

MmsResult       mms_content_type_set_param_filename(MmsContentType* thiz, char* filename)
{
	mms_return_val_if_fail(thiz != NULL && filename != NULL, MMS_RESULT_FAIL);

	strncpy(thiz->param_filename, filename, MMS_MAX_PATH);

	return MMS_RESULT_OK;
}

MmsResult       mms_content_type_set_param_type(MmsContentType* thiz, char* type)
{
	mms_return_val_if_fail(thiz != NULL && type != NULL, MMS_RESULT_FAIL);

	strncpy(thiz->param_type, type, MMS_NAME_LENGTH);
	
	return MMS_RESULT_OK;
}

MmsResult       mms_content_type_set_param_start(MmsContentType* thiz, char* start)
{
	mms_return_val_if_fail(thiz != NULL && start != NULL, MMS_RESULT_FAIL);

	strncpy(thiz->param_start, start, MMS_NAME_LENGTH);
	
	return MMS_RESULT_OK;
}

MmsResult       mms_content_type_set_param_charset(MmsContentType* thiz, char* charset)
{
	mms_return_val_if_fail(thiz != NULL && charset != NULL, MMS_RESULT_FAIL);

	strncpy(thiz->param_charset, charset, MMS_NAME_LENGTH);

	return MMS_RESULT_OK;
}

const char*     mms_content_type_get_content_type_name(MmsContentType* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);
	mms_return_val_if_fail(strlen (thiz->name) != 0, NULL);

	return thiz->name;
}

const char*     mms_content_type_get_param_name(MmsContentType* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);
	mms_return_val_if_fail(strlen (thiz->param_name) != 0, NULL);

	return thiz->param_name;
}

const char*     mms_content_type_get_param_filename(MmsContentType* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);
	mms_return_val_if_fail(strlen (thiz->param_filename) != 0, NULL);

	return thiz->param_filename;
}

const char*     mms_content_type_get_param_type(MmsContentType* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);
	mms_return_val_if_fail(strlen (thiz->param_type) != 0, NULL);

	return thiz->param_type;
}

const char*     mms_content_type_get_param_start(MmsContentType* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);
	mms_return_val_if_fail(strlen (thiz->param_start) != 0, NULL);

	return thiz->param_start;
}

const char*     mms_content_type_get_param_charset(MmsContentType* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);
	mms_return_val_if_fail(strlen (thiz->param_charset) != 0, NULL);

	return thiz->param_charset;
}

int             mms_content_type_is_content_type(const char* name)
{
	mms_return_val_if_fail(name, 0);

	return (strcmp(name, "Content-Type") == 0);
}

MmsResult       mms_content_type_dump(MmsContentType* thiz, void* unused)
{
	mms_return_val_if_fail(thiz, MMS_RESULT_FAIL);

	MMS_PRINTF(": %s start=%s type=%s name=%s\n", thiz->name, 
		thiz->param_start, thiz->param_type, thiz->param_name);
	
	return MMS_RESULT_OK;
}

#ifdef MMS_CONTENT_TYPE_TEST

static void test_get_set(void)
{
	int i = 0;
	MmsOutputBuffer* output = NULL;
	MmsInputBuffer*  input  = NULL;
	MmsContentType* thiz = mms_content_type_create();

	for(i = 0; g_content_types[i].name != NULL; i++)	
	{
		mms_assert(mms_content_type_set_content_type_by_name(thiz, g_content_types[i].name) == MMS_RESULT_OK);
		mms_assert(strcmp(mms_content_type_get_content_type_name(thiz), g_content_types[i].name) == 0);
		mms_assert(mms_content_type_get_content_type_id(thiz) == g_content_types[i].id);
		
		mms_assert(mms_content_type_set_content_type_by_id(thiz, g_content_types[i].id) == MMS_RESULT_OK);
		mms_assert(strcmp(mms_content_type_get_content_type_name(thiz), g_content_types[i].name) == 0);
		mms_assert(mms_content_type_get_content_type_id(thiz) == g_content_types[i].id);
	}

	mms_assert(mms_content_type_set_content_type_by_name(thiz, "application/test") == MMS_RESULT_OK);
	mms_assert(mms_content_type_get_content_type_id(thiz) == -1);

	mms_assert(mms_content_type_set_param_start(thiz, "start") == MMS_RESULT_OK);
	mms_assert(strcmp(mms_content_type_get_param_start(thiz), "start") == 0);
	
	mms_assert(mms_content_type_set_param_type(thiz, "type") == MMS_RESULT_OK);
	mms_assert(strcmp(mms_content_type_get_param_type(thiz), "type") == 0);
	
	mms_assert(mms_content_type_set_param_filename(thiz, "filename") == MMS_RESULT_OK);
	mms_assert(strcmp(mms_content_type_get_param_filename(thiz), "filename") == 0);

	mms_assert(mms_content_type_set_param_name(thiz, "name") == MMS_RESULT_OK);
	mms_assert(strcmp(mms_content_type_get_param_name(thiz), "name") == 0);
		
	mms_content_type_destroy(thiz);

	return ;
}

int main(int argc, char* argv[])
{
	if(argc < 2 || strcmp(argv[1], "--help") == 0)
	{
		MMS_PRINTF("usage: %s test_get_set\n", argv[0]);

		return -1;
	}

	if(strcmp(argv[1], "test_get_set") == 0)
	{
		test_get_set();
	}
	
	return 0;
}
#endif/*MMS_CONTENT_TYPE_TEST*/

