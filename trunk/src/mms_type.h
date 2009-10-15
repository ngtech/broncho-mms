/*
 * File:    mms_type.h
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


#ifndef MMS_TYPE_H
#define MMS_TYPE_H

#ifdef   LINUX
#include "mms_linux.h"
#elif defined(TW)
#include "mms_tw.h"
#elif defined(WIN32)
#include "mms_win32.h"
#else
#error "unknown platform"
#endif/**/

#ifdef  __cplusplus
# define MMS_BEGIN_DECLS  extern "C" {
# define MMS_END_DECLS    }
#else
# define MMS_BEGIN_DECLS
# define MMS_END_DECLS
#endif

#define mms_log_expr(p) MMS_PRINTF("%s(%d): "#p" failed.\n", __FILE__, __LINE__);
#define mms_return_if_fail(p)  if(!(p)) { mms_log_expr(p); return;}
#define mms_return_val_if_fail(p, ret); if(!(p)) { mms_log_expr(p); return (ret);}

typedef enum _MmsResult
{
	MMS_RESULT_NONE,
	MMS_RESULT_OK,
	MMS_RESULT_FAIL,
	MMS_RESULT_NR
}MmsResult;

typedef enum _MmsDataType
{
	MMS_TYPE_NONE,
	MMS_TYPE_LONG_INT,
	MMS_TYPE_STRING,
	MMS_TYPE_ENCODED_STRING,
	MMS_TYPE_TIME,
	MMS_TYPE_DATA,
	MMS_TYPE_NR
}MmsDataType;

typedef struct _TimeValue
{
	int rel_abs;
	unsigned long time;
}TimeValue;

typedef struct _EncodedString
{
	int    charset;
	char*  string;
}EncodedString;

typedef int         (*mms_compare_func)(void* data1, void* data2);
typedef void        (*mms_destroy_func)(void* data);

typedef struct _HeaderItem
{
	char* name;

	MmsDataType type;
	union
	{
		unsigned long long_int;
		char*         string;

		struct _Data
		{
			void* data;
			mms_destroy_func destroy;
		}data;
		
		TimeValue     time;
		EncodedString encoded_string;
	}value;
	char* value_desc;
	int   offset;
}HeaderItem;

typedef struct _NameIdPair
{
	char* name;
	int   id;
}NameIdPair;

struct _MmsInputBuffer;
typedef struct _MmsInputBuffer MmsInputBuffer;

struct _MmsOutputBuffer;
typedef struct _MmsOutputBuffer MmsOutputBuffer;

typedef MmsResult   (*mms_visit_func)(void* data, void* user_data);
typedef HeaderItem* (*header_unpack_item_func)(MmsInputBuffer* input);
typedef MmsResult   (*header_pack_item_func)(HeaderItem* item, MmsOutputBuffer* output);

#define         MMS_VALUE_LENGTH 1024
#define         MMS_MAX_PATH     260
#define         MMS_NAME_LENGTH  64
#define         MMS_VERSION      0x11
#define         MMS_CONTENT_TYPE "Content-Type"
#define         MMS_SMIL_NAME    "<smil>"
#define         MMS_USER_AGENT   "Dopod838/arcsoft-mmsua-3.0.0.xx"

#define SMIL_TO_PERCENT(n)       (0x10000 | (n))
#define SMIL_FROM_PERCENT(n)     (0xffff & (n))
#define SMIL_IS_PERCENT(n)       (n > 0xffff)

#endif/*MMS_TYPE_H*/

