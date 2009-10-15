/*
 * File:    mms_content_type.h
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

#ifndef MMS_CONTENT_TYPE_H
#define MMS_CONTENT_TYPE_H

#include "mms_type.h"
#include "mms_input_buffer.h"
#include "mms_output_buffer.h"

#define MMS_CONTENT_TYPE_PARAMS\
	MMS_CODEC_DEF(CONTENT_TYPE_PARAM_Q,            0,   "Q")\
	MMS_CODEC_DEF(CONTENT_TYPE_PARAM_CHARSET,      1,   "Charset")\
	MMS_CODEC_DEF(CONTENT_TYPE_PARAM_LEVEL,        2,   "Level")\
	MMS_CODEC_DEF(CONTENT_TYPE_PARAM_TYPE,         3,   "Type")\
	MMS_CODEC_DEF(CONTENT_TYPE_PARAM_NAME,         5,   "Name")\
	MMS_CODEC_DEF(CONTENT_TYPE_PARAM_FILENAME,     6,   "Filename")\
	MMS_CODEC_DEF(CONTENT_TYPE_PARAM_DIFFERENCES,  6,   "Differences")\
	MMS_CODEC_DEF(CONTENT_TYPE_PARAM_PADDING,      7,   "Padding")\
	MMS_CODEC_DEF(CONTENT_TYPE_PARAM_TYPE_RELATED, 9,   "Type Related")\
	MMS_CODEC_DEF(CONTENT_TYPE_PARAM_START,        10,  "Start")\
	MMS_CODEC_DEF(CONTENT_TYPE_PARAM_START_INFO,   11,  "Start-info")\
	MMS_CODEC_DEF(CONTENT_TYPE_PARAM_COMMENT,      11,  "Comment")\
	MMS_CODEC_DEF(CONTENT_TYPE_PARAM_DOMAIN,       12,  "Domain")\
	MMS_CODEC_DEF(CONTENT_TYPE_PARAM_MAX_AGE,      13,  "Max-Age")\
	MMS_CODEC_DEF(CONTENT_TYPE_PARAM_PATH,         14,  "Path")\
	MMS_CODEC_DEF(CONTENT_TYPE_PARAM_SECURE,       15,  "Secure")

#define MMS_CONTENT_TYPES \
	MMS_CODEC_DEF(CONTENT_TYPE____,                                        0,   "*/*")\
	MMS_CODEC_DEF(CONTENT_TYPE_TEXT__,                                     1,   "text/*")\
	MMS_CODEC_DEF(CONTENT_TYPE_TEXT_HTML,                                  2,   "text/html")\
	MMS_CODEC_DEF(CONTENT_TYPE_TEXT_PLAIN,                                 3,   "text/plain")\
	MMS_CODEC_DEF(CONTENT_TYPE_TEXT_X_HDML,                                4,   "text/x-hdml")\
	MMS_CODEC_DEF(CONTENT_TYPE_TEXT_X_TTML,                                5,   "text/x-ttml")\
	MMS_CODEC_DEF(CONTENT_TYPE_TEXT_X_VCALENDAR,                           6,   "text/x-vCalendar")\
	MMS_CODEC_DEF(CONTENT_TYPE_TEXT_X_VCARD,                               7,   "text/x-vCard")\
	MMS_CODEC_DEF(CONTENT_TYPE_TEXT_VND_WAP_WML,                           8,   "text/vnd.wap.wml")\
	MMS_CODEC_DEF(CONTENT_TYPE_TEXT_VND_WAP_WMLSCRIPT,                     9,   "text/vnd.wap.wmlscript")\
	MMS_CODEC_DEF(CONTENT_TYPE_TEXT_VND_WAP_WTA_EVENT,                     10,  "text/vnd.wap.wta-event")\
	MMS_CODEC_DEF(CONTENT_TYPE_MULTIPART__,                                11,  "multipart/*")\
	MMS_CODEC_DEF(CONTENT_TYPE_MULTIPART_MIXED,                            12,  "multipart/mixed")\
	MMS_CODEC_DEF(CONTENT_TYPE_MULTIPART_FORM_DATA,                        13,  "multipart/form-data")\
	MMS_CODEC_DEF(CONTENT_TYPE_MULTIPART_BYTERANTES,                       14,  "multipart/byterantes")\
	MMS_CODEC_DEF(CONTENT_TYPE_MULTIPART_ALTERNATIVE,                      15,  "multipart/alternative")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION__,                              16,  "application/*")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_JAVA_VM,                        17,  "application/java-vm")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_X_WWW_FORM_URLENCODED,          18,  "application/x-www-form-urlencoded")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_X_HDMLC,                        19,  "application/x-hdmlc")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_WMLC,                   20,  "application/vnd.wap.wmlc")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_WMLSCRIPTC,             21,  "application/vnd.wap.wmlscriptc")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_WTA_EVENTC,             22,  "application/vnd.wap.wta-eventc")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_UAPROF,                 23,  "application/vnd.wap.uaprof")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_WTLS_CA_CERTIFICATE,    24,  "application/vnd.wap.wtls-ca-certificate")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_WTLS_USER_CERTIFICATE,  25,  "application/vnd.wap.wtls-user-certificate")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_X_X509_CA_CERT,                 26,  "application/x-x509-ca-cert")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_X_X509_USER_CERT,               27,  "application/x-x509-user-cert")\
	MMS_CODEC_DEF(CONTENT_TYPE_IMAGE__,                                    28,  "image/*")\
	MMS_CODEC_DEF(CONTENT_TYPE_IMAGE_GIF,                                  29,  "image/gif")\
	MMS_CODEC_DEF(CONTENT_TYPE_IMAGE_JPEG,                                 30,  "image/jpeg")\
	MMS_CODEC_DEF(CONTENT_TYPE_IMAGE_TIFF,                                 31,  "image/tiff")\
	MMS_CODEC_DEF(CONTENT_TYPE_IMAGE_PNG,                                  32,  "image/png")\
	MMS_CODEC_DEF(CONTENT_TYPE_IMAGE_VND_WAP_WBMP,                         33,  "image/vnd.wap.wbmp")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_MULTIPART__,            34,  "application/vnd.wap.multipart.*")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_MULTIPART_MIXED,        35,  "application/vnd.wap.multipart.mixed")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_MULTIPART_FORM_DATA,    36,  "application/vnd.wap.multipart.form-data")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_MULTIPART_BYTERANGES,   37,  "application/vnd.wap.multipart.byteranges")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_MULTIPART_ALTERNATIVE,  38,  "application/vnd.wap.multipart.alternative")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_XML,                            39,  "application/xml")\
	MMS_CODEC_DEF(CONTENT_TYPE_TEXT_XML,                                   40,  "text/xml")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_WBXML,                  41,  "application/vnd.wap.wbxml")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_X_X968_CROSS_CERT,              42,  "application/x-x968-cross-cert")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_X_X968_CA_CERT,                 43,  "application/x-x968-ca-cert")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_X_X968_USER_CERT,               44,  "application/x-x968-user-cert")\
	MMS_CODEC_DEF(CONTENT_TYPE_TEXT_VND_WAP_SI,                            45,  "text/vnd.wap.si")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_SIC,                    46,  "application/vnd.wap.sic")\
	MMS_CODEC_DEF(CONTENT_TYPE_TEXT_VND_WAP_SL,                            47,  "text/vnd.wap.sl")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_SLC,                    48,  "application/vnd.wap.slc")\
	MMS_CODEC_DEF(CONTENT_TYPE_TEXT_VND_WAP_CO,                            49,  "text/vnd.wap.co")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_COC,                    50,  "application/vnd.wap.coc")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_MULTIPART_RELATED,      51,  "application/vnd.wap.multipart.related")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_SIA,                    52,  "application/vnd.wap.sia")\
	MMS_CODEC_DEF(CONTENT_TYPE_TEXT_VND_WAP_CONNECTIVITY_XML,              53,  "text/vnd.wap.connectivity-xml")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_CONNECTIVITY_WBXML,     54,  "application/vnd.wap.connectivity-wbxml")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_PKCS7_MIME,                     55,  "application/pkcs7-mime")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_HASHED_CERTIFICATE,     56,  "application/vnd.wap.hashed-certificate")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_SIGNED_CERTIFICATE,     57,  "application/vnd.wap.signed-certificate")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_CERT_RESPONSE,          58,  "application/vnd.wap.cert-response")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_XHTML_XML,                      59,  "application/xhtml+xml")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_WML_XML,                        60,  "application/wml+xml")\
	MMS_CODEC_DEF(CONTENT_TYPE_TEXT_CSS,                                   61,  "text/css")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_MMS_MESSAGE,            62,  "application/vnd.wap.mms-message")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_ROLLOVER_CERTIFICATE,   63,  "application/vnd.wap.rollover-certificate")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_LOCC_WBXML,             64,  "application/vnd.wap.locc+wbxml")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_LOC_XML,                65,  "application/vnd.wap.loc+xml")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_SYNCML_DM_WBXML,            66,  "application/vnd.syncml.dm+wbxml")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_SYNCML_DM_XML,              67,  "application/vnd.syncml.dm+xml")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_SYNCML_NOTIFICATION,        68,  "application/vnd.syncml.notification")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WAP_XHTML_XML,              69,  "application/vnd.wap.xhtml+xml")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_WV_CSP_CIR,                 70,  "application/vnd.wv.csp.cir")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_OMA_DD_XML,                 71,  "application/vnd.oma.dd+xml")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE,            72,  "application/vnd.oma.drm.message")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_OMA_DRM_CONTENT,            73,  "application/vnd.oma.drm.content")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_XML,         74,  "application/vnd.oma.drm.rights+xml")\
	MMS_CODEC_DEF(CONTENT_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_WBXML,       75,  "application/vnd.oma.drm.rights+wbxml")\

#undef MMS_CODEC_DEF
#define MMS_CODEC_DEF(id, value, name) id = value,
enum
{
	MMS_CONTENT_TYPE_PARAMS	
};

enum
{
	MMS_CONTENT_TYPES
	CONTENT_TYPE_NR
};
#undef MMS_CODEC_DEF

MMS_BEGIN_DECLS

struct _MmsContentType;
typedef struct _MmsContentType MmsContentType;

MmsContentType* mms_content_type_create(void);
void            mms_content_type_destroy(MmsContentType* thiz);

MmsResult       mms_content_type_unpack(MmsContentType* thiz, MmsInputBuffer* input);
MmsResult       mms_content_type_pack(MmsContentType* thiz, MmsOutputBuffer* output);

int             mms_content_type_is_multi_part(MmsContentType* thiz);

MmsResult       mms_content_type_set_content_type_by_id(MmsContentType* thiz, int id);
int             mms_content_type_get_content_type_id(MmsContentType* thiz);

MmsResult       mms_content_type_set_content_type_by_name(MmsContentType* thiz, char* name);

MmsResult       mms_content_type_set_param_name(MmsContentType* thiz, char* name);
MmsResult       mms_content_type_set_param_filename(MmsContentType* thiz, char* filename);
MmsResult       mms_content_type_set_param_type(MmsContentType* thiz, char* type);
MmsResult       mms_content_type_set_param_start(MmsContentType* thiz, char* start);
MmsResult       mms_content_type_set_param_charset(MmsContentType* thiz, char* charset);

const char*     mms_content_type_get_content_type_name(MmsContentType* thiz);

const char*     mms_content_type_get_param_name(MmsContentType* thiz);
const char*     mms_content_type_get_param_filename(MmsContentType* thiz);
const char*     mms_content_type_get_param_type(MmsContentType* thiz);
const char*     mms_content_type_get_param_start(MmsContentType* thiz);
const char*     mms_content_type_get_param_charset(MmsContentType* thiz);

int             mms_content_type_is_content_type(const char* name);

MmsResult       mms_content_type_dump(MmsContentType* thiz, void* unused);

MMS_END_DECLS

#endif/*MMS_CONTENT_TYPE_H*/

