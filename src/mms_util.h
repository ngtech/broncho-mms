/*
 * File:    mms_util.h
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

#ifndef MMS_UTIL_H
#define MMS_UTIL_H
#include   "mms_type.h"

MMS_BEGIN_DECLS

int         mms_name_to_id(const NameIdPair* pairs, const char* name);
const char* mms_id_to_name(const NameIdPair* pairs, int id);

HeaderItem* mms_header_item_long_create(const char* name,   unsigned long value);
HeaderItem* mms_header_item_time_create(const char* name,   int rel_abs, unsigned long value);
HeaderItem* mms_header_item_string_create(const char* name, const char*   value);
HeaderItem* mms_header_item_encoded_string_create(const char* name, void* value, int charset);
HeaderItem* mms_header_item_data_create(const char* name,  void*   value, mms_destroy_func destroy);
void        mms_header_item_destroy(HeaderItem* item);

MmsResult   mms_header_item_dump(HeaderItem* item, void* /*unused*/);

const char* mms_get_file_content_type(const char* filename);
const char* mms_get_file_ext_name(const char* filename);
const char* mms_get_file_name(const char* filename);

size_t      mms_get_unique_id(void);

char*       mms_strdup(const char* str);
char*       mms_strndup(const char* str, int length);
char*       mms_read_binary_file(const char* filename, size_t* length_ptr);
MmsResult   mms_write_binary_file(const char* filename, void* data, size_t length);

void		mms_get_host_from_url (const char* url, char* host);

MMS_END_DECLS

#endif/*MMS_UTIL_H*/
