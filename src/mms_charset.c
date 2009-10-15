/*
 * File:    mms_charset.c
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
 * 2007-01-04 Li XianJing <lixianjing@broncho.cn> created
 *
 */

#include "mms_util.h"
#include "mms_charset.h"

#undef MMS_CHARSET_DEF
#define MMS_CHARSET_DEF(id, value, name) {name, id},
static const NameIdPair g_charsets[] =
{
	MMS_CHARSETS
	{NULL, -1}
};
#undef MMS_CHARSET_DEF

int         mms_charset_name_to_id(const char* name)
{
	return mms_name_to_id(g_charsets, name);
}

const char* mms_charset_id_to_name(int id)
{
	return mms_id_to_name(g_charsets, id);
}

#ifdef MMS_CHARSET_TEST
int main(int argc, char* argv[])
{
	assert(mms_charset_name_to_id("ISO-Unicode-IBM-1261") == 1005);
	assert(mms_charset_name_to_id("Ventura-Math") == 2022);
	assert(mms_charset_name_to_id("UTF-8") == 106);
	assert(mms_charset_name_to_id("GBK") == 113);

	return 0;
}
#endif/*MMS_CHARSET_TEST*/

