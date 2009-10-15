/*
 * File:    mms_transaction.h
 * Author:  Li XianJing <lixianjing@broncho.cn>
 * Brief:   a simple mms client.
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


#ifndef MMS_TRANSACTION_H
#define MMS_TRANSACTION_H

#include "mms.h"

MMS_BEGIN_DECLS

struct _MmsTransaction;
typedef struct _MmsTransaction MmsTransaction;

MmsTransaction* mms_transaction_create(char* gateway, int gateway_port, char* mmsc, int mmsc_port, int timeout_usec);
void            mms_transaction_destroy(MmsTransaction* thiz);

MmsResult mms_transaction_set_user_agent(MmsTransaction* thiz, char* user_agent);
MmsResult mms_transaction_set_header(MmsTransaction* thiz, char* name, char* value);

MmsResult mms_transaction_send(MmsTransaction* thiz, char* url, Mms* mms, MmsHeader** resp_header);
MmsResult mms_transaction_send_buffer(MmsTransaction* thiz, char* url, void* buffer, size_t length, MmsHeader** resp_header);
MmsResult mms_transaction_receive(MmsTransaction* thiz, char* url, Mms** mms);
MmsResult mms_transaction_send_file(MmsTransaction* thiz, char* url, char* filename, MmsHeader** resp_header);
int       mms_transaction_get_resp_code(MmsTransaction* thiz);

MMS_END_DECLS

#endif/*MMS_TRANSACTION_H*/

