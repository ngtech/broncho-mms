/*
 * File:    mms_list.h
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

#ifndef MMS_LIST_H
#define MMS_LIST_H

#include "mms_type.h"

MMS_BEGIN_DECLS

struct _MmsList;
typedef struct _MmsList MmsList;

MmsList*  mms_list_create(mms_destroy_func destroy);
void      mms_list_destroy(MmsList* thiz); 

MmsResult mms_list_append(MmsList* thiz, void* data);
MmsResult mms_list_prepend(MmsList* thiz, void* data);
MmsResult mms_list_erase(MmsList* thiz, mms_compare_func compare, void* data);
MmsResult mms_list_remove(MmsList* thiz, mms_compare_func compare, void* data, void** ret_data);

size_t    mms_list_length(MmsList* thiz);
void*     mms_list_nth(MmsList* thiz, size_t index);
void*     mms_list_first(MmsList* thiz);
void*     mms_list_last(MmsList* thiz);
void*     mms_list_find(MmsList* thiz, mms_compare_func compare, void* data);

MmsResult mms_list_foreach(MmsList* thiz, mms_visit_func visit, void* context);

MMS_END_DECLS

#endif/*MMS_LIST_H*/

