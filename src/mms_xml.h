/*
 * File:    mms_xml.h
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

#ifndef MMS_XML_H
#define MMS_XML_H

#include "mms_type.h"

MMS_BEGIN_DECLS

typedef void (*mms_xml_start_element_handler_func)(void *user_data, const char *name, const char **atts);
typedef void (*mms_xml_end_element_handler_func)(void *user_data, const char *name);

MmsResult mms_xml_parse(void* user_data, mms_xml_start_element_handler_func on_start, 
	mms_xml_end_element_handler_func on_end, const char* buffer, size_t length);

MMS_END_DECLS

#endif/*MMS_XML_H*/
