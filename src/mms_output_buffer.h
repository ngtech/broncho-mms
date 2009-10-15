/*
 * File:    mms_output_buffer.h
 * Author:  Li XianJing <lixianjing@broncho.cn>
 * Brief:   output buffer
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


#ifndef MMS_OUTPUT_BUFFER_H
#define MMS_OUTPUT_BUFFER_H

#include "mms_type.h"

MMS_BEGIN_DECLS

MmsOutputBuffer* mms_output_buffer_create(size_t init_length);
void             mms_output_buffer_destroy(MmsOutputBuffer* thiz);

MmsResult        mms_output_buffer_clear(MmsOutputBuffer* thiz);

MmsResult        mms_output_buffer_put_char(MmsOutputBuffer* thiz, unsigned char data);
MmsResult        mms_output_buffer_put_string(MmsOutputBuffer* thiz, const char* data);
MmsResult        mms_output_buffer_put_encoded_string(MmsOutputBuffer* thiz, const char* data, int charset);
MmsResult        mms_output_buffer_put_string_without_null(MmsOutputBuffer* thiz, const char* data);
MmsResult        mms_output_buffer_put_data(MmsOutputBuffer* thiz, const char* data, size_t data_length);

MmsResult        mms_output_buffer_put_int8(MmsOutputBuffer* thiz, unsigned char data);
MmsResult        mms_output_buffer_put_int16(MmsOutputBuffer* thiz, unsigned short data);
MmsResult        mms_output_buffer_put_int32(MmsOutputBuffer* thiz, unsigned int data);

MmsResult        mms_output_buffer_put_var_int(MmsOutputBuffer* thiz, unsigned long var);
MmsResult        mms_output_buffer_put_int(MmsOutputBuffer* thiz, unsigned long data);
MmsResult        mms_output_buffer_put_multi_bytes_int(MmsOutputBuffer* thiz, unsigned int value);
MmsResult        mms_output_buffer_put_short_int(MmsOutputBuffer* thiz, unsigned long data);
MmsResult        mms_output_buffer_put_long_int(MmsOutputBuffer* thiz, unsigned long data);
MmsResult        mms_output_buffer_put_value_length(MmsOutputBuffer* thiz, unsigned long data);

MmsResult        mms_output_buffer_reset(MmsOutputBuffer* thiz);

unsigned long    mms_output_buffer_get_long_int_bytes(MmsOutputBuffer* thiz, unsigned long data);
unsigned long    mms_output_buffer_get_encoded_string_bytes(MmsOutputBuffer* thiz, const char* data, int charset);

size_t           mms_output_buffer_get_buffer_length(MmsOutputBuffer* thiz);
char*            mms_output_buffer_get_buffer(MmsOutputBuffer* thiz);
char*            mms_output_buffer_detach_buffer(MmsOutputBuffer* thiz);

MmsResult        mms_output_buffer_transaction_begin(MmsOutputBuffer* thiz);
MmsResult        mms_output_buffer_transaction_rollback(MmsOutputBuffer* thiz);
MmsResult        mms_output_buffer_transaction_commit(MmsOutputBuffer* thiz);
size_t           mms_output_buffer_transaction_data_length(MmsOutputBuffer* thiz);

MMS_END_DECLS

#endif/*MMS_OUTPUT_BUFFER_H*/

