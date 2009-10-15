/*
 * File:    mms_body_part.h
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

#ifndef MMS_BODY_PART_H
#define MMS_BODY_PART_H

#include "mms_type.h"
#include "mms_header.h"

MMS_BEGIN_DECLS

struct _MmsBodyPart;
typedef struct _MmsBodyPart MmsBodyPart;

MmsBodyPart* mms_body_part_create(void);
void         mms_body_part_destroy(MmsBodyPart* thiz);

MmsResult    mms_body_part_unpack(MmsBodyPart* thiz, MmsInputBuffer* input);
MmsResult    mms_body_part_pack(MmsBodyPart* thiz, MmsOutputBuffer* output);

MmsHeader*   mms_body_part_get_header(MmsBodyPart* thiz);
MmsResult    mms_body_part_set_header(MmsBodyPart* thiz, MmsHeader* header);
MmsResult    mms_body_part_detach_header(MmsBodyPart* thiz);

MmsResult    mms_body_part_set_name(MmsBodyPart* thiz, const char* name);
MmsResult    mms_body_part_set_data(MmsBodyPart* thiz, void* data, size_t length);

const char*  mms_body_part_get_name(MmsBodyPart* thiz);
const char*  mms_body_part_get_cid(MmsBodyPart* thiz);
const char*  mms_body_part_get_data(MmsBodyPart* thiz);
size_t       mms_body_part_get_data_length(MmsBodyPart* thiz);

int          mms_body_part_compare_by_name(MmsBodyPart* thiz, const char* filename);

MmsResult    mms_body_part_dump(MmsBodyPart* thiz, void* unused);

/*facade*/
const char*  mms_body_part_get_content_type(MmsBodyPart* thiz);
MmsBodyPart* mms_body_part_create_from_file(const char* filename);
MmsBodyPart* mms_body_part_create_from_buffer(const char* filename, void* buffer, size_t length, int callee_release);
MmsResult    mms_body_part_save_to_file(MmsBodyPart* thiz, const char* filename);

MMS_END_DECLS

#endif/*MMS_BODY_PART_H*/

