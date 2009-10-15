/*
 * File:    wap_push.h
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


#ifndef WAP_PUSH_H
#define WAP_PUSH_H

#include "mms_type.h"

MMS_BEGIN_DECLS

struct _WapPush;
typedef struct _WapPush WapPush;

WapPush*              wap_push_create(void* buffer, size_t length, int callee_release);
void                  wap_push_destroy(WapPush* thiz);
MmsResult             wap_push_dump(WapPush* thiz, void* unused);
int                   wap_push_get_push_id(WapPush* thiz);
int                   wap_push_get_pdu_type(WapPush* thiz);
size_t                wap_push_get_content_length(WapPush* thiz);
const char*           wap_push_get_content(WapPush* thiz);
const MmsContentType* wap_push_get_content_type(WapPush* thiz);

MMS_END_DECLS

#endif/*WAP_PUSH_H*/

