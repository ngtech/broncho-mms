/*
 * File:    wsp_header_codec.h
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

#ifndef WSP_HEADER_CODEC_H
#define WSP_HEADER_CODEC_H

#include "mms_type.h"

#define WSP_HEADERS \
	WSP_HEADER_DEF(WSP_HEADER_ACCEPT,                0x00,  "Accept")\
	WSP_HEADER_DEF(WSP_HEADER_ACCEPT_CHARSET1,       0x01,  "Accept-Charset1")\
	WSP_HEADER_DEF(WSP_HEADER_ACCEPT_ENCODING1,      0x02,  "Accept-Encoding1")\
	WSP_HEADER_DEF(WSP_HEADER_ACCEPT_LANGUAGE,       0x03,  "Accept-Language")\
	WSP_HEADER_DEF(WSP_HEADER_ACCEPT_RANGES,         0x04,  "Accept-Ranges")\
	WSP_HEADER_DEF(WSP_HEADER_AGE,                   0x05,  "Age")\
	WSP_HEADER_DEF(WSP_HEADER_ALLOW,                 0x06,  "Allow")\
	WSP_HEADER_DEF(WSP_HEADER_AUTHORIZATION,         0x07,  "Authorization")\
	WSP_HEADER_DEF(WSP_HEADER_CACHE_CONTROL1,        0x08,  "Cache-Control1")\
	WSP_HEADER_DEF(WSP_HEADER_CONNECTION,            0x09,  "Connection")\
	WSP_HEADER_DEF(WSP_HEADER_CONTENT_BASE1,         0x0A,  "Content-Base1")\
	WSP_HEADER_DEF(WSP_HEADER_CONTENT_ENCODING,      0x0B,  "Content-Encoding")\
	WSP_HEADER_DEF(WSP_HEADER_CONTENT_LANGUAGE,      0x0C,  "Content-Language")\
	WSP_HEADER_DEF(WSP_HEADER_CONTENT_LENGTH,        0x0D,  "Content-Length")\
	WSP_HEADER_DEF(WSP_HEADER_CONTENT_LOCATION,      0x0E,  "Content-Location")\
	WSP_HEADER_DEF(WSP_HEADER_CONTENT_MD5,           0x0F,  "Content-MD5")\
	WSP_HEADER_DEF(WSP_HEADER_CONTENT_RANGE1,        0x10,  "Content-Range1")\
	WSP_HEADER_DEF(WSP_HEADER_CONTENT_TYPE,          0x11,  "Content-Type")\
	WSP_HEADER_DEF(WSP_HEADER_DATE,                  0x12,  "Date")\
	WSP_HEADER_DEF(WSP_HEADER_ETAG,                  0x13,  "Etag")\
	WSP_HEADER_DEF(WSP_HEADER_EXPIRES,               0x14,  "Expires")\
	WSP_HEADER_DEF(WSP_HEADER_FROM,                  0x15,  "From")\
	WSP_HEADER_DEF(WSP_HEADER_HOST,                  0x16,  "Host")\
	WSP_HEADER_DEF(WSP_HEADER_IF_MODIFIED_SINCE,     0x17,  "If-Modified-Since")\
	WSP_HEADER_DEF(WSP_HEADER_IF_MATCH,              0x18,  "If-Match")\
	WSP_HEADER_DEF(WSP_HEADER_IF_NONE_MATCH,         0x19,  "If-None-Match")\
	WSP_HEADER_DEF(WSP_HEADER_IF_RANGE,              0x1A,  "If-Range")\
	WSP_HEADER_DEF(WSP_HEADER_IF_UNMODIFIED_SINCE,   0x1B,  "If-Unmodified-Since")\
	WSP_HEADER_DEF(WSP_HEADER_LOCATION,              0x1C,  "Location")\
	WSP_HEADER_DEF(WSP_HEADER_LAST_MODIFIED,         0x1D,  "Last-Modified")\
	WSP_HEADER_DEF(WSP_HEADER_MAX_FORWARDS,          0x1E,  "Max-Forwards")\
	WSP_HEADER_DEF(WSP_HEADER_PRAGMA,                0x1F,  "Pragma")\
	WSP_HEADER_DEF(WSP_HEADER_PROXY_AUTHENTICATE,    0x20,  "Proxy-Authenticate")\
	WSP_HEADER_DEF(WSP_HEADER_PROXY_AUTHORIZATION,   0x21,  "Proxy-Authorization")\
	WSP_HEADER_DEF(WSP_HEADER_PUBLIC,                0x22,  "Public")\
	WSP_HEADER_DEF(WSP_HEADER_RANGE,                 0x23,  "Range")\
	WSP_HEADER_DEF(WSP_HEADER_REFERER,               0x24,  "Referer")\
	WSP_HEADER_DEF(WSP_HEADER_RETRY_AFTER,           0x25,  "Retry-After")\
	WSP_HEADER_DEF(WSP_HEADER_SERVER,                0x26,  "Server")\
	WSP_HEADER_DEF(WSP_HEADER_TRANSFER_ENCODING,     0x27,  "Transfer-Encoding")\
	WSP_HEADER_DEF(WSP_HEADER_UPGRADE,               0x28,  "Upgrade")\
	WSP_HEADER_DEF(WSP_HEADER_USER_AGENT,            0x29,  "User-Agent")\
	WSP_HEADER_DEF(WSP_HEADER_VARY,                  0x2A,  "Vary")\
	WSP_HEADER_DEF(WSP_HEADER_VIA,                   0x2B,  "Via")\
	WSP_HEADER_DEF(WSP_HEADER_WARNING,               0x2C,  "Warning")\
	WSP_HEADER_DEF(WSP_HEADER_WWW_AUTHENTICATE,      0x2D,  "WWW-Authenticate")\
	WSP_HEADER_DEF(WSP_HEADER_CONTENT_DISPOSITION1,  0x2E,  "Content-Disposition1")\
	WSP_HEADER_DEF(WSP_HEADER_X_WAP_APPLICATION_ID,  0x2F,  "X-Wap-Application-Id")\
	WSP_HEADER_DEF(WSP_HEADER_X_WAP_CONTENT_URI,     0x30,  "X-Wap-Content-URI")\
	WSP_HEADER_DEF(WSP_HEADER_X_WAP_INITIATOR_URI,   0x31,  "X-Wap-Initiator-URI")\
	WSP_HEADER_DEF(WSP_HEADER_ACCEPT_APPLICATION,    0x32,  "Accept-Application")\
	WSP_HEADER_DEF(WSP_HEADER_BEARER_INDICATION,     0x33,  "Bearer-Indication")\
	WSP_HEADER_DEF(WSP_HEADER_PUSH_FLAG,             0x34,  "Push-Flag")\
	WSP_HEADER_DEF(WSP_HEADER_PROFILE,               0x35,  "Profile")\
	WSP_HEADER_DEF(WSP_HEADER_PROFILE_DIFF,          0x36,  "Profile-Diff")\
	WSP_HEADER_DEF(WSP_HEADER_PROFILE_WARNING1,      0x37,  "Profile-Warning1")\
	WSP_HEADER_DEF(WSP_HEADER_EXPECT,                0x38,  "Expect")\
	WSP_HEADER_DEF(WSP_HEADER_TE,                    0x39,  "TE")\
	WSP_HEADER_DEF(WSP_HEADER_TRAILER,               0x3A,  "Trailer")\
	WSP_HEADER_DEF(WSP_HEADER_ACCEPT_CHARSET,        0x3B,  "Accept-Charset")\
	WSP_HEADER_DEF(WSP_HEADER_ACCEPT_ENCODING,       0x3C,  "Accept-Encoding")\
	WSP_HEADER_DEF(WSP_HEADER_CACHE_CONTROL11,       0x3D,  "Cache-Control1")\
	WSP_HEADER_DEF(WSP_HEADER_CONTENT_RANGE,         0x3E,  "Content-Range")\
	WSP_HEADER_DEF(WSP_HEADER_X_WAP_TOD,             0x3F,  "X-Wap-Tod")\
	WSP_HEADER_DEF(WSP_HEADER_CONTENT_ID,            0x40,  "Content-ID")\
	WSP_HEADER_DEF(WSP_HEADER_SET_COOKIE,            0x41,  "Set-Cookie")\
	WSP_HEADER_DEF(WSP_HEADER_COOKIE,                0x42,  "Cookie")\
	WSP_HEADER_DEF(WSP_HEADER_ENCODING_VERSION,      0x43,  "Encoding-Version")\
	WSP_HEADER_DEF(WSP_HEADER_PROFILE_WARNING,       0x44,  "Profile-Warning")\
	WSP_HEADER_DEF(WSP_HEADER_CONTENT_DISPOSITION,   0x45,  "Content-Disposition")\
	WSP_HEADER_DEF(WSP_HEADER_X_WAP_SECURITY,        0x46,  "X-WAP-Security")\
	WSP_HEADER_DEF(WSP_HEADER_CACHE_CONTROL,         0x47,  "Cache-Control")

#undef WSP_HEADER_DEF
#define WSP_HEADER_DEF(id, value, name) id = value,
enum
{
	WSP_HEADERS
};
#undef WSP_HEADER_DEF

int              wsp_header_name_to_id(const char* name);
const char*      wsp_header_id_to_name(int id);
HeaderItem*      wsp_header_unpack_item(MmsInputBuffer* input);
MmsResult        wsp_header_pack_item(HeaderItem* item, MmsOutputBuffer* output);

HeaderItem*      http_header_unpack_item(MmsInputBuffer* input);
MmsResult        http_header_pack_item(HeaderItem* item, MmsOutputBuffer* output);

#endif/*WSP_HEADER_CODEC_H*/	
	
