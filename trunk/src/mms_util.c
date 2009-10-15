/*
 * File:    mms_util.c
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

#include "mms_util.h"
#include "mms_header_codec.h"
#include "mms_content_type.h"
#include "mms_charset.h"

int         mms_name_to_id(const NameIdPair* pairs, const char* name)
{
	int i = 0;
	mms_return_val_if_fail(pairs != NULL && name != NULL, -1);

	for(i = 0; pairs[i].name != NULL; i++)
	{
		if(pairs[i].name == name || strcmp(pairs[i].name, name) == 0)
		{
			return pairs[i].id;
		}
	}

	MMS_PRINTF("%s(%d): mms_name_to_id: %s failed.\n", __FILE__, __LINE__, name);
	
	return -1;
}

const char* mms_id_to_name(const NameIdPair* pairs, int id)
{
	int i = 0;
	mms_return_val_if_fail(pairs != NULL, NULL);

	for(i = 0; pairs[i].name != NULL; i++)
	{
		if(pairs[i].id == id)
		{
			return pairs[i].name;
		}
	}

	return NULL;
}

HeaderItem* mms_header_item_long_create(const char* name,    unsigned long value)
{
	HeaderItem* item = NULL;
	mms_return_val_if_fail(name != NULL, NULL);
	
	if((item = (HeaderItem*)mms_calloc(1, sizeof(HeaderItem))) != NULL)
	{
		item->name = (char*)name;
		item->type = MMS_TYPE_LONG_INT;
		item->value.long_int = value;
	}

	return item;
}

HeaderItem* mms_header_item_time_create(const char* name,   int rel_abs, unsigned long value)
{
	HeaderItem* item = NULL;
	mms_return_val_if_fail(name != NULL, NULL);
	
	if((item = (HeaderItem*)mms_calloc(1, sizeof(HeaderItem))) != NULL)
	{
		item->name = (char*)name;
		item->type = MMS_TYPE_TIME;
		item->value.time.time = value;
		item->value.time.rel_abs = rel_abs;
	}

	return item;
}

HeaderItem* mms_header_item_string_create(const char* name, const char*   value)
{
	HeaderItem* item = NULL;
	mms_return_val_if_fail(name != NULL, NULL);
	
	if((item = (HeaderItem*)mms_calloc(1, sizeof(HeaderItem))) != NULL)
	{
		item->name = (char*)name;
		item->type = MMS_TYPE_STRING;
		item->value.string = mms_strdup(value);
	}

	return item;
}

HeaderItem* mms_header_item_encoded_string_create(const char* name, void* value, int charset)
{
	HeaderItem* item = NULL;
	mms_return_val_if_fail(name != NULL && value != NULL && charset > 0, NULL);
	
	if((item = (HeaderItem*)mms_calloc(1, sizeof(HeaderItem))) != NULL)
	{
		item->name = (char*)name;
		item->type = MMS_TYPE_ENCODED_STRING;

		item->value.encoded_string.charset = charset;
		item->value.encoded_string.string  = mms_strdup(value);
	}

	return item;
}

HeaderItem* mms_header_item_data_create(const char* name,  void*   value, mms_destroy_func destroy)
{
	HeaderItem* item = NULL;
	mms_return_val_if_fail(name != NULL, NULL);
	
	if((item = (HeaderItem*)mms_calloc(1, sizeof(HeaderItem))) != NULL)
	{
		item->name = (char*)name;
		item->type = MMS_TYPE_DATA;
		item->value.data.data = value;
		item->value.data.destroy = destroy;
	}

	return item;
}

void        mms_header_item_destroy(HeaderItem* item)
{
	if(item != NULL)
	{
		if(item->type == MMS_TYPE_DATA && item->value.data.destroy != NULL)
		{
			item->value.data.destroy(item->value.data.data);
			item->value.data.data = NULL;
		}

		if(item->type == MMS_TYPE_STRING && item->value.string != NULL)
		{
			mms_free(item->value.string);
			item->value.string = NULL;
		}
	
		if(item->type == MMS_TYPE_ENCODED_STRING && item->value.encoded_string.string != NULL)
		{
			mms_free(item->value.encoded_string.string);
			item->value.encoded_string.string = NULL;
		}
		mms_free(item);
	}

	return;
}

char* mms_read_binary_file(const char* filename, size_t* length_ptr)
{
	char*   buffer = NULL;
	size_t  length = 0;
	MMS_FILE*   fp = mms_fopen(filename, "rb");
	
	if(fp != NULL)
	{
		mms_fseek(fp, 0, SEEK_END);
		length = mms_ftell(fp);
		mms_fseek(fp, 0, SEEK_SET);
		buffer = (char*)mms_malloc(length + 1);
		mms_fread(buffer, length, 1, fp);
		mms_fclose(fp);
	}
	
	if(length_ptr != NULL)
	{
		*length_ptr = length;
	}
	
	return buffer;
}

MmsResult   mms_write_binary_file(const char* filename, void* data, size_t length)
{
	MMS_FILE* fp = NULL;
	mms_return_val_if_fail(filename != NULL && data != NULL, MMS_RESULT_FAIL);

	if((fp = mms_fopen(filename, "wb+")) != NULL)
	{
		mms_fwrite(data, length, 1, fp);
		mms_fclose(fp);
	}
	
	return MMS_RESULT_OK;
}

#include <stdio.h>
MmsResult   mms_header_item_dump(HeaderItem* item, void* unused)
{
	mms_return_val_if_fail(item != NULL, MMS_RESULT_FAIL);

	MMS_PRINTF("0x%04x %s", item->offset, item->name);
	switch(item->type)
	{
		case MMS_TYPE_STRING:
		{
			MMS_PRINTF(": %s", item->value.string);
			break;
		}
		case MMS_TYPE_ENCODED_STRING:
		{
			MMS_PRINTF(": (%s) %s", 
				mms_charset_id_to_name(item->value.encoded_string.charset),
				item->value.encoded_string.string);
			break;
		}
		case MMS_TYPE_LONG_INT:
		{
			MMS_PRINTF(": %d %s", item->value.long_int, item->value_desc);
			break;
		}
		case MMS_TYPE_TIME:
		{
			MMS_PRINTF(": %d %d", item->value.time.rel_abs, item->value.time.time);
			break;
		}
		default:
		{
			if(mms_content_type_is_content_type(item->name))
			{
				mms_content_type_dump(item->value.data.data, NULL);
			}
			break;
		}
		
	}
	MMS_PRINTF("\n");

	return MMS_RESULT_OK;
}

typedef struct _ExtType
{
	char* ext;
	char* type;
}ExtType;

static const ExtType g_ext_type[] =
{
	{"amr", "audio/amr"},
	{"mid",   "audio/midi"},
	{"midi",  "audio/midi"},
	{"wav",   "audio/x-wav"},
	{"pcm",   "audio/x-wav"},
	{"mp3",   "audio/mpeg"},
	{"gif",   "image/gif"},
	{"jpe",   "image/jpeg"},
	{"jpg",   "image/jpeg"},
	{"jpeg",  "image/jpeg"},
	{"tiff",   "image/tiff"},
	{"tif",   "image/tiff"},
	{"png",   "image/png"},
	{"bmp",	  "image/bmp"},
	{"wbmp",  "image/vnd.wap.wbmp"},
	{"htm",   "text/html"},
	{"html",  "text/html"},
	{"wml",   "application/wml+xml"},
	{"xml",   "text/xml"},
	{"xhtml", "application/xhtml+xml"},
	{"c",     "text/plain"},
	{"h",     "text/plain"},
	{"txt",   "text/plain"},
	{"cpp",   "text/plain"},
	{"vcal",  "text/x-vCalendar"},
	{"ics",   "text/x-vCalendar"},
	{"vcs",   "text/x-vCalendar"},
	{"vcard", "text/x-vCard"},
	{"vcf",   "text/x-vCard"},
	{"pdf",   "application/pdf"},
	{"doc",   "application/msword"},
	{"xls",   "application/vnd.ms-excel"},
	{"3gpp",  "video/3gpp"},
	{"3gpp2", "video/3gpp2"},
	{"avi",   "video/x-msvideo"},
	{"mpeg",  "video/mpeg"},
	{"mp4",   "video/mp4"},
	{"mms",   "application/vnd.wap.mms-message"},
	{NULL,    NULL}
};

const char* mms_get_file_content_type(const char* filename)
{
	int i = 0;
	const char* ext = mms_get_file_ext_name(filename);
	mms_return_val_if_fail(ext != NULL, "unknowmn");

	for(i = 0; g_ext_type[i].ext != NULL; i++)
	{
		if(strcmp(g_ext_type[i].ext, ext) == 0)
		{
			return g_ext_type[i].type;
		}
	}
	
	return "unknown";
}

const char* mms_get_file_ext_name(const char* filename)
{
	char* ext = NULL;
	mms_return_val_if_fail(filename != NULL, NULL);
	
	ext = strrchr(filename, '.');

	if(ext != NULL)
	{
		++ext;
	}

	return ext;
}

const char* mms_get_file_name(const char* filename)
{
	const char* name = NULL;
	mms_return_val_if_fail(filename != NULL, NULL);

	if((name = strrchr(filename, '/')) == NULL)
	{
		name = strrchr(filename, '\\');
	}
	
	if(name != NULL)
	{
		++name;
	}
	else
	{
		name = filename;
	}
	
	return name;
}

char*       mms_strdup(const char* str)
{
	char *p = NULL;
	mms_return_val_if_fail(str != NULL, NULL);
	
	p = mms_malloc(strlen(str) + 1);
	if (p != NULL)
	{
		strcpy(p, str);
	}

	return p;
}

char*       mms_strndup(const char* str, int length)
{
	char *p = NULL;
	mms_return_val_if_fail(str != NULL, NULL);

	if(length <= 0)
	{
		length = strlen(str);
	}
	
	p = mms_malloc(length + 1);

	if(p != NULL)
	{
		memcpy(p, str, length);
		p[length] = '\0';
	}

	return p;
}

size_t   mms_get_unique_id(void)
{
	static size_t id = 1000;
	
	return ++id;
}

void        mms_get_host_from_url (const char* url, char* host)
{
	const char* p = NULL;
	const char* at = NULL;
	const char* colon = NULL;
	const char* slash = NULL;
	int 		copy_len = 0;

	if ((p = strstr (url, "://")) != NULL)
	{
		url = p + 3;
	}

	if ((at = strchr (url, '@')) != NULL)
	{
		url = at + 1;
	}

	if ((slash = strchr (url, '/')) == NULL)
	{
		slash = strchr (url, ';');
	}
	
	colon = strchr (url, ':');
	if (slash != NULL)
	{
		if (colon != NULL && colon < slash)
		{
			copy_len = colon - url;
		}
		else
		{
			copy_len = slash - url;
		}
	}
	else
	{
		if (colon != NULL)
		{
			copy_len = colon - url;
		}
		else
		{
			copy_len = strlen (url);
		}
	}
	memcpy (host, url, copy_len);
	host[copy_len] = '\0';

	return;
}

#ifdef MMS_UTIL_TEST
#include <assert.h>

static NameIdPair pairs[] = 
{
	{"1", 1},
	{"2", 2},
	{"3", 3},
	{"4", 4},
	{NULL, -1}
};

static void mms_header_item_dump_and_destroy(HeaderItem* item)
{
	mms_header_item_dump(item, NULL);
	
	mms_header_item_destroy(item);
	
	return;
}

int main(int argc, char* argv[])
{
	int i = 0;
	char* str = NULL;
	char host[MMS_MAX_PATH + 1] = {0};
	
	for(i = 0; pairs[i].name != NULL; i++)
	{
		mms_assert(mms_id_to_name(pairs, pairs[i].id) == pairs[i].name);
		mms_assert(mms_name_to_id(pairs, pairs[i].name) == pairs[i].id);
	}

	mms_header_item_dump_and_destroy(mms_header_item_long_create("long", 1234));
	mms_header_item_dump_and_destroy(mms_header_item_string_create("string", "strval"));
	mms_header_item_dump_and_destroy(mms_header_item_encoded_string_create("string", "strval", 106));

	mms_assert(strcmp(mms_get_file_content_type("test.c"), "text/plain") == 0);
	mms_assert(strcmp(mms_get_file_name("test.c"), "test.c") == 0);
	mms_assert(strcmp(mms_get_file_name("/test.c"), "test.c") == 0);
	mms_assert(strcmp(mms_get_file_name("\\test.c"), "test.c") == 0);
	mms_assert(strcmp(mms_get_file_ext_name("\\test.c"), "c") == 0);
	mms_assert(strcmp(mms_get_file_ext_name("\\test.cpp"), "cpp") == 0);

	mms_get_host_from_url ("smtp://topwise_pub;auth=PLAIN:topwise123@smtp.tom.com/;save_sent=0;", host);
	mms_assert(strcmp(host, "smtp.tom.com") == 0);
	mms_get_host_from_url ("smtp://lixianjing@broncho.cn;database=broncho.db", host);
	mms_assert(strcmp(host, "topwisesz.com") == 0);
	mms_get_host_from_url("http://mms.moternet.com:80", host);
	mms_assert(strcmp(host, "mms.moternet.com") == 0);
	mms_get_host_from_url("mms.moternet.com", host);
	mms_assert(strcmp(host, "mms.moternet.com") == 0);
	mms_get_host_from_url("http://211.136.221.115/TJdBRLGEPHtA", host);
	mms_assert(strcmp(host, "211.136.221.115") == 0);

	str = mms_strdup("abcd");
	mms_assert(strcmp(str, "abcd") == 0);
	mms_free(str);
	
	str = mms_strndup("abcd", 2);
	mms_assert(strcmp(str, "ab") == 0);
	mms_free(str);

	return 0;
}
#endif/*MMS_UTIL_TEST*/

