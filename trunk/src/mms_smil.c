/*
 * File:    mms_smil.c
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
#include "mms_smil.h"
#include "mms_list.h"
#include "mms_xml.h"
#include <string.h>
#include "wsp_header_codec.h"
#include "mms_input_buffer.h"
#include "mms_output_buffer.h"

typedef struct _SmilRegion
{
	char name[MMS_NAME_LENGTH + 1];
	int x;
	int y;
	int width;
	int height;
}SmilRegion;

typedef struct _SmilFile
{
	int	 cid_exist;

	char cid[MMS_NAME_LENGTH + 1];
	char filename[MMS_MAX_PATH + 1];
	char region[MMS_NAME_LENGTH + 1];
}SmilFile;

typedef struct _SimlCidFilename
{
	const char* cid;
	const char* filename;
}SimlCidFilename;

typedef struct _SmilPar
{
	int duration;

	SmilFile audio;
	SmilFile image;
	SmilFile text;
}SmilPar;

struct _MmsSmil
{
	MmsHeader* header;

	int root_width;
	int root_height;
	MmsList*   regions;
	MmsList*   pars;

	char   title[MMS_NAME_LENGTH + 1];
	char   author[MMS_NAME_LENGTH + 1];

	void*  data;
	size_t length;

	/*temp var*/
	SmilPar* par;
};

int          mms_smil_is_empty(MmsSmil* thiz)
{
	mms_return_val_if_fail(thiz != NULL, 1);

	return thiz->data == NULL && mms_list_length(thiz->regions) == 0 && mms_list_length(thiz->pars) == 0;
}

static char* mms_smil_format_size(char* buffer, size_t size);
static void  mms_smil_on_start(void *user_data, const char *name, const char **atts);
static void  mms_smil_on_end(void *user_data, const char *name);

MmsSmil*     mms_smil_create(void)
{
	MmsSmil* thiz = (MmsSmil*)mms_calloc(1, sizeof(MmsSmil));

	if(thiz != NULL)
	{
		MmsContentType* content_type = mms_content_type_create();
		
		mms_content_type_set_content_type_by_name(content_type, "application/smil");
		mms_content_type_set_param_name(content_type, MMS_SMIL_NAME);
		
		thiz->regions = mms_list_create((mms_destroy_func)mms_free);
		thiz->pars    = mms_list_create((mms_destroy_func)mms_free);

		thiz->header = mms_header_create(wsp_header_unpack_item, wsp_header_pack_item);
		mms_header_set_string(thiz->header, "Content-ID", MMS_SMIL_NAME);
		mms_header_set_string(thiz->header, "Content-Location", "mms.smil");
		mms_header_set_content_type(thiz->header, content_type);
	}
	
	return thiz;
}

void         mms_smil_destroy(MmsSmil* thiz)
{
	if(thiz != NULL)
	{
		mms_header_destroy(thiz->header);
		thiz->header = NULL;
		
		mms_list_destroy(thiz->regions);
		thiz->regions = NULL;

		mms_list_destroy(thiz->pars);
		thiz->pars = NULL;

		mms_free(thiz);
	}

	return;
}

static MmsResult mms_smil_parse_xml(MmsSmil* thiz)
{
	mms_xml_parse(thiz, mms_smil_on_start, mms_smil_on_end, thiz->data, thiz->length);

	return MMS_RESULT_OK;
}

#define SMIL_LINE_LENGTH 255 

static MmsResult mms_smil_region_to_xml(void* data, void* user_data)
{
	char height[32] = {0};
	char width[32]  = {0};
	char left[32]   = {0};
	char top[32]    = {0};
	
	char buffer[SMIL_LINE_LENGTH + 1] = {0};
	SmilRegion* region = (SmilRegion*)data;
	MmsOutputBuffer* output = (MmsOutputBuffer*)user_data;

	mms_output_buffer_put_string_without_null(output, "	<region id=\"");
	mms_output_buffer_put_string_without_null(output, region->name);
	snprintf(buffer, SMIL_LINE_LENGTH, "\" height=\"%s\" width=\"%s\" left=\"%s\" top=\"%s\"/>\n",
		mms_smil_format_size(height, region->height), mms_smil_format_size(width, region->width), 
		mms_smil_format_size(left, region->x), mms_smil_format_size(top, region->y));

	mms_output_buffer_put_string_without_null(output, buffer);

	return MMS_RESULT_OK;
}

static MmsResult mms_smil_par_to_xml(void* data, void* user_data)
{
	char buffer[SMIL_LINE_LENGTH + 1] = {0};
	SmilPar* par = (SmilPar*)data;
	MmsOutputBuffer* output = (MmsOutputBuffer*)user_data;

	snprintf(buffer, SMIL_LINE_LENGTH, "	<par dur=\"%d\">\n", par->duration);
	mms_output_buffer_put_string_without_null(output, buffer);
	
	if(par->image.filename[0])
	{
		mms_output_buffer_put_string_without_null(output, "		<img region=\"");
		mms_output_buffer_put_string_without_null(output, par->image.region);
		mms_output_buffer_put_string_without_null(output, "\" src=\"");
		mms_output_buffer_put_string_without_null(output, par->image.filename);
		mms_output_buffer_put_string_without_null(output, "\" />\n");
	}

	if(par->audio.filename[0])
	{
		mms_output_buffer_put_string_without_null(output, "		<audio region=\"");
		mms_output_buffer_put_string_without_null(output, par->audio.region);
		mms_output_buffer_put_string_without_null(output, "\" src=\"");
		mms_output_buffer_put_string_without_null(output, par->audio.filename);
		mms_output_buffer_put_string_without_null(output, "\" />\n");
	}

	if(par->text.filename[0])
	{
		mms_output_buffer_put_string_without_null(output, "		<text region=\"");
		mms_output_buffer_put_string_without_null(output, par->text.region);
		mms_output_buffer_put_string_without_null(output, "\" src=\"");
		mms_output_buffer_put_string_without_null(output, par->text.filename);
		mms_output_buffer_put_string_without_null(output, "\" />\n");
	}
	
	mms_output_buffer_put_string_without_null(output, "	</par>\n");

	return MMS_RESULT_OK;
}

MmsResult mms_smil_dump_xml(MmsSmil* thiz, MmsOutputBuffer* output)
{
	char buffer[SMIL_LINE_LENGTH + 1] = {0};
	
	mms_output_buffer_put_string_without_null(output, 
		"<smil xmlns=\"http://www.w3.org/2000/SMIL20/CR/Language\">\n");
	mms_output_buffer_put_string_without_null(output, "<head>\n");

	mms_output_buffer_put_string_without_null(output, "	<meta name=\"title\" content=\"");
	if(thiz->title != NULL && thiz->title[0])
	{
		mms_output_buffer_put_string_without_null(output, thiz->title);
	}
	mms_output_buffer_put_string_without_null(output, "\"/>\n");

	snprintf(buffer, SMIL_LINE_LENGTH, "	<meta name=\"author\" content=\"%s\"/>\n", thiz->author);
	mms_output_buffer_put_string_without_null(output, buffer);
	mms_output_buffer_put_string_without_null(output, "	<meta name=\"copyright\" content=\"\"/>\n");
	mms_output_buffer_put_string_without_null(output, "	<meta name=\"abstract\" content=\"\"/>\n");
	
	mms_output_buffer_put_string_without_null(output, "<layout>\n");
	if(thiz->root_width == 0 || thiz->root_height == 0)
	{
		snprintf(buffer, SMIL_LINE_LENGTH, "	<root-layout width=\"100%%\" height=\"100%%\" title=\"%s\"/>\n", 
			thiz->title);
	}
	else
	{
		snprintf(buffer, SMIL_LINE_LENGTH, "	<root-layout width=\"%d\" height=\"%d\" title=\"%s\"/>\n", 
			thiz->root_width, thiz->root_height, thiz->title);
	}
	mms_output_buffer_put_string_without_null(output,  buffer);	
	mms_list_foreach(thiz->regions, (mms_visit_func)mms_smil_region_to_xml, output);

	mms_output_buffer_put_string_without_null(output, "</layout>\n");
	mms_output_buffer_put_string_without_null(output, "</head>\n");
	
	mms_output_buffer_put_string_without_null(output, "<body>\n");
	mms_list_foreach(thiz->pars, (mms_visit_func)mms_smil_par_to_xml, output);
	mms_output_buffer_put_string_without_null(output, "</body>\n");
	mms_output_buffer_put_string_without_null(output, "</smil>\n");

	return MMS_RESULT_OK;
}

MmsResult    mms_smil_unpack(MmsSmil* thiz, MmsInputBuffer* input)
{
	mms_return_val_if_fail(thiz != NULL && input != NULL, MMS_RESULT_FAIL);

	thiz->data   = (void*)mms_input_buffer_get_remaining(input);
	thiz->length = mms_input_buffer_get_remaining_length(input);

	return mms_smil_parse_xml(thiz);
}

static void mms_smil_fix_cid_name (SmilFile* file, SimlCidFilename* cid_name)
{
	char cid_part[MMS_NAME_LENGTH] = {0};
	char* cid_begin = NULL;
	char* cid_end = NULL;
	
	if (file->cid_exist)
	{
		cid_begin = strchr (cid_name->cid, '<');
		if (cid_begin == NULL)
		{
			strncpy (cid_part, cid_name->cid, MMS_NAME_LENGTH);
		}
		else
		{
			cid_end = strchr (cid_name->cid, '>');
			if (cid_end != NULL && cid_end - cid_begin > 0)
			{
				memcpy (cid_part, cid_begin + 1, cid_end - cid_begin - 1);
			}
		}

		if (strcmp (file->cid, cid_part) == 0)
		{
			strncpy(file->filename, cid_name->filename, MMS_MAX_PATH);
		}
	}

	return;
}

static	MmsResult mms_smil_par_fix_filename (SmilPar* par, SimlCidFilename* cid_name)
{
	mms_smil_fix_cid_name (&par->audio, cid_name);
	mms_smil_fix_cid_name (&par->image, cid_name);
	mms_smil_fix_cid_name (&par->text, cid_name);

	return MMS_RESULT_OK;
}

MmsResult    mms_smil_fix_filename_by_cid(MmsSmil* thiz, const char* cid, const char* filename)
{
	SimlCidFilename cid_name = {0};

	cid_name.cid = cid;
	cid_name.filename = filename;
	mms_list_foreach(thiz->pars, (mms_visit_func)mms_smil_par_fix_filename, &cid_name);
	
	return MMS_RESULT_OK;
}

MmsResult    mms_smil_pack(MmsSmil* thiz, MmsOutputBuffer* output)
{
	mms_return_val_if_fail(thiz != NULL && output != NULL, MMS_RESULT_FAIL);

#ifdef MMS_TEST
	if(thiz->data != NULL)
	{
		mms_output_buffer_put_data(output, thiz->data, thiz->length);
	}
	else
	{
		mms_smil_dump_xml(thiz, output);
	}
#else
	mms_smil_dump_xml(thiz, output);
#endif
	

	return MMS_RESULT_OK;
}

MmsResult    mms_smil_pack_with_header(MmsSmil* thiz, MmsOutputBuffer* output)
{
	size_t header_length = 0;
	size_t data_length = 0;
	
	MmsContentType* content_type = NULL;
	MmsOutputBuffer* temp_output = NULL;
	
	mms_return_val_if_fail(thiz != NULL && output != NULL, MMS_RESULT_FAIL);

	content_type = mms_header_get_content_type(thiz->header);
	
	temp_output = mms_output_buffer_create(512);
	mms_content_type_pack(content_type, temp_output);
	mms_header_pack(thiz->header, temp_output);
	header_length = mms_output_buffer_get_buffer_length(temp_output);
	
	mms_smil_pack(thiz, temp_output);
	data_length = mms_output_buffer_get_buffer_length(temp_output) - header_length;

	mms_output_buffer_put_var_int(output, header_length);
	mms_output_buffer_put_var_int(output, data_length);

	mms_output_buffer_put_data(output, mms_output_buffer_get_buffer(temp_output), 
		mms_output_buffer_get_buffer_length(temp_output));

	mms_output_buffer_destroy(temp_output);

	return MMS_RESULT_OK;
}

MmsResult    mms_smil_set_root_region(MmsSmil* thiz, int width, int height)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	thiz->root_width  = width;
	thiz->root_height = height;

	return MMS_RESULT_OK;
}

MmsResult    mms_smil_get_root_region(MmsSmil* thiz, int* width, int* height)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	if(width != NULL)
	{
		*width = thiz->root_width;
	}

	if(height != NULL)
	{
		*height = thiz->root_height;
	}

	return MMS_RESULT_OK;
}

static       int mms_smil_region_compare_by_name(void* data, void* name)
{
	SmilRegion* region = (SmilRegion*)data;

	return strcasecmp(region->name, name);
}

MmsResult    mms_smil_add_region(MmsSmil* thiz, char* name, int x, int y, int width, int height)
{
	SmilRegion* region = NULL;
	mms_return_val_if_fail(thiz != NULL && name != NULL, MMS_RESULT_FAIL);

	if((region = mms_list_find(thiz->regions, (mms_compare_func)mms_smil_region_compare_by_name, (void*)name)) != NULL)
	{
		region->x      = x;
		region->y      = y;
		region->width  = width;
		region->height = height;
	}
	else
	{
		region = (SmilRegion*)mms_calloc(1, sizeof(SmilRegion));

		strncpy(region->name, name, MMS_NAME_LENGTH);
		region->x      = x;
		region->y      = y;
		region->width  = width;
		region->height = height;

		mms_list_append(thiz->regions, region);
	}

	return MMS_RESULT_OK;
}

MmsResult    mms_smil_get_region(MmsSmil* thiz, char* name, int* x, int* y, int* width, int* height)
{
	SmilRegion* region = NULL;
	
	mms_return_val_if_fail(thiz != NULL && name != NULL, MMS_RESULT_FAIL);

	region = mms_list_find(thiz->regions, (mms_compare_func)mms_smil_region_compare_by_name, (void*)name);

	mms_return_val_if_fail(region != NULL, MMS_RESULT_FAIL);

	if(x != NULL)      *x      = region->x;
	if(y != NULL)      *y      = region->y;
	if(width != NULL)  *width  = region->width;
	if(height != NULL) *height = region->height;

	return MMS_RESULT_OK;
}

int          mms_smil_add_par(MmsSmil* thiz, int duration)
{
	SmilPar* par = NULL;

	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	if((par = (SmilPar*)mms_calloc(1, sizeof(SmilPar))) != NULL)
	{
		par->duration = duration;
		mms_list_append(thiz->pars, par);
	}
	else
	{
		return -1;
	}

	return mms_list_length(thiz->pars) - 1;
}

MmsResult mms_smil_erase_par(MmsSmil* thiz, size_t par_index)
{
	SmilPar* par = NULL;

	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	
	par = mms_list_nth(thiz->pars, par_index);
	mms_return_val_if_fail(par != NULL, MMS_RESULT_FAIL);

	mms_list_erase(thiz->pars, NULL, par);

	return MMS_RESULT_OK;
}

size_t mms_smil_get_par_count(MmsSmil* thiz)
{
	SmilPar* par = NULL;
	mms_return_val_if_fail(thiz != NULL, 0);

	return mms_list_length(thiz->pars);
}

MmsResult    mms_smil_par_set_duration(MmsSmil* thiz, size_t par_index, size_t duration)
{
	SmilPar* par = NULL;
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	par = (SmilPar*)mms_list_nth(thiz->pars, par_index);
	mms_return_val_if_fail(par != NULL, 0);
	
	par->duration = duration;

	return MMS_RESULT_OK;
}

int          mms_smil_par_get_duration(MmsSmil* thiz, size_t par_index)
{
	SmilPar* par = NULL;
	mms_return_val_if_fail(thiz != NULL, 0);

	par = (SmilPar*)mms_list_nth(thiz->pars, par_index);
	mms_return_val_if_fail(par != NULL, 0);
	
	return par->duration;
}

MmsResult    mms_smil_par_set_audio(MmsSmil* thiz, size_t par_index, char* region, char* filename)
{
	SmilPar* par = NULL;
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(filename != NULL, MMS_RESULT_FAIL);

	par = (SmilPar*)mms_list_nth(thiz->pars, par_index);
	mms_return_val_if_fail(par != NULL, MMS_RESULT_FAIL);

	if(region != NULL)
	{
		strncpy(par->audio.region, region, MMS_NAME_LENGTH);
	}

	strncpy(par->audio.filename, mms_get_file_name(filename), MMS_MAX_PATH);

	return MMS_RESULT_OK;
}

MmsResult    mms_smil_par_set_image(MmsSmil* thiz, size_t par_index, char* region, char* filename)
{
	SmilPar* par = NULL;
	
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(filename != NULL, MMS_RESULT_FAIL);

	par = (SmilPar*)mms_list_nth(thiz->pars, par_index);
	mms_return_val_if_fail(par != NULL, MMS_RESULT_FAIL);

	if(region != NULL)
	{
		strncpy(par->image.region, region, MMS_NAME_LENGTH);
	}
	strncpy(par->image.filename, mms_get_file_name(filename), MMS_MAX_PATH);

	return MMS_RESULT_OK;
}

MmsResult    mms_smil_par_set_text(MmsSmil* thiz,  size_t par_index, char* region, char* filename)
{
	SmilPar* par = NULL;
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(filename != NULL, MMS_RESULT_FAIL);

	par = (SmilPar*)mms_list_nth(thiz->pars, par_index);
	mms_return_val_if_fail(par != NULL, MMS_RESULT_FAIL);

	if(region != NULL)
	{
		strncpy(par->text.region, region, MMS_NAME_LENGTH);
	}
	strncpy(par->text.filename, mms_get_file_name(filename), MMS_MAX_PATH);

	return MMS_RESULT_OK;
}

MmsResult    mms_smil_par_get_audio(MmsSmil* thiz, size_t par_index, char** region, char** filename)
{
	SmilPar* par = NULL;
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	par = (SmilPar*)mms_list_nth(thiz->pars, par_index);
	mms_return_val_if_fail(par != NULL, MMS_RESULT_FAIL);

	if(region != NULL)
	{
		*region = par->audio.region;
	}

	if(filename != NULL)
	{
		*filename = par->audio.filename;
	}

	return MMS_RESULT_OK;
}

MmsResult    mms_smil_par_get_image(MmsSmil* thiz, size_t par_index, char** region, char** filename)
{
	SmilPar* par = NULL;
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	par = (SmilPar*)mms_list_nth(thiz->pars, par_index);
	mms_return_val_if_fail(par != NULL, MMS_RESULT_FAIL);

	if(region != NULL)
	{
		*region = par->image.region;
	}

	if(filename != NULL)
	{
		*filename = par->image.filename;
	}

	return MMS_RESULT_OK;
}

MmsResult    mms_smil_par_get_text(MmsSmil* thiz,  size_t par_index, char** region, char** filename)
{
	SmilPar* par = NULL;
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	par = (SmilPar*)mms_list_nth(thiz->pars, par_index);
	mms_return_val_if_fail(par != NULL, MMS_RESULT_FAIL);

	if(region != NULL)
	{
		*region = par->text.region;
	}

	if(filename != NULL)
	{
		*filename = par->text.filename;
	}

	return MMS_RESULT_OK;
}

MmsHeader*   mms_smil_get_header(MmsSmil* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);

	return thiz->header;
}

MmsResult    mms_smil_set_header(MmsSmil* thiz, MmsHeader* header)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);
	mms_return_val_if_fail(thiz->header != header, MMS_RESULT_OK);
	
	if(thiz->header != NULL)
	{
		mms_header_destroy(thiz->header);
		thiz->header = NULL;
	}
	
	thiz->header = header;

	return MMS_RESULT_OK;
}

char*  mms_smil_get_title(MmsSmil* thiz)
{
	mms_return_val_if_fail(thiz != NULL, NULL);

	return thiz->title;
}

MmsResult    mms_smil_set_title(MmsSmil* thiz, char* title)
{
	mms_return_val_if_fail(thiz != NULL && title != NULL, MMS_RESULT_FAIL);

	strncpy(thiz->title, title, MMS_NAME_LENGTH);

	return MMS_RESULT_OK;
}

MmsResult    mms_smil_dump(MmsSmil* thiz, void* unused)
{
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	mms_header_dump(thiz->header, NULL);
	
	return MMS_RESULT_OK;
}

static void mms_smil_on_root_layout(MmsSmil* thiz, const char **atts)
{
	int i = 0;
	for(i = 0; atts[i] != NULL; i++)
	{
		if(strcmp(atts[i], "width") == 0)
		{
			i++;
			sscanf(atts[i], "%d", &thiz->root_width);
		}
		else if(strcmp(atts[i], "height") == 0)
		{
			i++;
			sscanf(atts[i], "%d", &thiz->root_height);
		}
	}

	return;
}

static void mms_smil_on_meta(MmsSmil* thiz, const char **atts)
{
	int i = 0;
	
	if(strcmp(atts[1], "title") == 0)
	{
		strncpy(thiz->title, atts[3], MMS_NAME_LENGTH);
	}
	else if(strcmp(atts[1], "author") == 0)
	{
		strncpy(thiz->author, atts[3], MMS_NAME_LENGTH);
	}

	return;
}

static char*  mms_smil_format_size(char* buffer, size_t size)
{
	if(SMIL_IS_PERCENT(size))
	{
		sprintf(buffer, "%u%%", SMIL_FROM_PERCENT(size));
	}
	else
	{
		sprintf(buffer, "%upx", size);
	}

	return buffer;
}

static size_t mms_smil_scanf_size(const char* str)
{
	size_t size = 0;
	mms_return_val_if_fail(str != NULL, 0);
	sscanf(str, "%u", &size);

	if(strrchr(str, '%'))
	{
		size = SMIL_TO_PERCENT(size);
	}

	return size;
}

static void mms_smil_on_region(MmsSmil* thiz, const char **atts)
{
	int i = 0;
	char   id[MMS_NAME_LENGTH + 1] = {0};
	size_t left = 0;
	size_t top = 0;
	size_t width = 0;
	size_t height = 0;

	for(i = 0; atts[i] != NULL; i++)
	{
		if(strcmp(atts[i], "id") == 0)
		{
			i++;
			strncpy(id, atts[i], MMS_NAME_LENGTH);
		}
		else if(strcmp(atts[i], "left") == 0)
		{
			i++;
			left = mms_smil_scanf_size(atts[i]);
		}
		else if(strcmp(atts[i], "top") == 0)
		{
			i++;
			top = mms_smil_scanf_size(atts[i]);
		}
		else if(strcmp(atts[i], "width") == 0)
		{
			i++;
			width = mms_smil_scanf_size(atts[i]);
		}
		else if(strcmp(atts[i], "height") == 0)
		{
			i++;
			height = mms_smil_scanf_size(atts[i]);
		}
	}

	mms_smil_add_region(thiz, id, left, top, width, height);

	return;
}

static void mms_smil_on_par(MmsSmil* thiz, const char **atts)
{
	int i = 0;
	size_t duration = 0;
	for(i = 0; atts[i] != NULL; i++)
	{
		if(strcmp(atts[i], "duration") == 0 || strcmp(atts[i], "dur") == 0)
		{
			i++;
			sscanf(atts[i], "%u", &duration);
		}
	}

	mms_smil_add_par(thiz, duration);

	return;
}

static void mms_smil_get_filename (SmilFile* file, const char* atts)
{
	if (memcmp (atts, "cid:", strlen ("cid:")) == 0)
	{
		snprintf (file->cid, MMS_NAME_LENGTH, "%s", &atts[strlen("cid:")]);
		file->cid_exist = 1;
	}
	else
	{
		strncpy (file->filename, atts, MMS_MAX_PATH);
		file->cid_exist = 0;
	}

	return;
}

static void mms_smil_on_image(MmsSmil* thiz, const char **atts)
{
	int i = 0;
	SmilPar* par = mms_list_last(thiz->pars);
	mms_return_if_fail(par != NULL);

	for(i = 0; atts[i] != NULL; i++)
	{
		if(strcmp(atts[i], "region") == 0)
		{
			i++;
			strncpy(par->image.region, atts[i], MMS_NAME_LENGTH);
		}
		else if(strcmp(atts[i], "src") == 0)
		{
			i++;
			mms_smil_get_filename (&par->image, atts[i]);
			//strncpy(par->image.filename, atts[i], MMS_MAX_PATH);
		}
	}

	return;
}

static void mms_smil_on_text(MmsSmil* thiz, const char **atts)
{
	int i = 0;
	SmilPar* par = mms_list_last(thiz->pars);
	mms_return_if_fail(par != NULL);

	for(i = 0; atts[i] != NULL; i++)
	{
		if(strcmp(atts[i], "region") == 0)
		{
			i++;
			strncpy(par->text.region, atts[i], MMS_NAME_LENGTH);
		}
		else if(strcmp(atts[i], "src") == 0)
		{
			i++;
//			strncpy(par->text.filename, atts[i], MMS_MAX_PATH);
			mms_smil_get_filename (&par->text, atts[i]);
		}
	}

	return;
}

static void mms_smil_on_audio(MmsSmil* thiz, const char **atts)
{
	int i = 0;
	SmilPar* par = mms_list_last(thiz->pars);
	mms_return_if_fail(par != NULL);

	for(i = 0; atts[i] != NULL; i++)
	{
		if(strcmp(atts[i], "region") == 0)
		{
			i++;
			strncpy(par->audio.region, atts[i], MMS_NAME_LENGTH);
		}
		else if(strcmp(atts[i], "src") == 0)
		{
			i++;
			strncpy(par->audio.filename, atts[i], MMS_MAX_PATH);
			mms_smil_get_filename (&par->audio, atts[i]);
		}
	}

	return;
}

typedef void (*mms_smil_on_element_func)(MmsSmil* thiz, const char **atts);

typedef struct _ElementHandler
{
	char* name;
	mms_smil_on_element_func handler;
}ElementHandler;

static ElementHandler g_element_handlers[] =
{
	{"root-layout",  mms_smil_on_root_layout},
	{"meta",         mms_smil_on_meta},
	{"region",       mms_smil_on_region},
	{"par",          mms_smil_on_par},
	{"img",          mms_smil_on_image},
    {"Img",          mms_smil_on_image},
	{"text",         mms_smil_on_text},
    {"Text",         mms_smil_on_text},
	{"audio",        mms_smil_on_audio},
    {"Audio",        mms_smil_on_audio},
	{NULL,           NULL},
};

static void mms_smil_on_start(void *user_data, const char *name, const char **atts)
{
	int i = 0;
	MmsSmil* thiz = (MmsSmil*)user_data;
	mms_return_if_fail(thiz != NULL && name != NULL);

	for(i = 0; g_element_handlers[i].name != NULL; i++)
	{
		if(strcmp(g_element_handlers[i].name, name) == 0)
		{
			g_element_handlers[i].handler(thiz, atts);
		}
	}
	
	return;
}

static void mms_smil_on_end(void *user_data, const char *name)
{
	MmsSmil* thiz = (MmsSmil*)user_data;
	mms_return_if_fail(thiz != NULL && name != NULL);

	return;
}

MmsSmil*         mms_smil_create_from_file(const char* filename)
{
	MmsSmil* thiz = NULL;
	char* buffer = NULL;
	size_t length = 0;
	MmsInputBuffer* input = NULL;
	
	mms_return_val_if_fail(filename != NULL, NULL);

	buffer = mms_read_binary_file(filename, &length);
	mms_return_val_if_fail(buffer != NULL, NULL);
	input  = mms_input_buffer_create(buffer, length);

	thiz = mms_smil_create();
	mms_smil_unpack(thiz, input);
	mms_input_buffer_destroy(input);
	
	/*FIXME: memory leak*/

	return thiz;
}

static       int mms_smil_par_compare_by_filename(void* data, void* filename)
{
	SmilPar* par = (SmilPar*)data;

	mms_return_val_if_fail(par != NULL && filename != NULL, -1);
	
	if(strcmp(par->audio.filename, mms_get_file_name(filename)) == 0
		|| strcmp(par->image.filename, mms_get_file_name(filename)) == 0
		|| strcmp(par->text.filename, mms_get_file_name(filename)) == 0)
	{
		return 0;
	}

	return -1;
}

int          mms_smil_is_file_exist(MmsSmil* thiz, const char* filename)
{
	mms_return_val_if_fail(thiz != NULL && filename != NULL, 0);

	return mms_list_find(thiz->pars, mms_smil_par_compare_by_filename, (void*)filename) != NULL;
}

size_t       mms_smil_get_size(MmsSmil* thiz)
{
	size_t size = 0;
	MmsOutputBuffer* output = NULL;

	mms_return_val_if_fail(thiz != NULL, 0);

	if((output = mms_output_buffer_create(1024)) != NULL)
	{
		mms_smil_pack_with_header(thiz, output);
		size = mms_output_buffer_get_buffer_length(output);
		mms_output_buffer_destroy(output);
	}

	return size;
}

#ifdef MMS_SMIL_TEST
static void test_construct(void)
{
	int x = 0;
	int y = 0;
	int width   = 0;
	int height  = 0;
	MmsOutputBuffer* output = mms_output_buffer_create(512);
	MmsSmil* thiz = mms_smil_create();
	char* region   = NULL;
	char* filename = NULL;
	
	mms_smil_set_root_region(thiz, 240, 320);
	mms_smil_get_root_region(thiz, &width, &height);
	mms_assert(width == 240 && height == 320);

	mms_smil_add_region(thiz, "text", 1, 1, 1, 1);
	mms_smil_add_region(thiz, "audio", 2, 2, 2, 2);
	mms_smil_add_region(thiz, "image", 3, 3, 3, 3);
	
	mms_smil_get_region(thiz, "text", &x, &y, &width, &height);
	mms_assert(x == 1 && y == 1 && width == 1 && height == 1);
	mms_smil_get_region(thiz, "audio", &x, &y, &width, &height);
	mms_assert(x == 2 && y == 2 && width == 2 && height == 2);
	mms_smil_get_region(thiz, "image", &x, &y, &width, &height);
	mms_assert(x == 3 && y == 3 && width == 3 && height == 3);

	mms_smil_add_par(thiz, 100);
	mms_assert(mms_smil_get_par_count(thiz) == 1);
	mms_assert(mms_smil_par_get_duration(thiz, 0) == 100);
	mms_assert(mms_smil_par_set_duration(thiz, 0, 111) == MMS_RESULT_OK);
	mms_assert(mms_smil_par_get_duration(thiz, 0) == 111);
	
	mms_smil_par_set_text(thiz, 0, "text", "text.txt");
	mms_smil_par_get_text(thiz, 0, &region, &filename);
	mms_assert(strcmp(region, "text") == 0 && strcmp(filename, "text.txt") == 0);
	mms_assert(mms_smil_is_file_exist(thiz, filename));

	mms_smil_par_set_audio(thiz, 0, "audio", "audio.mid");
	mms_smil_par_get_audio(thiz, 0, &region, &filename);
	mms_assert(strcmp(region, "audio") == 0 && strcmp(filename, "audio.mid") == 0);
	mms_assert(mms_smil_is_file_exist(thiz, filename));

	mms_smil_par_set_image(thiz, 0, "image", "image.bmp");
	mms_smil_par_get_image(thiz, 0, &region, &filename);
	mms_assert(strcmp(region, "image") == 0 && strcmp(filename, "image.bmp") == 0);
	mms_assert(mms_smil_is_file_exist(thiz, filename));
	
	mms_assert(!mms_smil_is_file_exist(thiz, "lxjabc"));

	mms_smil_pack(thiz, output);

	mms_assert(mms_smil_erase_par(thiz, 0) == MMS_RESULT_OK);
	mms_assert(mms_smil_get_par_count(thiz) == 0);
	
	MMS_PRINTF("%s", mms_output_buffer_get_buffer(output));
	mms_output_buffer_destroy(output);
	
	mms_smil_destroy(thiz);

	return;
}

static void test_from_file(const char* filename)
{
	MmsOutputBuffer* output = mms_output_buffer_create(1024);
	MmsSmil* thiz = mms_smil_create_from_file(filename);

	mms_smil_dump_xml(thiz, output);
	mms_write_binary_file("lxj.xml", mms_output_buffer_get_buffer(output), mms_output_buffer_get_buffer_length(output));
	mms_output_buffer_destroy(output);

	mms_smil_destroy(thiz);

	return ;
}

int main(int argc, char* argv[])
{
	if(argc < 2 || strcmp(argv[1], "--help") == 0)
	{
		MMS_PRINTF("usage: %s test_construct\n", argv[0]);
		MMS_PRINTF("usage: %s test_from_file xml\n", argv[0]);

		return -1;
	}
	
	if(strcmp(argv[1], "test_construct") == 0)
	{
		test_construct();
	}
	else if(strcmp(argv[1], "test_from_file") == 0)
	{
		test_from_file(argv[2]);
	}

	return 0;
}
#endif/*MMS_SMIL_TEST*/

