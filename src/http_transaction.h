/*
 * File:    http_transaction.h
 * Author:  Li XianJing <lixianjing@broncho.cn>
 * Brief:   a simple http client.
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

#ifndef HTTP_TRANSACTION_H
#define HTTP_TRANSACTION_H

#include "mms_type.h"

MMS_BEGIN_DECLS

struct _HttpTransaction;
typedef struct _HttpTransaction HttpTransaction;

HttpTransaction* http_transaction_create(char* host, int port, int timeout_usec);
void             http_transaction_destroy(HttpTransaction* thiz);

MmsResult  http_transaction_set_user_agent(HttpTransaction* thiz, char* user_agent);
MmsResult  http_transaction_set_header(HttpTransaction* thiz, char* name, char* value);

MmsResult  http_transaction_post_request(HttpTransaction* thiz, char* url, void* data, size_t length);
MmsResult  http_transaction_get_request(HttpTransaction* thiz, char* url);
MmsResult  http_transaction_send_file(HttpTransaction* thiz, const char* filename);
int        http_transaction_get_resp_code(HttpTransaction* thiz);
char*      http_transaction_get_resp_header(HttpTransaction* thiz, char* key);
size_t     http_transaction_get_resp_content_length(HttpTransaction* thiz);
char*      http_transaction_get_resp_content(HttpTransaction* thiz);


MMS_END_DECLS

#endif/*HTTP_TRANSACTION_H*/

