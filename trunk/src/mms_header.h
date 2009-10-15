/*
 * File:    mms_header.h
 * Author:  Li XianJing <lixianjing@broncho.cn>
 * Brief:   mms/wsp/http header.
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

#ifndef MMS_HEADER_H
#define MMS_HEADER_H

#include "mms_type.h"
#include "mms_content_type.h"

MMS_BEGIN_DECLS

struct _MmsHeader;
typedef struct _MmsHeader MmsHeader;

MmsHeader* mms_header_create(header_unpack_item_func unpack, header_pack_item_func pack);
void       mms_header_destroy(MmsHeader* thiz);

MmsResult  mms_header_set_unpack_end_at_content_type(MmsHeader* thiz, int unpack_end_at_content_type);

MmsResult  mms_header_unpack(MmsHeader* thiz, MmsInputBuffer* input);
MmsResult  mms_header_pack(MmsHeader* thiz, MmsOutputBuffer* output);

MmsResult  mms_header_set_string(MmsHeader* thiz, const char* name, const char* value);
MmsResult  mms_header_set_encoded_string(MmsHeader* thiz, const char* name, void* value, int charset);
MmsResult  mms_header_set_long(MmsHeader* thiz, const char* name, unsigned long value);
MmsResult  mms_header_set_time(MmsHeader* thiz, const char* name, int is_relative, unsigned long value);
MmsResult  mms_header_set_content_type(MmsHeader* thiz, MmsContentType* content_type);

MmsResult  mms_header_erase(MmsHeader* thiz, const char* name);

const char*      mms_header_get_string(MmsHeader* thiz, const char* name);
const char*      mms_header_get_encoded_string(MmsHeader* thiz, const char* name, int* charset);
unsigned long    mms_header_get_long(MmsHeader* thiz, const char* name);
unsigned long    mms_header_get_time(MmsHeader* thiz, const char* name, int* is_relative);
MmsContentType*  mms_header_get_content_type(MmsHeader* thiz);

MmsResult        mms_header_dump(MmsHeader* thiz, void* unused);
MmsResult        mms_header_clear(MmsHeader* thiz);

MmsDataType      mms_header_get_item_type(MmsHeader* thiz, const char* name);

MMS_END_DECLS

#endif/*MMS_HEADER_H*/
