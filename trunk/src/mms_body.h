/*
 * File:    mms_body.h
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


#ifndef MMS_BODY_H
#define MMS_BODY_H

#include "mms_type.h"
#include "mms_smil.h"

MMS_BEGIN_DECLS

struct _MmsBody;
typedef struct _MmsBody MmsBody;

MmsBody*        mms_body_create(void);
void            mms_body_destroy(MmsBody* thiz);

MmsResult       mms_body_unpack(MmsBody* thiz, MmsInputBuffer* input);
MmsResult       mms_body_pack(MmsBody* thiz, MmsOutputBuffer* output);

MmsResult       mms_body_set_smil(MmsBody* thiz, MmsSmil* smil);
MmsSmil*        mms_body_get_smil(MmsBody* thiz);

MmsResult       mms_body_add_body_part(MmsBody* thiz, MmsBodyPart* body_part);
MmsResult       mms_body_erase_body_part(MmsBody* thiz, const char* filename);
MmsBodyPart*    mms_body_find_body_part(MmsBody* thiz, const char* filename);
size_t          mms_body_get_body_part_count(MmsBody* thiz);
MmsResult       mms_body_foreach(MmsBody* thiz, mms_visit_func visit, void* user_data);

MmsResult       mms_body_dump(MmsBody* thiz, void* unused);

MMS_END_DECLS

#endif/*MMS_BODY_H*/

