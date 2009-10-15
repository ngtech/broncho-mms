/*
 * File:    mms_header_codec.c
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

#include "mms_type.h"
#include "mms_util.h"
#include "mms_content_type.h"
#include "mms_input_buffer.h"
#include "mms_output_buffer.h"
#include "mms_header_codec.h"

#undef MMS_HEADER_ITEM_DEF
#define MMS_HEADER_ITEM_DEF(value, name, id) {name, id},
static const NameIdPair g_mms_headers[] =
{
	MMS_HEADERS
	{NULL, -1}
};
#undef MMS_HEADER_ITEM_DEF


int              mms_header_name_to_id(const char* name)
{
	return mms_name_to_id(g_mms_headers, name);	
}

const char*      mms_header_id_to_name(int id)
{
	return mms_id_to_name(g_mms_headers, id);
}

#undef MMS_CODEC_DEF
#define MMS_CODEC_DEF(name, id) {name, id},

#define VALS_OF_DELIVERY_REPORT\
   MMS_CODEC_DEF("Yes", 128) \
   MMS_CODEC_DEF("No", 129)

static const NameIdPair g_delivery_report_values[] = 
{
	VALS_OF_DELIVERY_REPORT
	{NULL, -1}
};

#define VALS_OF_DELIVERY_TIME_TOKEN \
   MMS_CODEC_DEF("Absolute-token", 128) \
   MMS_CODEC_DEF("Relative-token", 129) 
static const NameIdPair g_delivery_time_token_values[] =
{
	VALS_OF_DELIVERY_TIME_TOKEN
	{NULL, -1}
};

#define VALS_OF_EXPIRY_TIME_TOKEN \
   MMS_CODEC_DEF("Absolute-token", 128) \
   MMS_CODEC_DEF("Relative-token", 129) 
static const NameIdPair g_expiry_time_token_values[] = 
{
	VALS_OF_EXPIRY_TIME_TOKEN
	{NULL, -1}
};

#define VALS_OF_FROM_ADDR_TYPE \
   MMS_CODEC_DEF("Address-present-token", 128)\
   MMS_CODEC_DEF("Insert-address-token", 129) 
static const NameIdPair g_from_addr_type_values[] =
{
	VALS_OF_FROM_ADDR_TYPE
	{NULL, -1}
};

#define VALS_OF_MESSAGE_CLASS \
   MMS_CODEC_DEF("Personal", 128) \
   MMS_CODEC_DEF("Advertisement", 129) \
   MMS_CODEC_DEF("Informational", 130) 
static const NameIdPair g_message_class_values[] = 
{
	VALS_OF_MESSAGE_CLASS
	{NULL, -1}
};

#define VALS_OF_MESSAGE_TYPE \
   MMS_CODEC_DEF("m-send-req", 128)\
   MMS_CODEC_DEF("m-send-conf", 129)\
   MMS_CODEC_DEF("m-notification-ind", 130)\
   MMS_CODEC_DEF("m-notifyresp-ind", 131)\
   MMS_CODEC_DEF("m-retrieve-conf", 132)\
   MMS_CODEC_DEF("m-acknowledge-ind", 133)\
   MMS_CODEC_DEF("m-delivery-ind", 134) 
static const NameIdPair g_message_type_values[] =
{
	VALS_OF_MESSAGE_TYPE
	{NULL, -1}
};

#define VALS_OF_PRIORITY \
   MMS_CODEC_DEF("Low", 128)\
   MMS_CODEC_DEF("Normal", 129)\
   MMS_CODEC_DEF("High", 130) 
static const NameIdPair g_priority_values[] =
{
	VALS_OF_PRIORITY
	{NULL, -1}
};

#define VALS_OF_READ_REPLY \
   MMS_CODEC_DEF("Yes", 128)\
   MMS_CODEC_DEF("No", 129) 
static const NameIdPair g_read_reply_values[] = 
{
	VALS_OF_READ_REPLY
	{NULL, -1}
};

#define VALS_OF_REPORT_ALLOWED \
   MMS_CODEC_DEF("Yes", 128)\
   MMS_CODEC_DEF("No", 129) 
static const NameIdPair g_report_allowed_values[] =
{
	VALS_OF_REPORT_ALLOWED
	{NULL, -1}
};

#define VALS_OF_RESPONSE_STATUS \
   MMS_CODEC_DEF("Ok", 128)\
   MMS_CODEC_DEF("Error-unspecified", 129)\
   MMS_CODEC_DEF("Error- service-denied", 130)\
   MMS_CODEC_DEF("Error-message-format-corrupt", 131)\
   MMS_CODEC_DEF("Error-sending-address-unresolved", 132)\
   MMS_CODEC_DEF("Error-message-not-found", 133)\
   MMS_CODEC_DEF("Error-network-problem", 134)\
   MMS_CODEC_DEF("Error- content-not-accepted", 135)\
   MMS_CODEC_DEF("Error-unsupported-message", 136) 
static const NameIdPair g_response_status_values[] =
{
	VALS_OF_RESPONSE_STATUS
	{NULL, -1}
};

#define VALS_OF_SENDER_VISIBILITY \
   MMS_CODEC_DEF("Hide", 128)\
   MMS_CODEC_DEF("Show", 129) 
static const NameIdPair g_sender_visibility_values[] =
{
	VALS_OF_SENDER_VISIBILITY
	{NULL, -1}
};

#define VALS_OF_STATUS \
   MMS_CODEC_DEF("Expired",   128)\
   MMS_CODEC_DEF("Retrieved", 129)\
   MMS_CODEC_DEF("Rejected",  130)\
   MMS_CODEC_DEF("Deferred",  131)\
   MMS_CODEC_DEF("Unrecognised", 132) 
static const NameIdPair g_status_values[] =
{
	VALS_OF_STATUS
	{NULL, -1}
};

static const NameIdPair* g_header_id_values[MMS_HEADER_NR] = {0};

static const NameIdPair* mms_header_get_name_id_pair(int id)
{
	static int name_id_initialized = 0;

	mms_return_val_if_fail(id >= 0 && id < MMS_HEADER_NR, NULL);

	if(!name_id_initialized)
	{
		g_header_id_values[MMS_HEADER_X_MMS_MESSAGE_TYPE]      = g_message_type_values;
		g_header_id_values[MMS_HEADER_X_MMS_DELIVERY_REPORT]   = g_delivery_report_values;
		g_header_id_values[MMS_HEADER_X_MMS_PRIORITY]          = g_priority_values;
		g_header_id_values[MMS_HEADER_X_MMS_READ_REPORT]       = g_read_reply_values;
		g_header_id_values[MMS_HEADER_X_MMS_REPORT_ALLOWED]    = g_report_allowed_values;
		g_header_id_values[MMS_HEADER_X_MMS_RESPONSE_STATUS]   = g_response_status_values;
		g_header_id_values[MMS_HEADER_X_MMS_SENDER_VISIBILITY] = g_sender_visibility_values;
		g_header_id_values[MMS_HEADER_X_MMS_STATUS]            = g_status_values;
		g_header_id_values[MMS_HEADER_X_MMS_MESSAGE_CLASS]     = g_message_class_values;
		name_id_initialized = !0;
	}

	return g_header_id_values[id];
}

/******************************************************/
HeaderItem*      mms_header_unpack_item(MmsInputBuffer* input)
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
	int           charset  = 0;

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
	name = mms_header_id_to_name(id);
	mms_return_val_if_fail(!mms_input_buffer_is_eof(input), NULL);

	switch(id)
	{
		/*content-type*/
		case MMS_HEADER_CONTENT_TYPE:
		{
			MmsContentType* content_type = mms_content_type_create();
			mms_content_type_unpack(content_type, input);
			item = mms_header_item_data_create(name, content_type, (mms_destroy_func)mms_content_type_destroy);

			break;
		}
		/*long int*/
		case MMS_HEADER_X_MMS_MESSAGE_SIZE:
		case MMS_HEADER_DATE:
		{
			long_int = mms_input_buffer_get_long_int(input);
			item = mms_header_item_long_create(name, long_int);
			break;
		}
		/*uchar*/
		case MMS_HEADER_X_MMS_MESSAGE_TYPE:
		case MMS_HEADER_X_MMS_DELIVERY_REPORT:
		case MMS_HEADER_X_MMS_PRIORITY:
		case MMS_HEADER_X_MMS_READ_REPORT:
		case MMS_HEADER_X_MMS_REPORT_ALLOWED:
		case MMS_HEADER_X_MMS_RESPONSE_STATUS:
		case MMS_HEADER_X_MMS_SENDER_VISIBILITY:
		case MMS_HEADER_X_MMS_STATUS:
		{
			uchar = mms_input_buffer_get_int8(input);
			item = mms_header_item_long_create(name, uchar);
			
			item->value_desc = (char*)mms_id_to_name(mms_header_get_name_id_pair(id), uchar);
			break;
		}
		/*string*/
		case MMS_HEADER_X_MMS_CONTENT_LOCATION:
		case MMS_HEADER_X_MMS_TRANSACTION_ID:
		{
			str = (char*)mms_input_buffer_get_string(input);
			item = mms_header_item_string_create(name, str);
			break;
		}
		/*encoded string*/
		case MMS_HEADER_BCC:
		case MMS_HEADER_CC:
		case MMS_HEADER_MESSAGE_ID:
		case MMS_HEADER_X_MMS_RESPONSE_TEXT:
		case MMS_HEADER_SUBJECT:
		case MMS_HEADER_TO:
		{
			if(mms_input_buffer_is_value_length(input))
			{
				str = mms_input_buffer_get_encoded_string(input, &charset);
				item = mms_header_item_encoded_string_create(name, str, charset);
			}
			else
			{
				str = (char*)mms_input_buffer_get_string(input);
				item = mms_header_item_string_create(name, str);
			}
			break;
		}
		/*time*/
		case MMS_HEADER_X_MMS_EXPIRY:
		case MMS_HEADER_X_MMS_DELIVERY_TIME:
		{
			length   = mms_input_buffer_get_value_length(input);
			uchar    = mms_input_buffer_get_int8(input);
			long_int = mms_input_buffer_get_long_int(input);
			item     = mms_header_item_time_create(name, uchar, long_int);
			break;
		}
		/*address*/
		case MMS_HEADER_FROM:
		{
			length = mms_input_buffer_get_value_length(input);
			mms_input_buffer_push_limit(input, mms_input_buffer_tell(input) + length);

			uchar = mms_input_buffer_get_int8(input);
			
			if(uchar == MMS_VAL_ADDRESS_PRESENT_TOKEN)
			{
				if(mms_input_buffer_is_value_length(input))
				{
					str = mms_input_buffer_get_encoded_string(input, &charset);
					item = mms_header_item_encoded_string_create(name, str, charset);
				}
				else
				{
					str = (char*)mms_input_buffer_get_string(input);
					item = mms_header_item_string_create(name, str);
				}
			}
			else
			{
				item = mms_header_item_long_create(name, uchar);
			}
	
			mms_input_buffer_skip_to_limit(input);
			mms_input_buffer_pop_limit(input);
			break;
		}
		/*message-class*/
		case MMS_HEADER_X_MMS_MESSAGE_CLASS:
		{
			if(mms_input_buffer_is_short_int(input))
			{
				uchar = mms_input_buffer_get_int8(input);
				item  = mms_header_item_long_create(name, uchar);
				item->value_desc = (char*)mms_id_to_name(mms_header_get_name_id_pair(id), uchar);
			}
			else
			{
				str = (char*)mms_input_buffer_get_string(input);
				item = mms_header_item_string_create(name, str);
			}
			break;
		}
		case MMS_HEADER_X_MMS_MMS_VERSION:
		{
			uchar = (unsigned char)mms_input_buffer_get_short_int(input);
			item = mms_header_item_long_create(name, uchar);
			break;
		}
		case MMS_HEADER_X_MMS_RETRIEVE_STATUS:
		case MMS_HEADER_X_MMS_RETRIEVE_TEXT:
		case MMS_HEADER_X_MMS_READ_STATUS:
		{
			if(mms_input_buffer_is_short_int(input))
			{
				uchar = mms_input_buffer_get_int8(input);
				item  = mms_header_item_long_create(name, uchar);
				item->value_desc = (char*)mms_id_to_name(mms_header_get_name_id_pair(id), uchar);
			}
			else if(mms_input_buffer_is_string(input))
			{
				str = (char*)mms_input_buffer_get_string(input);
				item = mms_header_item_string_create(name, str);
			}
			else
			{
				mms_assert(!"impossible");
			}
			break;
		}
#if 0		
		case MMS_HEADER_X_MMS_REPLY_CHARGING:
		{
			break;
		}
		case MMS_HEADER_X_MMS_REPLY_CHARGING_DEADLINE:
		{
			break;
		}
		case MMS_HEADER_X_MMS_REPLY_CHARGING_ID:
		{
			break;
		}
		case MMS_HEADER_X_MMS_REPLY_CHARGING_SIZE:
		{
			break;
		}
		case MMS_HEADER_X_MMS_PREVIOUSLY_SENT_BY:
		{
			break;
		}
		case MMS_HEADER_X_MMS_PREVIOUSLY_SENT_DATE:
		{
			break;
		}
#endif		
		default:
		{
			mms_assert(!"unreachable");
			break;
		}
	}

	if(item != NULL)
	{
		item->offset = offset;
	}
	return item;
}

MmsResult     mms_header_pack_item(HeaderItem* item, MmsOutputBuffer* output)
{
	int id = 0;
	unsigned long length = 0;

	mms_return_val_if_fail(item != NULL && output != NULL, MMS_RESULT_FAIL);

	id = mms_header_name_to_id(item->name);

	if(id < 0)
	{
		mms_assert(item->type == MMS_TYPE_STRING);
		mms_output_buffer_put_string(output, item->name);
		mms_output_buffer_put_string(output, item->value.string);

		return MMS_RESULT_OK;
	}
	mms_output_buffer_put_int8(output, (unsigned char)(id + 0x80));

	switch(id)
	{
		/*content-type*/
		case MMS_HEADER_CONTENT_TYPE:
		{
			mms_content_type_pack((MmsContentType*)item->value.data.data, output);	
			break;
		}
		/*long int*/
		case MMS_HEADER_X_MMS_MESSAGE_SIZE:
		case MMS_HEADER_DATE:
		{
			mms_assert(item->type == MMS_TYPE_LONG_INT);
			mms_output_buffer_put_long_int(output, item->value.long_int);
			break;
		}
		/*uchar*/
		case MMS_HEADER_X_MMS_MESSAGE_TYPE:
		case MMS_HEADER_X_MMS_DELIVERY_REPORT:
		case MMS_HEADER_X_MMS_PRIORITY:
		case MMS_HEADER_X_MMS_READ_REPORT:
		case MMS_HEADER_X_MMS_REPORT_ALLOWED:
		case MMS_HEADER_X_MMS_RESPONSE_STATUS:
		case MMS_HEADER_X_MMS_SENDER_VISIBILITY:
		case MMS_HEADER_X_MMS_STATUS:
		{
			mms_assert(item->type == MMS_TYPE_LONG_INT);
			mms_output_buffer_put_int8(output, (unsigned char)item->value.long_int);
			break;
		}
		/*string*/
		case MMS_HEADER_X_MMS_CONTENT_LOCATION:
		case MMS_HEADER_X_MMS_TRANSACTION_ID:
		{
			mms_assert(item->type == MMS_TYPE_STRING);

			mms_output_buffer_put_string(output, item->value.string);
			break;
		}
		/*encoded string*/
		case MMS_HEADER_BCC:
		case MMS_HEADER_CC:
		case MMS_HEADER_MESSAGE_ID:
		case MMS_HEADER_X_MMS_RESPONSE_TEXT:
		case MMS_HEADER_SUBJECT:
		case MMS_HEADER_TO:
		{
			if(item->type == MMS_TYPE_STRING)
			{
				mms_output_buffer_put_string(output, item->value.string);
			}
			else if(item->type == MMS_TYPE_ENCODED_STRING)
			{
				mms_output_buffer_put_encoded_string(output, item->value.encoded_string.string,
					item->value.encoded_string.charset);
			}
			else
			{
				mms_assert(!"invalid type");
			}
			break;
		}
		/*time*/
		case MMS_HEADER_X_MMS_EXPIRY:
		case MMS_HEADER_X_MMS_DELIVERY_TIME:
		{
			length = mms_output_buffer_get_long_int_bytes(output, item->value.time.time) + 1;
			mms_output_buffer_put_value_length(output, length);
			mms_output_buffer_put_int8(output, (unsigned char)item->value.time.rel_abs);
			mms_output_buffer_put_long_int(output, item->value.time.time);

			break;
		}
		/*address*/
		case MMS_HEADER_FROM:
		{
			if(item->type == MMS_TYPE_LONG_INT)
			{
				length = 1;
				mms_output_buffer_put_value_length(output, length);
				mms_output_buffer_put_int8(output, (unsigned char)item->value.long_int);
			}
			else
			{
				if(item->type == MMS_TYPE_STRING)
				{
					length = strlen(item->value.string) + 2;
					mms_output_buffer_put_value_length(output, length);
					mms_output_buffer_put_int8(output, MMS_VAL_ADDRESS_PRESENT_TOKEN);
					mms_output_buffer_put_string(output, item->value.string);
				}
				else if(item->type == MMS_TYPE_ENCODED_STRING)
				{
					MmsOutputBuffer* temp_output = mms_output_buffer_create(1024);
				
					mms_output_buffer_put_encoded_string(temp_output, item->value.encoded_string.string,
						item->value.encoded_string.charset);
					length = mms_output_buffer_get_buffer_length(temp_output) + 1;

					mms_output_buffer_put_value_length(output, length);
					mms_output_buffer_put_int8(output, MMS_VAL_ADDRESS_PRESENT_TOKEN);
					mms_output_buffer_put_data(output, mms_output_buffer_get_buffer(temp_output),
						mms_output_buffer_get_buffer_length(temp_output));
					mms_output_buffer_destroy(temp_output);
				}
				else
				{
					mms_assert(!"invalid type");
				}
			}
			break;
		}
		/*message-class*/
		case MMS_HEADER_X_MMS_MESSAGE_CLASS:
		{
			if(item->type == MMS_TYPE_STRING)
			{
				mms_output_buffer_put_string(output, item->value.string);
			}
			else
			{
				mms_output_buffer_put_int8(output, (unsigned char)item->value.long_int);
			}
			break;
		}
		case MMS_HEADER_X_MMS_MMS_VERSION:
		{
			mms_output_buffer_put_short_int(output, item->value.long_int);
			break;
		}
#if 0		
		case MMS_HEADER_X_MMS_RETRIEVE_STATUS:
		{
			break;
		}
		case MMS_HEADER_X_MMS_RETRIEVE_TEXT:
		{
			break;
		}
		case MMS_HEADER_X_MMS_READ_STATUS:
		{
			break;
		}
		case MMS_HEADER_X_MMS_REPLY_CHARGING:
		{
			break;
		}
		case MMS_HEADER_X_MMS_REPLY_CHARGING_DEADLINE:
		{
			break;
		}
		case MMS_HEADER_X_MMS_REPLY_CHARGING_ID:
		{
			break;
		}
		case MMS_HEADER_X_MMS_REPLY_CHARGING_SIZE:
		{
			break;
		}
		case MMS_HEADER_X_MMS_PREVIOUSLY_SENT_BY:
		{
			break;
		}
		case MMS_HEADER_X_MMS_PREVIOUSLY_SENT_DATE:
		{
			break;
		}
#endif		
		default:
		{
			if(item->type == MMS_TYPE_LONG_INT)
			{
				mms_output_buffer_put_int(output, item->value.long_int);
			}
			else if(item->type == MMS_TYPE_STRING)
			{
				mms_output_buffer_put_string(output, item->value.string);
			}
			else
			{
				mms_assert(!"invalid header");
			}
			break;
		}
	}

	return MMS_RESULT_OK;
}

#ifdef MMS_HEADER_CODER_TEST

int main(int argc, char* argv[])
{
	size_t length = 0;
	char*  buffer = NULL;
	HeaderItem* item = NULL;
	MmsInputBuffer* input = NULL;

	if(argc != 2)
	{
		printf("usage: %s mms", argv[0]);

		return -1;
	}
	
	buffer = mms_read_binary_file(argv[1], &length);
	if((input = mms_input_buffer_create(buffer, length)) != NULL)
	{
		int id = 0;
		while((item = mms_header_unpack_item(input)) != NULL)
		{
			mms_header_item_dump(item, NULL);

			if(mms_header_name_to_id(item->name) == MMS_HEADER_CONTENT_TYPE)
			{
				mms_header_item_destroy(item);
				break;
			}
			mms_header_item_destroy(item);
		}

		mms_free(buffer);
		mms_input_buffer_destroy(input);
	}

	return 0;
}
#endif/*MMS_HEADER_CODER_TEST*/
