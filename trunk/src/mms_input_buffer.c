/*
 * File:    mms_input_buffer.c
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

#include "mms_input_buffer.h"

#define MAX_LIMITS 8

struct _MmsInputBuffer
{
	unsigned char* buffer;
	size_t offset;
	size_t limit;
	size_t length;

	size_t limits[MAX_LIMITS + 1];
	size_t limit_nr;
};

MmsInputBuffer* mms_input_buffer_create(const char* buffer, size_t length)
{
	MmsInputBuffer* thiz = NULL;

	mms_return_val_if_fail(buffer != NULL && length != 0, NULL);

	if((thiz = (MmsInputBuffer*)mms_calloc(1, sizeof(MmsInputBuffer))) != NULL)
	{
		thiz->buffer = (unsigned char*)buffer;
		thiz->length = length;
		thiz->limit  = length;
	}
	
	return thiz;
}

void            mms_input_buffer_destroy(MmsInputBuffer* thiz)
{
	if(thiz != NULL)
	{
		mms_free(thiz);
	}

	return;
}

MmsResult       mms_input_buffer_push_limit(MmsInputBuffer* thiz, size_t limit)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(limit <= thiz->length, MMS_RESULT_FAIL);
	mms_return_val_if_fail(thiz->limit_nr < MAX_LIMITS, MMS_RESULT_FAIL);

	thiz->limit = limit;

	thiz->limits[thiz->limit_nr++] = limit;

	return MMS_RESULT_OK;
}

MmsResult       mms_input_buffer_pop_limit(MmsInputBuffer* thiz)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(thiz->limit_nr > 0, MMS_RESULT_FAIL);

	thiz->limit_nr--;
	if(thiz->limit_nr > 0)
	{
		thiz->limit = thiz->limits[thiz->limit_nr - 1];
		thiz->limits[thiz->limit_nr] = 0;
	}
	else
	{
		thiz->limit = thiz->length;
	}

	return MMS_RESULT_OK;
}

MmsResult       mms_input_buffer_skip_to_limit(MmsInputBuffer* thiz)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	thiz->offset = thiz->limit;

	return MMS_RESULT_OK;
}

unsigned char   mms_input_buffer_peek_int8(MmsInputBuffer* thiz)
{
	mms_return_val_if_fail(!mms_input_buffer_is_eof(thiz), 0);

	return thiz->buffer[thiz->offset];
}

unsigned char   mms_input_buffer_get_int8(MmsInputBuffer* thiz)
{
	mms_return_val_if_fail(!mms_input_buffer_is_eof(thiz), 0);

	return thiz->buffer[thiz->offset++];
}

unsigned short  mms_input_buffer_get_int16(MmsInputBuffer* thiz)
{
	unsigned char high = mms_input_buffer_get_int8(thiz);
	unsigned char low = mms_input_buffer_get_int8(thiz);

	return high << 8 | low;
}

unsigned int    mms_input_buffer_get_int32(MmsInputBuffer* thiz)
{
	unsigned short high = mms_input_buffer_get_int16(thiz);
	unsigned short low  = mms_input_buffer_get_int16(thiz);

	return high << 16 | low;
}

char*     mms_input_buffer_get_string(MmsInputBuffer* thiz)
{
	char* str = NULL;
	mms_return_val_if_fail(!mms_input_buffer_is_eof(thiz), NULL);

	str = (char*)(thiz->buffer + thiz->offset);
	thiz->offset += strlen(str) + 1;

	return str;
}

int             mms_input_buffer_get_charset(MmsInputBuffer* thiz)
{
	unsigned char val = mms_input_buffer_peek_int8(thiz);

	if(val < 32)
	{
		long length = mms_input_buffer_get_value_length(thiz);
		return mms_input_buffer_get_multi_bytes_int(thiz, length);
	}
	else if(val >= 128)
	{
		return mms_input_buffer_get_int8(thiz) - 128;
	}
	else
	{
		/*FIXME:*/
		char* str = mms_input_buffer_get_string(thiz);
	
		return 0;
	}
}

char*           mms_input_buffer_get_encoded_string(MmsInputBuffer* thiz, int* ret_charset)
{
	char* str = NULL;
	int  charset = 0;
	
	mms_return_val_if_fail(thiz != NULL, NULL);

	if(mms_input_buffer_is_value_length(thiz))
	{
		unsigned char val = 0;
		long value_length = 0;
		
		value_length = mms_input_buffer_get_value_length(thiz);
		mms_input_buffer_push_limit(thiz, thiz->offset + value_length);

		charset = mms_input_buffer_get_charset(thiz); 
		str = mms_input_buffer_get_string(thiz);

		mms_input_buffer_skip_to_limit(thiz);
		mms_input_buffer_pop_limit(thiz);
	}
	else
	{
		str = mms_input_buffer_get_string(thiz);
	}

	if(ret_charset != NULL)
	{
		*ret_charset = charset;
	}
	
	return str;
}

char*           mms_input_buffer_get_string_end_with(MmsInputBuffer* thiz, unsigned char ch)
{
	size_t i = 0;
	char* str = NULL;
	mms_return_val_if_fail(!mms_input_buffer_is_eof(thiz), NULL);

	str = (char*)(thiz->buffer + thiz->offset);

	for(i = thiz->offset; i < thiz->limit; i++)
	{
		if(thiz->buffer[i] == ch)
		{
			thiz->buffer[i] = '\0';
			break;
		}
	}
	thiz->offset += strlen(str) + 1;

	return str;
}

char*     mms_input_buffer_get_data(MmsInputBuffer* thiz, size_t length)
{
	char* data = NULL;
	mms_return_val_if_fail(!mms_input_buffer_is_eof(thiz), NULL);
	mms_return_val_if_fail((thiz->offset + length) <= thiz->limit, NULL);
	
	data = (char*)(thiz->buffer + thiz->offset);
	thiz->offset += length;

	return data;
}

long   mms_input_buffer_get_var_int(MmsInputBuffer* thiz)
{
	unsigned char octet  = 0;
	unsigned long result = 0;

	do
	{
		octet = mms_input_buffer_get_int8(thiz);

		result <<= 7;
		result |= (octet & 0x7f);
	}while((octet & 0x80) != 0);

	return result;
}

long            mms_input_buffer_get_int(MmsInputBuffer* thiz)
{
	if(mms_input_buffer_is_short_int(thiz))
	{
		return mms_input_buffer_get_short_int(thiz);
	}
	else
	{
		return mms_input_buffer_get_long_int(thiz);
	}
}

long            mms_input_buffer_get_multi_bytes_int(MmsInputBuffer* thiz, int length)
{
	long value = 0;
	
	mms_return_val_if_fail(thiz != NULL && length >= 0 && length <= sizeof(long), 0);

	while(length > 0)
	{
		unsigned char ch = mms_input_buffer_get_int8(thiz);
		value = (value << 8) + ch;
		length--;
	}

	return value;
}

short  mms_input_buffer_get_short_int(MmsInputBuffer* thiz)
{
	unsigned char ch = mms_input_buffer_get_int8(thiz);

	return ch & 0x7f;
}

long   mms_input_buffer_get_long_int(MmsInputBuffer* thiz)
{
	long ret = 0;
	long len = mms_input_buffer_get_int8(thiz);

	while(len > 0)
	{
		len--;

		ret |= mms_input_buffer_get_int8(thiz) << (len*8);
	}

	return ret;
}

long   mms_input_buffer_get_value_length(MmsInputBuffer* thiz)
{
	long len = mms_input_buffer_get_int8(thiz);

	if(len == 31)
	{
		len = mms_input_buffer_get_var_int(thiz);
	}

	return len;
}

int  mms_input_buffer_get_remaining_length(MmsInputBuffer* thiz)
{
	mms_return_val_if_fail(!mms_input_buffer_is_eof(thiz), 0);

	return thiz->limit - thiz->offset;
}

const   char*   mms_input_buffer_get_remaining(MmsInputBuffer* thiz)
{
	mms_return_val_if_fail(!mms_input_buffer_is_eof(thiz), NULL);

	return (char*)(thiz->buffer + thiz->offset);
}

int             mms_input_buffer_is_string(MmsInputBuffer* thiz)
{
	unsigned char c = mms_input_buffer_peek_int8(thiz);

	return ( c >= 0x20 && c < 0x80);
}

int             mms_input_buffer_is_short_int(MmsInputBuffer* thiz)
{
	unsigned char c = mms_input_buffer_peek_int8(thiz);
	
	return (c >= 0x80);
}

int             mms_input_buffer_is_value_length(MmsInputBuffer* thiz)
{
	unsigned char c = mms_input_buffer_peek_int8(thiz);

	return c <= 31;
}

size_t          mms_input_buffer_tell(MmsInputBuffer* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);

	return thiz->offset;
}

MmsResult       mms_input_buffer_seek(MmsInputBuffer* thiz, int offset)
{
	int new_offset = 0;
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	new_offset = thiz->offset + offset;
	mms_return_val_if_fail(new_offset>= 0 && new_offset <= (int)thiz->limit, MMS_RESULT_FAIL);

	thiz->offset = new_offset;

	return MMS_RESULT_OK;
}

int             mms_input_buffer_is_eof(MmsInputBuffer* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);
	mms_return_val_if_fail(thiz->buffer != NULL, 0);
	
	return (thiz->offset >= thiz->limit);
}

#ifdef MMS_INPUT_BUFFER_TEST

static void test_get(void)
{
	long value = 0;
	int pos = 0;
	const char str[] = "str\x0\x7f\x10\x20\x10\x20\x30\x40\x81\x82\x83\x4\x8f\x2\x10\x20\x10";
	MmsInputBuffer* thiz = mms_input_buffer_create(str, sizeof(str));

	mms_assert(mms_input_buffer_is_string(thiz));
	mms_assert(!mms_input_buffer_is_short_int(thiz));
	mms_assert(!mms_input_buffer_is_eof(thiz));

	mms_assert(mms_input_buffer_tell(thiz) == 0);
	mms_input_buffer_seek(thiz, 0);
	mms_input_buffer_seek(thiz, 1);
	mms_input_buffer_seek(thiz, -1);

	mms_assert(strcmp(mms_input_buffer_get_string(thiz), str) == 0);
	mms_assert(mms_input_buffer_get_int8(thiz) == 0x7f);
	mms_assert(mms_input_buffer_get_int16(thiz) == 0x1020);
	mms_assert(mms_input_buffer_get_int32(thiz) == 0x10203040);
	value = (((((1 << 7) | 2) << 7) | 3) << 7) | 4;
	mms_assert(mms_input_buffer_get_var_int(thiz) == value);
	mms_assert(mms_input_buffer_get_short_int(thiz) == 0x0f);
	mms_assert(mms_input_buffer_get_long_int(thiz) == 0x1020);
	mms_assert(mms_input_buffer_get_value_length(thiz) == 0x10);

	mms_assert(mms_input_buffer_get_int8(thiz) == 0x00);
	mms_assert(mms_input_buffer_is_eof(thiz));
	mms_input_buffer_destroy(thiz);

	return;
}

static void test_push_pop(void)
{
	int i = 0;
	const char str[1024] = "str\x0\x7f\x10\x20\x10\x20\x30\x40\x81\x82\x83\x4\x8f\x2\x10\x20\x10";
	MmsInputBuffer* thiz = mms_input_buffer_create(str, sizeof(str));

	for(i = 0; i < MAX_LIMITS; i++)
	{
		mms_assert(mms_input_buffer_push_limit(thiz, i + 1) == MMS_RESULT_OK);
		mms_assert(thiz->limit == i + 1);
		MMS_PRINTF("limit_nr=%d\n", thiz->limit_nr);
	}
	
	for(i = 0; i < MAX_LIMITS; i++)
	{
		mms_assert(thiz->limit == (MAX_LIMITS - i));
		MMS_PRINTF("limit_nr=%d\n", thiz->limit_nr);
		mms_assert(mms_input_buffer_pop_limit(thiz) == MMS_RESULT_OK);
	}
	
	mms_input_buffer_destroy(thiz);

	return;
}

int main(int argc, char* argv[])
{
	test_get();
	test_push_pop();

	return 0;
}
#endif/*MMS_INPUT_BUFFER_TEST*/

