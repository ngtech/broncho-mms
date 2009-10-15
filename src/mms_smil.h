/*
 * File:    mms_smil.h
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

#ifndef MMS_SMIL_H
#define MMS_SMIL_H

#include "mms_type.h"
#include "mms_body_part.h"

MMS_BEGIN_DECLS

struct _MmsSmil;
typedef struct _MmsSmil MmsSmil;

MmsSmil*     mms_smil_create(void);
void         mms_smil_destroy(MmsSmil* thiz);

MmsResult    mms_smil_unpack(MmsSmil* thiz, MmsInputBuffer* input);
MmsResult	 mms_smil_fix_filename_by_cid(MmsSmil* thiz, const char* cid, const char* filename);

MmsResult    mms_smil_pack(MmsSmil* thiz, MmsOutputBuffer* output);
MmsResult    mms_smil_pack_with_header(MmsSmil* thiz, MmsOutputBuffer* output);

MmsResult    mms_smil_set_root_region(MmsSmil* thiz, int width, int height);
MmsResult    mms_smil_get_root_region(MmsSmil* thiz, int* width, int* height);

MmsResult    mms_smil_add_region(MmsSmil* thiz, char* name, int x, int y, int width, int height);
MmsResult    mms_smil_get_region(MmsSmil* thiz, char* name, int* x, int* y, int* width, int* height);

int          mms_smil_add_par(MmsSmil* thiz, int duration);
MmsResult    mms_smil_erase_par(MmsSmil* thiz, size_t par_index);
size_t       mms_smil_get_par_count(MmsSmil* thiz);
int          mms_smil_par_get_duration(MmsSmil* thiz, size_t par_index);

MmsResult    mms_smil_par_set_duration(MmsSmil* thiz, size_t par_index, size_t duration);
MmsResult    mms_smil_par_set_audio(MmsSmil* thiz, size_t par_index, char* region, char* filename);
MmsResult    mms_smil_par_set_image(MmsSmil* thiz, size_t par_index, char* region, char* filename);
MmsResult    mms_smil_par_set_text(MmsSmil* thiz,  size_t par_index, char* region, char* filename);
MmsResult    mms_smil_par_get_audio(MmsSmil* thiz, size_t par_index, char** region, char** filename);
MmsResult    mms_smil_par_get_image(MmsSmil* thiz, size_t par_index, char** region, char** filename);
MmsResult    mms_smil_par_get_text(MmsSmil* thiz,  size_t par_index, char** region, char** filename);

char*        mms_smil_get_title(MmsSmil* thiz);
MmsResult    mms_smil_set_title(MmsSmil* thiz, char* title);

MmsHeader*   mms_smil_get_header(MmsSmil* thiz);
MmsResult    mms_smil_set_header(MmsSmil* thiz, MmsHeader* header);

MmsResult    mms_smil_dump(MmsSmil* thiz, void* unused);
MmsResult    mms_smil_dump_xml(MmsSmil* thiz, MmsOutputBuffer* output);

MmsSmil*     mms_smil_create_from_file(const char* filename);

int          mms_smil_is_empty(MmsSmil* thiz);
int          mms_smil_is_file_exist(MmsSmil* thiz, const char* filename);

size_t       mms_smil_get_size(MmsSmil* thiz);

MMS_END_DECLS

#endif/*MMS_SMIL_H*/

