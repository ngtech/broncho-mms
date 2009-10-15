/*
 * File:    mms_notify.c
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

#include "mms_header.h"
#include "mms_notify.h"
#include "mms_header_codec.h"

struct _MmsNotify
{
	MmsHeader* mms_header;
	MmsHeader* retrieve_header;

	void*  buffer;
	size_t length;
};

static MmsResult   mms_notify_unpack(MmsNotify* thiz, MmsInputBuffer* input);

MmsNotify*  mms_notify_create(void* buffer, size_t length, int callee_release)
{
	MmsNotify* thiz = NULL;
	
	mms_return_val_if_fail(buffer != NULL && length > 0, NULL);

	thiz = (MmsNotify*)mms_calloc(1, sizeof(MmsNotify));

	if(thiz != NULL)
	{
		MmsInputBuffer* input = mms_input_buffer_create(buffer, length);

		thiz->mms_header = mms_header_create(mms_header_unpack_item, mms_header_pack_item);
    	mms_header_set_unpack_end_at_content_type(thiz->mms_header, 1);

		mms_notify_unpack(thiz, input);

		mms_input_buffer_destroy(input);

		if(callee_release)
		{
			thiz->buffer = buffer;
			thiz->length = length;
		}
	}

	return thiz;
}

void        mms_notify_destroy(MmsNotify* thiz)
{
	if(thiz != NULL)
	{
		mms_header_destroy(thiz->mms_header);
		thiz->mms_header = NULL;

		mms_header_destroy(thiz->retrieve_header);
		thiz->retrieve_header = NULL;

		if(thiz->buffer != NULL)
		{
			mms_free(thiz->buffer);
			thiz->buffer = NULL;
		}

		mms_free(thiz);
	}

	return;
}

static MmsResult   mms_notify_unpack(MmsNotify* thiz, MmsInputBuffer* input)
{
	mms_return_val_if_fail(thiz != NULL && input != NULL, MMS_RESULT_FAIL);

	mms_header_unpack(thiz->mms_header, input);

	return MMS_RESULT_OK;
}

const char* mms_notify_get_url(MmsNotify* thiz, int* charset)
{
	const char* key = mms_header_id_to_name(MMS_HEADER_X_MMS_CONTENT_LOCATION);

	mms_return_val_if_fail(thiz != NULL, NULL);

	if(mms_header_get_item_type(thiz->mms_header, key) == MMS_TYPE_STRING)
	{
		return mms_header_get_string(thiz->mms_header, key);
	}
	else
	{
		return mms_header_get_encoded_string(thiz->mms_header, key, charset);
	}
}

const char* mms_notify_get_transaction_id(MmsNotify* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);

	return mms_header_get_string(thiz->mms_header, mms_header_id_to_name(MMS_HEADER_X_MMS_TRANSACTION_ID));
}

const char* mms_notify_get_from(MmsNotify* thiz, int* charset)
{
	const char* key = mms_header_id_to_name(MMS_HEADER_FROM);

	mms_return_val_if_fail(thiz != NULL, NULL);

	if(mms_header_get_item_type(thiz->mms_header, key) == MMS_TYPE_STRING)
	{
		return mms_header_get_string(thiz->mms_header, key);
	}
	else
	{
		return mms_header_get_encoded_string(thiz->mms_header, key, charset);
	}
}

size_t      mms_notify_get_message_size(MmsNotify* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);
	
	return mms_header_get_long(thiz->mms_header, mms_header_id_to_name(MMS_HEADER_X_MMS_MESSAGE_SIZE));
}

time_t      mms_notify_get_date(MmsNotify* thiz)
{
	time_t date = 0;

	mms_return_val_if_fail(thiz != NULL, 0);
	
	if((date = mms_header_get_long(thiz->mms_header, "Date")) == 0)
	{
		date = time(NULL);
	}
	
	return date;
}

time_t      mms_notify_get_expiry(MmsNotify* thiz)
{
	int is_relative = 0;
	time_t expiry = 0;
	
	mms_return_val_if_fail(thiz != NULL, 0);
	
	if((expiry = mms_header_get_time(thiz->mms_header, "X-Mms-Expiry", &is_relative)) > 0)
	{
		if(is_relative)
		{
			expiry += mms_notify_get_date(thiz);
		}
	}
	else
	{
		expiry = time(0) + 30 * 24 * 3600;
	}

	return expiry;
}

const char* mms_notify_get_subject(MmsNotify* thiz, int* charset)
{
	const char* key = "Subject";
	mms_return_val_if_fail(thiz != NULL, NULL);

	if(mms_header_get_item_type(thiz->mms_header, key) == MMS_TYPE_STRING)
	{
		return mms_header_get_string(thiz->mms_header, key);
	}
	else
	{
		return mms_header_get_encoded_string(thiz->mms_header, key, charset);
	}
}

MmsResult   mms_notify_dump(MmsNotify* thiz, void* unused)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	
	mms_header_dump(thiz->mms_header, NULL);
	
	return MMS_RESULT_OK;
}

#ifdef MMS_NOTIFY_TEST
#include "mms_util.h"
void dump_time(const char* prefix, time_t t)
{
	struct tm* tm_ptr = localtime(&t);
	
	MMS_PRINTF("%s: %d-%d-%d %d:%d\n", prefix, tm_ptr->tm_year - 100, tm_ptr->tm_mon + 1, 
		tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min);

	return;
}

int main(int argc, char* argv[])
{
	char* buffer = NULL;
	size_t length = 0;
	MmsNotify* thiz = NULL;
	
	if(argc != 2)
	{
		MMS_PRINTF("%s notify\n", argv[0]);
		return 0;
	}
	
	buffer = mms_read_binary_file(argv[1], &length);
	if(buffer != NULL)
	{
		thiz = mms_notify_create(buffer, length, 1);

		mms_notify_dump(thiz, NULL);

		MMS_PRINTF("mms_notify_get_url: %s\n",            mms_notify_get_url(thiz, NULL));
		MMS_PRINTF("mms_notify_get_message_size:%d\n",    mms_notify_get_message_size(thiz));
		MMS_PRINTF("mms_notify_get_transaction_id: %s\n", mms_notify_get_transaction_id(thiz));
		MMS_PRINTF("mms_notify_get_from: %s\n",           mms_notify_get_from(thiz, NULL));
		dump_time("mms_notify_get_date",   mms_notify_get_date(thiz));
		dump_time("mms_notify_get_expiry", mms_notify_get_expiry(thiz));
		MMS_PRINTF("mms_notify_get_subject:%s\n",         mms_notify_get_subject(thiz, NULL));

		mms_notify_destroy(thiz);
	}

	return 0;
}
#endif/*MMS_NOTIFY_TEST*/

