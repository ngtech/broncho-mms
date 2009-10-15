/*
 * File:    mms_output_buffer.c
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

#include "mms_charset.h"
#include "mms_output_buffer.h"

#define MAX_TRANSACTIONS 8

struct _MmsOutputBuffer
{
	unsigned char*  buffer;
	size_t data_length;
	size_t length;

	size_t transaction_begin;
	size_t transactions[MAX_TRANSACTIONS + 1];
	size_t transaction_nr;
};

#define DEFAULT_OUTPUT_BUFFER_LENGTH 512

MmsOutputBuffer* mms_output_buffer_create(size_t init_length)
{
	MmsOutputBuffer* thiz = (MmsOutputBuffer*)mms_calloc(1, sizeof(MmsOutputBuffer));

	if(thiz != NULL)
	{
		thiz->length = init_length != 0 ? init_length : DEFAULT_OUTPUT_BUFFER_LENGTH;
		
		thiz->buffer = (unsigned char*)mms_malloc(thiz->length);
		thiz->buffer[0] = '\0';
	}

	return thiz;
}

void             mms_output_buffer_destroy(MmsOutputBuffer* thiz)
{
	if(thiz != NULL)
	{
		if(thiz->buffer != NULL)
		{
			mms_free(thiz->buffer);
		}

		thiz->buffer      = NULL;
		thiz->length      = 0;
		thiz->data_length = 0;

		mms_free(thiz);
	}

	return;
}

MmsResult        mms_output_buffer_clear(MmsOutputBuffer* thiz)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	
	thiz->data_length = 0;

	return MMS_RESULT_OK;
}

MmsResult        mms_output_buffer_reset(MmsOutputBuffer* thiz)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	thiz->data_length = 0;

	return MMS_RESULT_OK;
}

static MmsResult mms_output_buffer_extend(MmsOutputBuffer* thiz, size_t min_size)
{
	MmsResult ret = MMS_RESULT_FAIL;
	
	if((thiz->data_length + min_size) < thiz->length)
	{
		ret = MMS_RESULT_OK;
	}
	else
	{
		size_t new_length = thiz->length * 2 + min_size;
		thiz->buffer = (unsigned char*)mms_realloc(thiz->buffer, new_length);
		if(thiz->buffer != NULL)
		{
			thiz->length = new_length;
			ret = MMS_RESULT_OK;
			thiz->buffer[thiz->data_length] = '\0';
		}
	}

	return ret;
}

MmsResult        mms_output_buffer_put_char(MmsOutputBuffer* thiz, unsigned char ch)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(mms_output_buffer_extend(thiz, sizeof(ch)) == MMS_RESULT_OK, MMS_RESULT_FAIL);

	thiz->buffer[thiz->data_length++] = ch;

	return MMS_RESULT_OK;
}

MmsResult        mms_output_buffer_put_string(MmsOutputBuffer* thiz, const char* str)
{
	mms_return_val_if_fail(thiz != NULL && str != NULL, MMS_RESULT_FAIL);

	return mms_output_buffer_put_data(thiz, str, strlen(str) + 1);
}

static size_t      mms_get_int_bytes(unsigned int n)
{
	if(n > 0xfffff)
	{
		return 4;
	}
	else if(n > 0xffff)
	{
		return 3;
	}
	else if(n > 0xff)
	{
		return 2;
	}
	else
	{
		return 1;
	}
}
static size_t mms_output_buffer_get_charset_bytes(int charset)
{
	size_t bytes = charset <= 128 ? 1 : mms_get_int_bytes(charset) + 1;

	return bytes;
}

MmsResult        mms_output_buffer_put_multi_bytes_int(MmsOutputBuffer* thiz, unsigned int value)
{
	int i     = 0;
	int n     = mms_get_int_bytes(value) - 1;

	for(i = n; i >= 0; i--)
	{
		unsigned char ch = (value >> (8 * i)) & 0xff;
		mms_output_buffer_put_int8(thiz, ch);
	}
	
	return MMS_RESULT_OK;
}

MmsResult        mms_output_buffer_put_encoded_string(MmsOutputBuffer* thiz, const char* data, int charset)
{
	mms_return_val_if_fail(thiz != NULL && data != NULL, MMS_RESULT_FAIL);
	
	if(charset > 0)
	{
		size_t charset_bytes = mms_output_buffer_get_charset_bytes(charset);
		size_t value_length = charset_bytes + strlen(data) + 1;
		
		mms_output_buffer_put_value_length(thiz, value_length);
		if(charset < 128)
		{
			mms_output_buffer_put_int8(thiz, (unsigned char)(charset + 128));
		}
		else
		{
			mms_output_buffer_put_value_length(thiz, mms_get_int_bytes(charset));
			mms_output_buffer_put_multi_bytes_int(thiz, charset);
		}
	}
		
	return mms_output_buffer_put_string(thiz, data);
}

MmsResult        mms_output_buffer_put_string_without_null(MmsOutputBuffer* thiz, const char* str)
{
	mms_return_val_if_fail(thiz != NULL && str != NULL, MMS_RESULT_FAIL);

	return mms_output_buffer_put_data(thiz, str, strlen(str));
}

MmsResult        mms_output_buffer_put_data(MmsOutputBuffer* thiz, const char* data, size_t data_length)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(data != NULL && data_length != 0, MMS_RESULT_FAIL);
	mms_return_val_if_fail(mms_output_buffer_extend(thiz, data_length) == MMS_RESULT_OK, MMS_RESULT_FAIL);

	memcpy(thiz->buffer + thiz->data_length, data, data_length);

	thiz->data_length += data_length;
	thiz->buffer[thiz->data_length] = '\0';

	return MMS_RESULT_OK;
}

MmsResult        mms_output_buffer_put_int8(MmsOutputBuffer* thiz, unsigned char data)
{
	return mms_output_buffer_put_char(thiz, data);
}

MmsResult        mms_output_buffer_put_int16(MmsOutputBuffer* thiz, unsigned short data)
{
	mms_output_buffer_put_int8(thiz, (unsigned char)((data >> 8) & 0xff));

	return mms_output_buffer_put_int8(thiz,  (unsigned char)(data & 0xff));
}

MmsResult        mms_output_buffer_put_int32(MmsOutputBuffer* thiz, unsigned int data)
{
	mms_output_buffer_put_int16(thiz, (unsigned short)((data >> 16) & 0xffff));

	return mms_output_buffer_put_int16(thiz, (unsigned short)(data & 0xffff));
}

MmsResult        mms_output_buffer_put_var_int(MmsOutputBuffer* thiz, unsigned long var)
{
	int i;
	unsigned char bits[8] = {0};
	
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	
	if(var==0)
	{
		mms_output_buffer_put_int8(thiz, 0);
	}   
	else
	{
		for(i=0; i<8; i++) 
		{
			bits[i] = (unsigned char)(var & 0x7f);
			var >>= 7;
		}
		i=7;
		while ( bits[i] == 0 ) { i--; } // skip over empty values
		// now start coding:
		while (i > 0) 
		{
			mms_output_buffer_put_int8(thiz, (unsigned char)(0x80 | bits[i]));
			i--;
		}
		mms_output_buffer_put_int8(thiz, (unsigned char)(bits[0] & 0x7f));
	}

	return MMS_RESULT_OK;
}

MmsResult        mms_output_buffer_put_int(MmsOutputBuffer* thiz, unsigned long data)
{
	if(data <= 0x7f)
	{
		return mms_output_buffer_put_short_int(thiz, data);
	}
	else
	{
		return mms_output_buffer_put_long_int(thiz, data);
	}
}

MmsResult        mms_output_buffer_put_short_int(MmsOutputBuffer* thiz, unsigned long data)
{
	return mms_output_buffer_put_int8(thiz,  (unsigned char)(data | 0x80));
}

MmsResult        mms_output_buffer_put_long_int(MmsOutputBuffer* thiz, unsigned long data)
{
	unsigned char len = 0;
	unsigned char bits[32] = {0};
	
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	
	if (data == 0) 
	{
		mms_output_buffer_put_int8(thiz, 0x01);
		mms_output_buffer_put_int8(thiz, 0x00);
	 }
	else
	{
		for(len = 0; data != 0; data >>= 8, len++) 
		{
			bits[len] = (unsigned char)(data & 0xff);
		}

		mms_output_buffer_put_int8(thiz, len);
		
		for(; len > 0; len--)
		{
			mms_output_buffer_put_int8(thiz, bits[len - 1]);
		}
	}

	return MMS_RESULT_OK;
}

MmsResult        mms_output_buffer_put_value_length(MmsOutputBuffer* thiz, unsigned long data)
{
    if (data <= 30)
	{
		mms_output_buffer_put_int8(thiz, (unsigned char)(data & 0xff));
	}
	else 
	{
		mms_output_buffer_put_int8(thiz, 31);
		mms_output_buffer_put_var_int(thiz, data);
    }

    return MMS_RESULT_OK;
}

size_t           mms_output_buffer_get_buffer_length(MmsOutputBuffer* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);
	
	return thiz->data_length;
}

char*      mms_output_buffer_get_buffer(MmsOutputBuffer* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);

	return (char*)thiz->buffer;
}

char*            mms_output_buffer_detach_buffer(MmsOutputBuffer* thiz)
{
	char* buffer = NULL;
	
	mms_return_val_if_fail(thiz != NULL, NULL);
	
	buffer = (char*)thiz->buffer;

	thiz->buffer      = NULL;
	thiz->data_length = 0;
	thiz->length      = 0;

	return buffer;
}

unsigned long    mms_output_buffer_get_long_int_bytes(MmsOutputBuffer* thiz, unsigned long data)
{
	unsigned long ret = 0;
	unsigned long old_data_length = 0;
	mms_return_val_if_fail(thiz != NULL, 0);

	old_data_length = thiz->data_length;
	mms_output_buffer_put_long_int(thiz, data);
	
	ret = thiz->data_length - old_data_length;
	thiz->data_length = old_data_length;
	
	return ret;
}

unsigned long    mms_output_buffer_get_encoded_string_bytes(MmsOutputBuffer* thiz, const char* data, int charset)
{
	size_t old_data_length = 0;
	size_t length = 0;
	mms_return_val_if_fail(thiz != NULL && data != NULL, 0);

	old_data_length = thiz->data_length;

	mms_output_buffer_transaction_begin(thiz);
	mms_output_buffer_put_encoded_string(thiz, data, charset);
	length = mms_output_buffer_transaction_data_length(thiz);
	mms_output_buffer_transaction_rollback(thiz);

	mms_assert(old_data_length == thiz->data_length);

	return length;
}

MmsResult        mms_output_buffer_transaction_begin(MmsOutputBuffer* thiz)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(thiz->transaction_nr < MAX_TRANSACTIONS, MMS_RESULT_FAIL);

	thiz->transaction_begin = thiz->data_length;
	
	thiz->transactions[thiz->transaction_nr++] = thiz->transaction_begin;
	
	return MMS_RESULT_OK;
}

static MmsResult        mms_output_buffer_transaction_end(MmsOutputBuffer* thiz, int rollback)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(thiz->transaction_nr > 0, MMS_RESULT_FAIL);

	if(rollback)
	{
		thiz->data_length = thiz->transaction_begin;
	}

	thiz->transaction_nr--;
	if(thiz->transaction_nr > 0)
	{
		thiz->transactions[thiz->transaction_nr] = 0;
		thiz->transaction_begin = thiz->transactions[thiz->transaction_nr - 1];
	}
	else
	{
		thiz->transaction_begin = 0;
	}

	return MMS_RESULT_OK;
}

MmsResult        mms_output_buffer_transaction_rollback(MmsOutputBuffer* thiz)
{
	return mms_output_buffer_transaction_end(thiz, 1);
}

MmsResult        mms_output_buffer_transaction_commit(MmsOutputBuffer* thiz)
{
	return mms_output_buffer_transaction_end(thiz, 0);
}

size_t mms_output_buffer_transaction_data_length(MmsOutputBuffer* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);
	
	return thiz->data_length - thiz->transaction_begin;
}


#ifdef MMS_OUTPUT_BUFFER_TEST

static void normal_test(void)
{
	int i = 0;
	int n = 10;
	char* str = "str";
	char  c = 0x80;
	size_t length = 0;
	const char* buffer = NULL;
	
	MmsOutputBuffer* thiz = mms_output_buffer_create(0);
	
	for(i = 0; i < n; i++)
	{
		length = mms_output_buffer_get_buffer_length(thiz);
		buffer = mms_output_buffer_get_buffer(thiz) + length;	
		mms_output_buffer_put_char(thiz, c);
		
		length++;
		mms_assert(buffer[0] == c);
		mms_assert(mms_output_buffer_get_buffer_length(thiz) == length);
	}

	for(i = 0; i < n; i++)
	{
		length = mms_output_buffer_get_buffer_length(thiz);
		buffer = mms_output_buffer_get_buffer(thiz) + length;	
		mms_output_buffer_put_string(thiz, str);
		length += strlen(str) + 1;

		mms_assert(strcmp(buffer, str) == 0);
		mms_assert(mms_output_buffer_get_buffer_length(thiz) == length);
	}
	
	for(i = 0; i < n; i++)
	{
		length = mms_output_buffer_get_buffer_length(thiz);
		buffer = mms_output_buffer_get_buffer(thiz) + length;	
		mms_output_buffer_put_data(thiz, str, strlen(str));
		length += strlen(str);

		mms_assert(memcmp(buffer, str, strlen(str)) == 0);
		mms_assert(mms_output_buffer_get_buffer_length(thiz) == length);
	}

	mms_output_buffer_destroy(thiz);

	mms_assert(mms_get_int_bytes(1) == 1);
	mms_assert(mms_get_int_bytes((1 << 8) + 1) == 2);
	mms_assert(mms_get_int_bytes((1 << 16) + 1) == 3);
	mms_assert(mms_get_int_bytes((1 << 24) + 1) == 4);

	return;
}

#include "mms_input_buffer.h"

static void test_transaction(void)
{
	const char* str = "1234";
	size_t length = strlen(str) + 1;
	size_t i = 0;
	
	MmsOutputBuffer* output = mms_output_buffer_create(256);

	mms_output_buffer_put_string(output, str);
	mms_assert(mms_output_buffer_get_buffer_length(output) == length);
	
	for(i = 0; i < MAX_TRANSACTIONS; i++)
	{
		mms_assert(mms_output_buffer_transaction_begin(output) == MMS_RESULT_OK);
		mms_output_buffer_put_string(output, str);
		mms_assert(mms_output_buffer_transaction_data_length(output) == length);
		MMS_PRINTF("transaction_nr=%d\n", output->transaction_nr);
	}	
	
	for(i = 0; i < MAX_TRANSACTIONS; i++)
	{
		MMS_PRINTF("transaction_nr=%d\n", output->transaction_nr);
		mms_assert(mms_output_buffer_transaction_data_length(output) == length);
		mms_assert(mms_output_buffer_transaction_rollback(output) == MMS_RESULT_OK);
	}

	mms_assert(mms_output_buffer_transaction_begin(output) == MMS_RESULT_OK);
	mms_output_buffer_put_string(output, str);
	mms_assert(mms_output_buffer_transaction_data_length(output) == length);
	mms_assert(mms_output_buffer_get_buffer_length(output) == 2 * length);
	mms_assert(mms_output_buffer_transaction_rollback(output) == MMS_RESULT_OK);
	mms_assert(mms_output_buffer_get_buffer_length(output) == length);

	mms_assert(mms_output_buffer_transaction_begin(output) == MMS_RESULT_OK);
	mms_output_buffer_put_string(output, str);
	mms_assert(mms_output_buffer_transaction_data_length(output) == length);
	mms_assert(mms_output_buffer_get_buffer_length(output) == 2 * length);
	mms_assert(mms_output_buffer_transaction_commit(output) == MMS_RESULT_OK);
	mms_assert(mms_output_buffer_get_buffer_length(output) == 2 * length);
	
	mms_output_buffer_destroy(output);

	return;
}
static void test_with_input(void)
{
	int charset = 0;
	MmsInputBuffer* input   = NULL;
	MmsOutputBuffer* output = mms_output_buffer_create(256);

	mms_output_buffer_put_int8(output, 0x01);
	mms_output_buffer_put_int16(output, 0x0102);
	mms_output_buffer_put_int32(output, 0x01020304);
	mms_output_buffer_put_int(output, 0x12);
	mms_output_buffer_put_int(output, 0x1234);
	mms_output_buffer_put_short_int(output, 0x12);
	mms_output_buffer_put_long_int(output, 0x12);
	mms_output_buffer_put_long_int(output, 0x11223344);

	mms_output_buffer_put_var_int(output, 0x12);
	mms_output_buffer_put_var_int(output, 0x1234);
	mms_output_buffer_put_var_int(output, 0x12345678);
	mms_output_buffer_put_value_length(output, 0x12);
	mms_output_buffer_put_value_length(output, 0x1234);
	mms_output_buffer_put_value_length(output, 0x12345678);

	mms_output_buffer_put_int8(output, 0);
	mms_output_buffer_put_int16(output, 0);
	mms_output_buffer_put_int32(output, 0);
	mms_output_buffer_put_int(output, 0);
	mms_output_buffer_put_short_int(output, 0);
	mms_output_buffer_put_long_int(output, 0);
	mms_output_buffer_put_var_int(output, 0);
	mms_output_buffer_put_value_length(output, 0);
	mms_assert(mms_output_buffer_put_encoded_string(output, "1234", CHARSET_UTF_8) == MMS_RESULT_OK);
	mms_assert(mms_output_buffer_put_encoded_string(output, "1234", CHARSET_GB2312) == MMS_RESULT_OK);

	input = mms_input_buffer_create(mms_output_buffer_get_buffer(output), mms_output_buffer_get_buffer_length(output));
	
	mms_assert(mms_input_buffer_get_int8(input) == 0x01);
	mms_assert(mms_input_buffer_get_int16(input) == 0x0102);
	mms_assert(mms_input_buffer_get_int32(input) == 0x01020304);
	mms_assert(mms_input_buffer_get_int(input) == 0x12);
	mms_assert(mms_input_buffer_get_int(input) == 0x1234);
	mms_assert(mms_input_buffer_get_short_int(input) == 0x12);
	mms_assert(mms_input_buffer_get_long_int(input) == 0x12);
	mms_assert(mms_input_buffer_get_long_int(input) == 0x11223344);
	
	mms_assert(mms_input_buffer_get_var_int(input) == 0x12);
	mms_assert(mms_input_buffer_get_var_int(input) == 0x1234);
	mms_assert(mms_input_buffer_get_var_int(input) == 0x12345678);
	mms_assert(mms_input_buffer_get_value_length(input) == 0x12);
	mms_assert(mms_input_buffer_get_value_length(input) == 0x1234);
	mms_assert(mms_input_buffer_get_value_length(input) == 0x12345678);

	mms_assert(mms_input_buffer_get_int8(input) == 0);
	mms_assert(mms_input_buffer_get_int16(input) == 0);
	mms_assert(mms_input_buffer_get_int32(input) == 0);
	mms_assert(mms_input_buffer_get_int(input) == 0);
	mms_assert(mms_input_buffer_get_short_int(input) == 0);
	mms_assert(mms_input_buffer_get_long_int(input) == 0);
	mms_assert(mms_input_buffer_get_var_int(input) == 0);
	mms_assert(mms_input_buffer_get_value_length(input) == 0);
	mms_assert(strcmp(mms_input_buffer_get_encoded_string(input, &charset), "1234") == 0);
	mms_assert(charset == CHARSET_UTF_8);
	mms_assert(strcmp(mms_input_buffer_get_encoded_string(input, &charset), "1234") == 0);
	mms_assert(charset == CHARSET_GB2312);

	mms_assert(mms_input_buffer_is_eof(input));

	mms_assert(mms_output_buffer_get_long_int_bytes(output, 0x01) == 2);
	mms_assert(mms_output_buffer_get_long_int_bytes(output, 0x0102) == 3);
	mms_assert(mms_output_buffer_get_long_int_bytes(output, 0x010203) == 4);
	mms_assert(mms_output_buffer_get_long_int_bytes(output, 0x01020304) == 5);

	mms_input_buffer_destroy(input);
	mms_output_buffer_destroy(output);

	return;
}

int main(int argc, char* argv[])
{
	normal_test();

	test_with_input();

	test_transaction();

	return;
}

#endif/*MMS_OUTPUT_BUFFER_TEST*/

