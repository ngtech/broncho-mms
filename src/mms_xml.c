/*
 * File:    mms_xml.c
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

#include "mms_xml.h"

#ifdef HAS_EXPAT

#include <expat.h>

MmsResult mms_xml_parse(void* user_data, mms_xml_start_element_handler_func on_start, 
	mms_xml_end_element_handler_func on_end, const char* buffer, size_t length)
{
	XML_Parser parser = XML_ParserCreate(NULL);
	XML_SetUserData(parser, user_data);
	XML_SetElementHandler(parser, on_start, on_end);

	if(XML_Parse(parser, buffer, length, 0) == XML_STATUS_ERROR)
	{
		fprintf(stderr,
			"%s at line %d\n",
			XML_ErrorString(XML_GetErrorCode(parser)),
			XML_GetCurrentLineNumber(parser));
	}

	XML_ParserFree(parser);

	return MMS_RESULT_OK;
}
#else
MmsResult mms_xml_parse(void* user_data, mms_xml_start_element_handler_func on_start, 
	mms_xml_end_element_handler_func on_end, const char* buffer, size_t length)
{
	return MMS_RESULT_OK;
}
#endif

#ifdef MMS_XML_TEST
#include "mms_util.h"

static void on_start(void *user_data, const char *name, const char **atts)
{
	fprintf(user_data, "<%s>\n", name);

	return;
}

static void on_end(void *user_data, const char *name)
{
	fprintf(user_data, "</%s>\n", name);
	
	return;
}

int main(int argc, char* argv[])
{
	char*  buffer = NULL;
	size_t length = 0;
	
	if(argc !=2 )
	{
		MMS_PRINTF("usage: %s xml\n", argv[0]);
		return -1;
	}

	buffer = mms_read_binary_file(argv[1], &length);

	mms_xml_parse(stderr, on_start, on_end, buffer, length);

	return 0;
}
#endif
