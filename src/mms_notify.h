/*
 * File:    mms_notify.h
 * Author:  Li XianJing <lixianjing@broncho.cn>
 * Brief:   mms notify
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


#ifndef MMS_NOTIFY_H
#define MMS_NOTIFY_H

#include "mms_type.h"

MMS_BEGIN_DECLS

struct _MmsNotify;
typedef struct _MmsNotify MmsNotify;

MmsNotify*  mms_notify_create(void* buffer, size_t length, int callee_release);
void        mms_notify_destroy(MmsNotify* thiz);

const char* mms_notify_get_url(MmsNotify* thiz, int* charset);
const char* mms_notify_get_transaction_id(MmsNotify* thiz);
const char* mms_notify_get_from(MmsNotify* thiz, int* charset);
const char* mms_notify_get_subject(MmsNotify* thiz, int* charset);
size_t      mms_notify_get_message_size(MmsNotify* thiz);
time_t      mms_notify_get_date(MmsNotify* thiz);
time_t      mms_notify_get_expiry(MmsNotify* thiz);

MmsResult   mms_notify_dump(MmsNotify* thiz, void* unused);

MMS_END_DECLS

#endif/*MMS_NOTIFY_H*/

