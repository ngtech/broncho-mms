/*
 * File:    mms.c
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

#include "mms.h"
#include "mms_body.h"
#include "mms_util.h"
#include "mms_list.h"
#include "mms_header.h"
#include "mms_header_codec.h"
#include "mms_input_buffer.h"
#include "mms_output_buffer.h"

struct _Mms
{
	MmsHeader* header;
	MmsBody*   body;

	MmsList*   auto_release;
};

Mms*             mms_create(void)
{
	Mms* thiz = (Mms*)mms_calloc(1, sizeof(Mms));
	
	if(thiz != NULL)
	{
		thiz->header = mms_header_create(mms_header_unpack_item, mms_header_pack_item);
		thiz->body   = mms_body_create();
		
    	mms_header_set_unpack_end_at_content_type(thiz->header, 1);
	}
	
	return thiz;
}

void             mms_destroy(Mms* thiz)
{
	if(thiz != NULL)
	{
		mms_header_destroy(thiz->header);
		thiz->header = NULL;

		mms_body_destroy(thiz->body);
		thiz->body = NULL;

		if(thiz->auto_release != NULL)
		{
			mms_list_destroy(thiz->auto_release);
			thiz->auto_release = NULL;
		}

		mms_free(thiz);
	}

	return;
}

static  void mms_add_auto_release(Mms* thiz, void* data)
{
	mms_return_if_fail(thiz != NULL && data != NULL);

	if(thiz->auto_release == NULL)
	{
		thiz->auto_release = mms_list_create((mms_destroy_func)mms_free);
	}

	mms_list_append(thiz->auto_release, data);

	return;
}

MmsResult        mms_unpack(Mms* thiz, MmsInputBuffer* input)
{
	mms_return_val_if_fail(thiz != NULL && input != NULL, MMS_RESULT_FAIL);

	mms_header_unpack(thiz->header, input);
	mms_body_unpack(thiz->body, input);

	return MMS_RESULT_OK;
}

MmsResult        mms_pack(Mms* thiz, MmsOutputBuffer* output)
{
	mms_return_val_if_fail(thiz != NULL && output != NULL, MMS_RESULT_FAIL);

	mms_header_pack(thiz->header, output);
	mms_body_pack(thiz->body, output);

	return MMS_RESULT_OK;
}

MmsResult        mms_set_header(Mms* thiz, MmsHeader* header)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(thiz->header != header, MMS_RESULT_FAIL);

	if(thiz->header != NULL)
	{
		mms_header_destroy(thiz->header);
		thiz->header = NULL;
	}

	thiz->header = header;

	return MMS_RESULT_OK;
}

MmsResult        mms_set_body(Mms* thiz, MmsBody* body)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(thiz->body != body, MMS_RESULT_FAIL);

	if(thiz->body != NULL)
	{
		mms_body_destroy(thiz->body);
		thiz->body = NULL;
	}

	thiz->body = body;

	return MMS_RESULT_OK;
}

MmsHeader*       mms_get_header(Mms* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);

	return thiz->header;
}

MmsBody*         mms_get_body(Mms* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);

	return thiz->body;
}

Mms*      mms_create_send_req(unsigned long expire_abs)
{
	char buffer[32] = {0};
	Mms* thiz = mms_create();
	MmsContentType* content_type = mms_content_type_create();

	sprintf(buffer, "%d", mms_get_unique_id());
	mms_header_set_content_type(thiz->header, content_type);
	mms_header_set_long(thiz->header,   "X-Mms-Message-Type",   MMS_VAL_M_SEND_REQ);
	mms_header_set_string(thiz->header, "X-Mms-Transaction-Id", buffer);
	mms_header_set_long(thiz->header,   "X-Mms-MMS-Version",    MMS_VERSION);

	mms_set_from(thiz, NULL, 0);
	mms_set_to(thiz, NULL, 0);
	//mms_set_subject(thiz, "none", 0);
	
	if(expire_abs != 0)
	{
		mms_set_expire(thiz, expire_abs);
	}
	
	mms_header_set_long(thiz->header,   "Date", time(0));
	mms_header_set_long(thiz->header,   "X-Mms-Delivery-Report", MMS_VAL_NO);
	mms_header_set_long(thiz->header,   "X-Mms-Sender-Visibility", MMS_VAL_SHOW);
	mms_header_set_long(thiz->header,   "X-Mms-Read-Report", MMS_VAL_NO);
	mms_header_set_long(thiz->header,   "X-Mms-Message-Class", MMS_VAL_PERSONAL);
	mms_header_set_long(thiz->header,   "X-Mms-Priority", MMS_VAL_NORMAL);


	mms_content_type_set_content_type_by_name(content_type, "application/vnd.wap.multipart.related");
	mms_content_type_set_param_type(content_type, "application/smil");
	mms_content_type_set_param_start(content_type, MMS_SMIL_NAME);

	return thiz;
}

Mms*      mms_create_delivery_ack(const char* transaction_id)
{
	Mms* thiz = NULL;
	
	mms_return_val_if_fail(transaction_id != NULL, NULL);

	thiz = mms_create();

	mms_header_set_long(thiz->header,   "X-Mms-Message-Type",   MMS_VAL_M_ACKNOWLEDGE_IND);
	mms_header_set_string(thiz->header, "X-Mms-Transaction-Id", transaction_id);
	mms_header_set_long(thiz->header,   "X-Mms-MMS-Version",    MMS_VERSION);

	return thiz;
}

MmsResult mms_set_expire_relat (Mms* thiz, unsigned long expire_relat)
{
	mms_return_val_if_fail(thiz != NULL && expire_relat != 0, MMS_RESULT_FAIL);

	return mms_header_set_time(thiz->header, "X-Mms-Expiry", 1, expire_relat);
}

MmsResult mms_set_expire(Mms* thiz, time_t expire_abs)
{
	mms_return_val_if_fail(thiz != NULL && expire_abs != 0, MMS_RESULT_FAIL);
	
	return mms_header_set_time(thiz->header, "X-Mms-Expiry", 0, expire_abs);
}

MmsResult mms_set_from(Mms* thiz, const char* value, int charset)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	if(value == NULL)
	{
		return mms_header_set_long(thiz->header,   "From", 129);
	}
	else
	{
		if(charset == 0)
		{
			return mms_header_set_string(thiz->header, "From", value);
		}
		else
		{
			return mms_header_set_encoded_string(thiz->header, "From", (void*)value, charset);
		}
	}
}

MmsResult mms_set_to(Mms* thiz, const char* value, int charset)
{
	mms_return_val_if_fail(thiz != NULL && value != NULL, MMS_RESULT_FAIL);

	if(charset == 0)
	{
		return mms_header_set_string(thiz->header, "To", value);
	}
	else
	{
		return mms_header_set_encoded_string(thiz->header, "To", (void*)value, charset);
	}
}

MmsResult mms_set_subject(Mms* thiz, const char* value, int charset)
{
	mms_return_val_if_fail(thiz != NULL && value != NULL, MMS_RESULT_FAIL);

	if(charset == 0)
	{
		return mms_header_set_string(thiz->header, "Subject", value);
	}
	else
	{
		return mms_header_set_encoded_string(thiz->header, "Subject", (void*)value, charset);
	}
}

MmsResult mms_set_delivery_report(Mms* thiz, int need_delivery_report)
{
	unsigned long delivery_report = need_delivery_report ? MMS_VAL_YES : MMS_VAL_NO;
	
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	return mms_header_set_long(thiz->header,   "X-Mms-Delivery-Report", delivery_report);
}

MmsResult mms_set_read_report(Mms* thiz, int need_read_report)
{
	unsigned long read_report = need_read_report ? MMS_VAL_YES : MMS_VAL_NO;

	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	return mms_header_set_long(thiz->header,   "X-Mms-Read-Report", read_report);
}

MmsResult        mms_set_header_string(Mms* thiz, const char* name, const char* value)
{
	mms_return_val_if_fail(thiz != NULL && name != NULL && value != NULL, MMS_RESULT_FAIL);

	return mms_header_set_string(thiz->header, name, value);
}

MmsResult        mms_set_header_long(Mms* thiz, const char* name, long value)
{
	mms_return_val_if_fail(thiz != NULL && name != NULL, MMS_RESULT_FAIL);

	return mms_header_set_long(thiz->header,   name, value);
}

MmsResult        mms_set_header_time(Mms* thiz, const char* name, int is_relative, unsigned long value)
{
	mms_return_val_if_fail(thiz != NULL && name != NULL, MMS_RESULT_FAIL);

	return mms_header_set_time(thiz->header,   name, is_relative, value);
}

Mms*             mms_create_from_buffer(void* buffer, size_t length, int callee_release)
{
	Mms* thiz = NULL;
	MmsInputBuffer* input = NULL;
	
	mms_return_val_if_fail(buffer != NULL && length != 0, NULL);
	thiz = mms_create();
	mms_return_val_if_fail(thiz != NULL, NULL);
	
	input = mms_input_buffer_create(buffer, length);
	mms_unpack(thiz, input);
	mms_input_buffer_destroy(input);

	if(callee_release)
	{
		mms_add_auto_release(thiz, buffer);	
	}

	return thiz;
}

Mms*             mms_create_from_file(const char* filename)
{
	char*  buffer = NULL;
	size_t length = 0;
	Mms* thiz = NULL;
	
	mms_return_val_if_fail(filename != NULL, NULL);
	buffer = mms_read_binary_file(filename, &length);
	mms_return_val_if_fail(buffer != NULL, NULL);

	if((thiz = mms_create_from_buffer(buffer, length, 1)) == NULL)
	{
		mms_free(buffer);
	}

	return thiz;
}

void*     mms_save_to_buffer(Mms* thiz, size_t* length)
{
	char*  buffer = NULL;
	MmsOutputBuffer* output = NULL;

	mms_return_val_if_fail(thiz != NULL && length != NULL, NULL);

	output = mms_output_buffer_create(2048);
	mms_pack(thiz, output);
	buffer = mms_output_buffer_get_buffer(output);
	*length = mms_output_buffer_get_buffer_length(output);

	mms_output_buffer_detach_buffer(output);
	mms_output_buffer_destroy(output);

	return buffer;
}

MmsResult        mms_save_to_file(Mms* thiz, const char* filename)
{
	char*  buffer = NULL;
	size_t length = 0;
	MmsResult ret = MMS_RESULT_FAIL;

	mms_return_val_if_fail(thiz != NULL && filename != NULL, MMS_RESULT_FAIL);

	buffer = mms_save_to_buffer(thiz, &length);
	ret = mms_write_binary_file(filename, buffer, length);

	return ret;
}

int mms_add_par(Mms* thiz, int duration)
{
	MmsSmil* smil = NULL;
	mms_return_val_if_fail(thiz != NULL, -1);
	smil = mms_body_get_smil(thiz->body);
	mms_return_val_if_fail(smil != NULL, -1);

	return mms_smil_add_par(smil, duration);
}

MmsResult mms_erase_par(Mms* thiz, size_t par_index)
{
	char* filename = NULL;
	MmsSmil* smil = NULL;
	
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	
	smil = mms_body_get_smil(thiz->body);
	mms_return_val_if_fail(smil != NULL, MMS_RESULT_FAIL);

	mms_par_erase_text(thiz,  par_index);
	mms_par_erase_image(thiz, par_index);
	mms_par_erase_audio(thiz, par_index);

	mms_smil_erase_par(smil, par_index);

	return MMS_RESULT_OK;
}

MmsResult mms_set_image_region(Mms* thiz, int x, int y, int width, int height)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	
	return mms_smil_add_region(mms_body_get_smil(thiz->body), "image", x, y, width, height);
}

MmsResult mms_set_text_region(Mms* thiz, int x, int y, int width, int height)
{
	return mms_smil_add_region(mms_body_get_smil(thiz->body), "text", x, y, width, height);
}

static int mms_is_par_invalid(Mms* thiz, size_t par_index)
{
	mms_return_val_if_fail(thiz != NULL, 0);

	return (par_index < mms_smil_get_par_count(mms_body_get_smil(thiz->body)));
}

#define MMS_ERASE_FILE(type) \
	char* filename = NULL;\
	mms_return_val_if_fail(mms_is_par_invalid(thiz, par_index), MMS_RESULT_FAIL);\
    \
	mms_smil_par_get_##type(mms_body_get_smil(thiz->body), par_index, NULL, &filename);\
    \
	if(filename != NULL && filename[0] != '\0') \
	{ \
		char save_filename[MMS_MAX_PATH + 1] = {0};\
		strncpy(save_filename, filename, MMS_MAX_PATH);\
		mms_smil_par_set_##type(mms_body_get_smil(thiz->body), par_index, NULL, ""); \
		if(!mms_smil_is_file_exist(mms_body_get_smil(thiz->body), save_filename))\
		{\
			mms_body_erase_body_part(thiz->body, save_filename);\
		}\
	}\
    \
	return MMS_RESULT_OK;

MmsResult mms_par_erase_image(Mms* thiz, size_t par_index)
{
	MMS_ERASE_FILE(image);
}

MmsResult mms_par_erase_text(Mms* thiz, size_t par_index)
{
	MMS_ERASE_FILE(text);
}

MmsResult mms_par_erase_audio(Mms* thiz, size_t par_index)
{
	MMS_ERASE_FILE(audio);
}

static MmsResult mms_add_body_part(Mms* thiz, const char* filename)
{
	MmsBodyPart* body_part = NULL;
	
	mms_return_val_if_fail(thiz != NULL && filename != NULL, MMS_RESULT_FAIL);

	if(mms_body_find_body_part(thiz->body, filename) == NULL)
	{
		body_part = mms_body_part_create_from_file(filename);
		mms_return_val_if_fail(body_part != NULL, MMS_RESULT_FAIL);

		mms_body_add_body_part(thiz->body, body_part);	
	}
	else
	{
		MMS_PRINTF("%s(%d): reuse the same body part.\n", __FILE__, __LINE__);
	}

	return MMS_RESULT_OK;
}

#define MMS_PAR_SET_FILE(type) \
	mms_return_val_if_fail(mms_is_par_invalid(thiz, par_index), MMS_RESULT_FAIL);\
    mms_return_val_if_fail(mms_par_erase_##type(thiz, par_index) == MMS_RESULT_OK, MMS_RESULT_FAIL); \
	mms_return_val_if_fail(mms_add_body_part(thiz, filename) == MMS_RESULT_OK, MMS_RESULT_FAIL); \
	return mms_smil_par_set_##type(mms_body_get_smil(thiz->body), par_index, #type, (char*)filename);

MmsResult        mms_par_set_image(Mms* thiz, size_t par_index, const char* filename)
{
	MMS_PAR_SET_FILE(image);
}

MmsResult        mms_par_set_text(Mms* thiz, size_t par_index, const char* filename)
{
	MMS_PAR_SET_FILE(text);
}

MmsResult        mms_par_set_audio(Mms* thiz, size_t par_index, const char* filename)
{
	MMS_PAR_SET_FILE(audio);
}

MmsResult mms_par_set_text_buffer(Mms* thiz, size_t par_index, const char* buffer, int callee_release)
{
	MmsBodyPart* body_part = NULL;
	char filename[MMS_MAX_PATH + 1] = {0};
	char* old_filename = NULL;

	mms_return_val_if_fail(buffer != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(mms_is_par_invalid(thiz, par_index), MMS_RESULT_FAIL);	
	mms_return_val_if_fail(mms_par_erase_text(thiz, par_index) == MMS_RESULT_OK, MMS_RESULT_FAIL);

	snprintf(filename, MMS_MAX_PATH, "mms_%d.txt", mms_get_unique_id());
	body_part = mms_body_part_create_from_buffer(filename, (void*)buffer, strlen(buffer) + 1, callee_release);
	mms_return_val_if_fail(body_part != NULL, MMS_RESULT_FAIL);

	mms_body_add_body_part(thiz->body, body_part);	
	
	return mms_smil_par_set_text(mms_body_get_smil(thiz->body), par_index, "text", (char*)filename);
}

static MmsResult mms_visit_body_part_calc_size(void* data, void* user_data)
{
	size_t* size = (size_t*)user_data;
	MmsBodyPart* body_part = (MmsBodyPart*)data;

	mms_return_val_if_fail(size != NULL && body_part != NULL, MMS_RESULT_FAIL);

	*size += 50; /*for header*/
	*size += mms_body_part_get_data_length(body_part);

	return MMS_RESULT_OK;
}

size_t    mms_get_size(Mms* thiz)
{
	size_t size = 0;
	mms_return_val_if_fail(thiz != NULL, 0);
	
	size = 100; /*for mms header*/
	size += mms_smil_get_size(mms_body_get_smil(thiz->body));
	mms_body_foreach(thiz->body, mms_visit_body_part_calc_size, &size);

	return size;
}

MmsResult mms_add_attachment(Mms* thiz,   const char* filename)
{
	mms_return_val_if_fail(thiz != NULL && filename != NULL, MMS_RESULT_FAIL);	
	
	if(mms_smil_is_file_exist(mms_body_get_smil(thiz->body), filename)
		|| mms_body_find_body_part(thiz->body, filename) != NULL)
	{
		/*exit*/
		return MMS_RESULT_FAIL;
	}

	return mms_body_add_body_part(thiz->body, mms_body_part_create_from_file(filename));
}

MmsResult mms_erase_attachment(Mms* thiz, const char* filename)
{
	mms_return_val_if_fail(thiz != NULL && filename != NULL, MMS_RESULT_FAIL);

	if(mms_smil_is_file_exist(mms_body_get_smil(thiz->body), filename))
	{
		/*can't erase if ref by smil*/
		return MMS_RESULT_FAIL;
	}

	return mms_body_erase_body_part(thiz->body, filename);
}

MmsResult mms_erase_all_attachments(Mms* thiz)
{
	size_t i = 0;
	size_t n = 0;
	MmsList*  attachments = mms_get_attachments(thiz);	

	mms_return_val_if_fail(attachments != NULL, MMS_RESULT_FAIL);

	n = mms_list_length(attachments);
	for(i = 0; i < n; i++)
	{
		mms_erase_attachment(thiz, (const char*)mms_list_nth(attachments, i));
	}
	mms_list_destroy(attachments);

	return MMS_RESULT_OK;
}

typedef struct _VisitBodyPartCtx
{
	Mms* mms;
	MmsList* attachments;
}VisitBodyPartCtx;

static MmsResult   mms_visit_body_part(void* data, void* user_data)
{
	MmsBodyPart* body_part = (MmsBodyPart*)data;
	VisitBodyPartCtx* ctx = (VisitBodyPartCtx*)user_data;
	const char* filename = mms_body_part_get_name(body_part);
	
	mms_assert(filename != NULL);

	if(!mms_smil_is_file_exist(mms_body_get_smil(ctx->mms->body), filename))
	{
		mms_list_append(ctx->attachments, (void*)filename);
	}
	
	return MMS_RESULT_OK;
}

MmsList*  mms_get_attachments(Mms* thiz)
{
	VisitBodyPartCtx ctx = {0};
	mms_return_val_if_fail(thiz != NULL, NULL);

	ctx.mms = thiz;
	ctx.attachments = mms_list_create(NULL);

	mms_body_foreach(thiz->body, mms_visit_body_part, &ctx);

	return ctx.attachments;
}

time_t      mms_get_expire(Mms* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);

	return mms_header_get_time(thiz->header, "X-Mms-Expiry", NULL);
}

time_t      mms_get_date(Mms* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);

	return mms_header_get_long(thiz->header, "Date");
}

const char* mms_get_from(Mms* thiz, int* charset)
{
	const char* key = "From";
	mms_return_val_if_fail(thiz != NULL, NULL);

	if(mms_header_get_item_type(thiz->header, key) == MMS_TYPE_STRING)
	{
		return mms_header_get_string(thiz->header, key);
	}
	else
	{
		return mms_header_get_encoded_string(thiz->header, key, charset);
	}
}

const char* mms_get_to(Mms* thiz, int* charset)
{
	const char* key = "To";
	mms_return_val_if_fail(thiz != NULL, NULL);

	if(mms_header_get_item_type(thiz->header, key) == MMS_TYPE_STRING)
	{
		return mms_header_get_string(thiz->header, key);
	}
	else
	{
		return mms_header_get_encoded_string(thiz->header, key, charset);
	}
}

const char* mms_get_subject(Mms* thiz, int* charset)
{
	const char* key = "Subject";
	mms_return_val_if_fail(thiz != NULL, NULL);

	if(mms_header_get_item_type(thiz->header, key) == MMS_TYPE_STRING)
	{
		return mms_header_get_string(thiz->header, key);
	}
	else
	{
		return mms_header_get_encoded_string(thiz->header, key, charset);
	}
}

int         mms_get_delivery_report(Mms* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);

	return mms_header_get_long(thiz->header, "X-Mms-Delivery-Report") == MMS_VAL_YES;
}

int         mms_get_read_report(Mms* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);
	
	return mms_header_get_long(thiz->header, "X-Mms-Read-Report") == MMS_VAL_YES;
}

size_t      mms_get_total_duration(Mms* thiz)
{
	size_t i = 0;
	size_t n = 0;
	size_t t = 0;
	mms_return_val_if_fail(thiz != NULL, 0);
	
	n = mms_get_par_count(thiz);

	for(i = 0; i < n; i++)
	{
		t += mms_par_get_duration(thiz, i);
	}

	return t;
}

int         mms_get_status(Mms* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);
	
	return mms_header_get_long(thiz->header, "X-Mms-Status");
}

size_t           mms_get_par_count(Mms* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 0);

	return mms_smil_get_par_count(mms_body_get_smil(thiz->body));
}

MmsResult mms_par_set_duration(Mms* thiz, size_t par_index, size_t duration)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	
	return mms_smil_par_set_duration(mms_body_get_smil(thiz->body), par_index, duration);
}

size_t           mms_par_get_duration(Mms* thiz, size_t par_index)
{
	mms_return_val_if_fail(thiz != NULL, 0);

	return mms_smil_par_get_duration(mms_body_get_smil(thiz->body), par_index);
}

MmsResult        mms_par_get_image(Mms* thiz, size_t par_index, char** filename)
{
	mms_return_val_if_fail(thiz != NULL && filename != NULL, MMS_RESULT_FAIL);

	return mms_smil_par_get_image(mms_body_get_smil(thiz->body), par_index, NULL, filename);
}

MmsResult        mms_par_get_text(Mms* thiz,  size_t par_index, char** filename)
{
	mms_return_val_if_fail(thiz != NULL && filename != NULL, MMS_RESULT_FAIL);

	return mms_smil_par_get_text(mms_body_get_smil(thiz->body), par_index, NULL, filename);
}

MmsResult        mms_par_get_audio(Mms* thiz, size_t par_index, char** filename)
{
	mms_return_val_if_fail(thiz != NULL && filename != NULL, MMS_RESULT_FAIL);

	return mms_smil_par_get_audio(mms_body_get_smil(thiz->body), par_index, NULL, filename);
}

MmsResult        mms_par_get_file_data(Mms* thiz, const char* filename, void** data, size_t* length, char** type)
{
	MmsBodyPart* body_part = NULL;

	mms_return_val_if_fail(thiz != NULL && filename != NULL && filename[0] != '\0', MMS_RESULT_FAIL);

	body_part = mms_body_find_body_part(thiz->body, filename);
	mms_return_val_if_fail(body_part != NULL, MMS_RESULT_FAIL);

	if(data != NULL)
	{
		*data = (void*)mms_body_part_get_data(body_part);
	}

	if(length != NULL)
	{
		*length = mms_body_part_get_data_length(body_part);
	}

	if(type != NULL)
	{
		MmsContentType* content_type = mms_header_get_content_type(mms_body_part_get_header(body_part));
		if(content_type != NULL)
		{
			*type = (char*)mms_content_type_get_content_type_name(content_type);
		}
	}

	return MMS_RESULT_OK;
}

MmsResult        mms_dump(Mms* thiz, void* unused)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	mms_header_dump(thiz->header, NULL);
	mms_body_dump(thiz->body, NULL);

	MMS_PRINTF("mms size: %d\n", mms_get_size(thiz));

	return MMS_RESULT_OK;
}

static MmsResult mms_remove_date(void* data, void* user_data)
{
	MmsHeader* header = mms_body_part_get_header((MmsBodyPart*)data);

	mms_header_erase(header, "Date");

	return MMS_RESULT_OK;
}

MmsResult mms_fix_nokia(Mms* thiz)
{
	MmsHeader* header = NULL;
	
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	if((header = mms_smil_get_header(mms_body_get_smil(thiz->body))) != NULL)
	{
		mms_header_erase(header, "Date");
	}

	mms_body_foreach(thiz->body, mms_remove_date, NULL);

	return MMS_RESULT_OK;
}

#ifdef MMS_TEST
#include "mms_util.h"

static void test_unpack_pack(const char* filename)
{
	MmsOutputBuffer* output = mms_output_buffer_create(128);
    Mms* thiz = mms_create_from_file(filename);
    
    mms_pack(thiz, output);
	mms_dump(thiz, NULL);

	mms_write_binary_file("lxj.mms", mms_output_buffer_get_buffer(output), mms_output_buffer_get_buffer_length(output));
	
	mms_destroy(thiz);
	mms_output_buffer_destroy(output);

	return;
}

static void test_get_set(void)
{
	Mms* thiz = mms_create();
	size_t length = 0;
	int charset = 0;

	mms_assert(mms_get_header(thiz) != NULL);
	mms_assert(mms_get_body(thiz) != NULL);

	mms_assert(mms_set_expire(thiz, 10000) == MMS_RESULT_OK);
	mms_assert(mms_get_expire(thiz) == 10000);
	
	mms_assert(mms_set_from(thiz, "123", CHARSET_UTF_8) == MMS_RESULT_OK);
	mms_assert(strcmp(mms_get_from(thiz, &charset), "123") == 0 && charset == CHARSET_UTF_8);
	
	mms_assert(mms_set_to(thiz, "321", 0) == MMS_RESULT_OK);
	mms_assert(strcmp(mms_get_to(thiz, NULL), "321") == 0);

	mms_assert(mms_set_subject(thiz, "abc", CHARSET_GB2312) == MMS_RESULT_OK);
	mms_assert(strcmp(mms_get_subject(thiz, &charset), "abc") == 0 && charset == CHARSET_GB2312);

	mms_assert(mms_set_read_report(thiz, 1) == MMS_RESULT_OK);
	mms_assert(mms_get_read_report(thiz));

	mms_assert(mms_set_delivery_report(thiz, 1) == MMS_RESULT_OK);
	mms_assert(mms_get_delivery_report(thiz));

	mms_set_header(thiz, NULL);
	mms_set_body(thiz, NULL);
	mms_destroy(thiz);

	return ;
}

static void test_add_par(Mms* thiz, const char* audio, const char* image, const char* text)
{
	char* str = NULL;
	char* content_type = NULL;
	const char* filename = NULL;
	void* data = NULL;
	size_t par_index = 0;
	size_t length = 0;

	size_t old_par_count = mms_get_par_count(thiz);

	par_index = mms_add_par(thiz, 10);
	mms_assert(mms_get_par_count(thiz) == (old_par_count + 1));
	mms_assert(mms_par_get_duration(thiz, 0) == 10);
	mms_assert(mms_par_set_duration(thiz, 0, 111) == MMS_RESULT_OK);
	mms_assert(mms_par_get_duration(thiz, 0) == 111);
	mms_assert(mms_par_set_duration(thiz, 0, 10) == MMS_RESULT_OK);
	
	if(audio != NULL)
	{
		filename = audio;
		mms_par_set_audio(thiz, par_index, filename);
		mms_assert(mms_par_get_audio(thiz, par_index, &str) == MMS_RESULT_OK && strcmp(str, mms_get_file_name(filename)) == 0);
		mms_assert(mms_par_get_file_data(thiz, filename, &data, &length, &content_type) == MMS_RESULT_OK);
	}

	if(image != NULL)
	{
		filename = image;
		mms_par_set_image(thiz, par_index, filename);
		mms_assert(mms_par_get_image(thiz, par_index, &str) == MMS_RESULT_OK && strcmp(str, mms_get_file_name(filename)) == 0);
		mms_assert(mms_par_get_file_data(thiz, filename, &data, &length, &content_type) == MMS_RESULT_OK);
	}

	if(text != NULL)
	{
		filename = text;
		mms_par_set_text(thiz, par_index, filename);
		mms_assert(mms_par_get_text(thiz, par_index, &str) == MMS_RESULT_OK && strcmp(str, mms_get_file_name(filename)) == 0);
		mms_assert(mms_par_get_file_data(thiz, filename, &data, &length, &content_type) == MMS_RESULT_OK);
	}

	return;
}

static void test_construct(void)
{
	int i = 0;
	const char* from    = NULL;
	const char* to      = NULL;
	const char* subject = NULL;
	int has_read_report	= 0;
	size_t length = 0;
	int charset = 0;
	MmsOutputBuffer* output = mms_output_buffer_create(1024);

	Mms* thiz = mms_create_send_req(time(0) + 3600);
#if 0	
	mms_assert(mms_set_read_report(thiz, 1) == MMS_RESULT_OK);
	mms_assert(mms_set_delivery_report(thiz, 1) == MMS_RESULT_OK);

	mms_assert(mms_set_from(thiz, "1234", CHARSET_UTF_8) == MMS_RESULT_OK);
	from = mms_get_from(thiz, &charset);
	mms_assert(strcmp(from, "1234") == 0 && charset == CHARSET_UTF_8);

	mms_assert(mms_set_to(thiz, "4321", CHARSET_GB2312) == MMS_RESULT_OK);
	to = mms_get_to(thiz, &charset);
	mms_assert(strcmp(to, "4321") == 0 && charset == CHARSET_GB2312);

	mms_assert(mms_set_subject(thiz, "\xd6\xd0\xb9\xfa", CHARSET_GBK) == MMS_RESULT_OK);
	subject = mms_get_subject(thiz, &charset);
	mms_assert(strcmp(subject, "\xd6\xd0\xb9\xfa") == 0 && charset == CHARSET_GBK);

	mms_assert(mms_get_read_report(thiz));
	mms_assert(mms_get_delivery_report(thiz));

	test_add_par(thiz,  "testdata/flourish.mid", "testdata/gol.gif", "testdata/score.txt");
	mms_assert(mms_get_total_duration(thiz) == 10);
	test_add_par(thiz,  NULL, "testdata/nokia.gif", "testdata/ad.txt");
	mms_assert(mms_get_total_duration(thiz) == 20);
	mms_assert(mms_body_get_body_part_count(mms_get_body(thiz)) == 5);
	test_add_par(thiz,  "testdata/flourish.mid", "testdata/gol.gif", "testdata/score.txt");
	mms_assert(mms_get_total_duration(thiz) == 30);
	test_add_par(thiz,  NULL, "testdata/nokia.gif", "testdata/ad.txt");
	mms_assert(mms_get_total_duration(thiz) == 40);
	mms_assert(mms_body_get_body_part_count(mms_get_body(thiz)) == 5);

	mms_par_set_text_buffer(thiz, 0, "hello", 0);

	while(mms_get_par_count(thiz) > 0)
	{
		mms_assert(mms_erase_par(thiz, 0) == MMS_RESULT_OK);
	}
	test_add_par(thiz,  "testdata/flourish.mid", "testdata/gol.gif", "testdata/score.txt");
#endif	
	mms_dump(thiz, NULL);
	mms_pack(thiz, output);
	mms_write_binary_file("lxj.mms", mms_output_buffer_get_buffer(output), mms_output_buffer_get_buffer_length(output));

	mms_destroy(thiz);
	mms_output_buffer_destroy(output);
	
	thiz = mms_create_delivery_ack("10000");
	mms_dump(thiz, NULL);
	mms_destroy(thiz);

	return ;
}

static void fix_nokia(const char* filename)
{
	Mms* thiz = NULL;
	mms_return_if_fail(filename != NULL);
	
	if((thiz = mms_create_from_file(filename)) != NULL)
	{
		mms_fix_nokia(thiz);
		mms_save_to_file(thiz, "fix_nokia.mms");
		mms_destroy(thiz);
	}

	return;
}

static void test_attachments(const char* filename)
{
	Mms* thiz = mms_create_send_req(0);
	MmsList* attachments = NULL;
	
	mms_assert(mms_add_attachment(thiz, filename) == MMS_RESULT_OK);
	mms_assert(mms_add_attachment(thiz, filename) == MMS_RESULT_FAIL);

	attachments = mms_get_attachments(thiz);
	mms_assert(mms_list_length(attachments) == 1);
	mms_assert(strcmp(mms_list_nth(attachments, 0), mms_get_file_name(filename)) == 0);

	mms_assert(mms_erase_attachment(thiz, filename) == MMS_RESULT_OK);
	mms_assert(mms_erase_attachment(thiz, filename) == MMS_RESULT_FAIL);
	mms_list_destroy(attachments);
	
	mms_assert(mms_add_attachment(thiz, filename) == MMS_RESULT_OK);
	mms_assert(mms_erase_all_attachments(thiz) == MMS_RESULT_OK);
	attachments = mms_get_attachments(thiz);
	mms_assert(mms_list_length(attachments) == 0);

	mms_destroy(thiz);

	return;
}

int main(int argc, char* argv[])
{
    size_t length = 0;
    char*  buffer = NULL;
    HeaderItem* item = NULL;
    MmsInputBuffer* input = NULL;
    
	if(argc < 2 || strcmp(argv[1], "--help") == 0)
	{
		MMS_PRINTF("usage: %s test_get_set\n", argv[0]);
		MMS_PRINTF("usage: %s test_construct\n", argv[0]);
		MMS_PRINTF("usage: %s test_unpack_pack mms\n", argv[0]);
		MMS_PRINTF("usage: %s fix_nokia mms\n", argv[0]);
		MMS_PRINTF("usage: %s test_attachments filename\n", argv[0]);

		return -1;
	}

	if(strcmp(argv[1], "test_get_set") == 0)
	{
		test_get_set();
	}
	else if(strcmp(argv[1], "test_construct") == 0)
	{
		test_construct();
	}
	else if(strcmp(argv[1], "test_unpack_pack") == 0)
	{
		test_unpack_pack(argv[2]);
	}
	else if(strcmp(argv[1], "fix_nokia") == 0)
	{
		fix_nokia(argv[2]);
	}
	else if(strcmp(argv[1], "test_attachments") == 0)
	{
		test_attachments(argv[2]);
	}

	return 0;
}
#endif/*MMS_TEST*/

