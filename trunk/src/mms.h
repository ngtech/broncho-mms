/*
 * File:    mms.h
 * Author:  Li XianJing <lixianjing@broncho.cn>
 * Brief:   mms
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

#ifndef MMS_H
#define MMS_H

#include "mms_charset.h"
#include "mms_body.h"
#include "mms_list.h"
#include "mms_header.h"
#include "mms_input_buffer.h"
#include "mms_output_buffer.h"

MMS_BEGIN_DECLS

struct Mms;
typedef struct _Mms Mms;

Mms*             mms_create(void);
void             mms_destroy(Mms* thiz);

MmsResult        mms_unpack(Mms* thiz, MmsInputBuffer* input);
MmsResult        mms_pack(Mms* thiz, MmsOutputBuffer* output);

MmsResult        mms_set_header(Mms* thiz, MmsHeader* header);
MmsResult        mms_set_body(Mms* thiz, MmsBody* body);

MmsHeader*       mms_get_header(Mms* thiz);
MmsBody*         mms_get_body(Mms* thiz);

MmsResult        mms_dump(Mms* thiz, void* unused);

/*focade*/
/*construct*/
Mms*      mms_create_delivery_ack(const char* transaction_id);
Mms*      mms_create_send_req(unsigned long expire_abs);

MmsResult mms_set_expire(Mms* thiz, time_t expire_abs);
MmsResult mms_set_expire_relat (Mms* thiz, unsigned long expire_relat);
MmsResult mms_set_from(Mms* thiz, const char* value, int charset);
MmsResult mms_set_to(Mms* thiz, const char* value, int charset);
MmsResult mms_set_subject(Mms* thiz, const char* value, int charset);
MmsResult mms_set_delivery_report(Mms* thiz, int need_delivery_report);
MmsResult mms_set_read_report(Mms* thiz, int need_read_report);

MmsResult mms_set_image_region(Mms* thiz, int x, int y, int width, int height);
MmsResult mms_set_text_region(Mms* thiz, int x, int y, int width, int height);

int       mms_add_par(Mms* thiz, int duration);
MmsResult mms_par_set_duration(Mms* thiz, size_t par_index, size_t duration);
MmsResult mms_par_set_image(Mms* thiz, size_t par_index, const char* filename);
MmsResult mms_par_set_text(Mms* thiz, size_t par_index, const char* filename);
MmsResult mms_par_set_text_buffer(Mms* thiz, size_t par_index, const char* buffer, int callee_release);
MmsResult mms_par_set_audio(Mms* thiz, size_t par_index, const char* filename);

MmsResult mms_erase_par(Mms* thiz, size_t par_index);
MmsResult mms_par_erase_image(Mms* thiz, size_t par_index);
MmsResult mms_par_erase_text(Mms* thiz,  size_t par_index);
MmsResult mms_par_erase_audio(Mms* thiz, size_t par_index);

size_t    mms_get_size(Mms* thiz);

MmsResult mms_add_attachment(Mms* thiz,   const char* filename);
MmsResult mms_erase_attachment(Mms* thiz, const char* filename);
MmsResult mms_erase_all_attachments(Mms* thiz);
MmsList*  mms_get_attachments(Mms* thiz);

/*retrieve*/
time_t      mms_get_expire(Mms* thiz);
time_t      mms_get_date(Mms* thiz);
const char* mms_get_from(Mms* thiz, int* charset);
const char* mms_get_to(Mms* thiz, int* charset);
const char* mms_get_subject(Mms* thiz, int* charset);
int         mms_get_delivery_report(Mms* thiz);
int         mms_get_read_report(Mms* thiz);
size_t      mms_get_total_duration(Mms* thiz);
int         mms_get_status(Mms* thiz);

size_t    mms_get_par_count(Mms* thiz);
size_t    mms_par_get_duration(Mms* thiz, size_t par_index);
MmsResult mms_par_get_image(Mms* thiz, size_t par_index, char** filename);
MmsResult mms_par_get_text(Mms* thiz,  size_t par_index, char** filename);
MmsResult mms_par_get_audio(Mms* thiz, size_t par_index, char** filename);
MmsResult mms_par_get_file_data(Mms* thiz, const char* filename, void** data, size_t* length, char** content_type);

/*load/store*/
MmsResult mms_save_to_file(Mms* thiz, const char* filename);
void*     mms_save_to_buffer(Mms* thiz, size_t* length);
Mms*      mms_create_from_buffer(void* buffer, size_t length, int callee_release);
Mms*      mms_create_from_file(const char* filename);

/*misc*/
MmsResult mms_fix_nokia(Mms* thiz);

MMS_END_DECLS

#endif/*MMS_H*/

