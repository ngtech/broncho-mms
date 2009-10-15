/*
 * File:    mms_input_buffer.h
 * Author:  Li XianJing <lixianjing@broncho.cn>
 * Brief:   input buffer
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

#ifndef MMS_INPUT_BUFFER_H
#define MMS_INPUT_BUFFER_H

#include "mms_type.h"

MMS_BEGIN_DECLS

MmsInputBuffer* mms_input_buffer_create(const char* buffer, size_t length);
void            mms_input_buffer_destroy(MmsInputBuffer* thiz);
MmsResult       mms_input_buffer_push_limit(MmsInputBuffer* thiz, size_t length);
MmsResult       mms_input_buffer_skip_to_limit(MmsInputBuffer* thiz);
MmsResult       mms_input_buffer_pop_limit(MmsInputBuffer* thiz);

unsigned char   mms_input_buffer_peek_int8(MmsInputBuffer* thiz);
unsigned char   mms_input_buffer_get_int8(MmsInputBuffer* thiz);
unsigned short  mms_input_buffer_get_int16(MmsInputBuffer* thiz);
unsigned int    mms_input_buffer_get_int32(MmsInputBuffer* thiz);
char*           mms_input_buffer_get_string(MmsInputBuffer* thiz);
char*           mms_input_buffer_get_encoded_string(MmsInputBuffer* thiz, int* charset);
char*           mms_input_buffer_get_string_end_with(MmsInputBuffer* thiz, unsigned char ch);
char*           mms_input_buffer_get_data(MmsInputBuffer* thiz, size_t length);

long            mms_input_buffer_get_var_int(MmsInputBuffer* thiz);
long            mms_input_buffer_get_int(MmsInputBuffer* thiz);
long            mms_input_buffer_get_multi_bytes_int(MmsInputBuffer* thiz, int length);
short           mms_input_buffer_get_short_int(MmsInputBuffer* thiz);
long            mms_input_buffer_get_long_int(MmsInputBuffer* thiz);
long            mms_input_buffer_get_value_length(MmsInputBuffer* thiz);

int             mms_input_buffer_get_remaining_length(MmsInputBuffer* thiz);
const char*     mms_input_buffer_get_remaining(MmsInputBuffer* thiz);

int             mms_input_buffer_is_string(MmsInputBuffer* thiz);
int             mms_input_buffer_is_short_int(MmsInputBuffer* thiz);
int             mms_input_buffer_is_value_length(MmsInputBuffer* thiz);

size_t          mms_input_buffer_tell(MmsInputBuffer* thiz);
MmsResult       mms_input_buffer_seek(MmsInputBuffer* thiz, int offset);
int             mms_input_buffer_is_eof(MmsInputBuffer* thiz);

MMS_END_DECLS

#endif/*MMS_INPUT_BUFFER_H*/

