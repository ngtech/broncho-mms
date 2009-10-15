/*
 * File:    mms_header_codec.h
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

#ifndef MMS_MMS_HEADER_CODEC_H
#define MMS_MMS_HEADER_CODEC_H 

#include "mms_type.h"

#define MMS_HEADERS \
	MMS_HEADER_ITEM_DEF(0,   "undef",                          MMS_HEADER_UNDEF)\
	MMS_HEADER_ITEM_DEF(1,   "Bcc",                            MMS_HEADER_BCC)\
	MMS_HEADER_ITEM_DEF(2,   "Cc",                             MMS_HEADER_CC)\
	MMS_HEADER_ITEM_DEF(3,   "X-Mms-Content-Location",         MMS_HEADER_X_MMS_CONTENT_LOCATION)\
	MMS_HEADER_ITEM_DEF(4,   "Content-Type",                   MMS_HEADER_CONTENT_TYPE)\
	MMS_HEADER_ITEM_DEF(5,   "Date",                           MMS_HEADER_DATE)\
	MMS_HEADER_ITEM_DEF(6,   "X-Mms-Delivery-Report",          MMS_HEADER_X_MMS_DELIVERY_REPORT)\
	MMS_HEADER_ITEM_DEF(7,   "X-Mms-Delivery-Time",            MMS_HEADER_X_MMS_DELIVERY_TIME)\
	MMS_HEADER_ITEM_DEF(8,   "X-Mms-Expiry",                   MMS_HEADER_X_MMS_EXPIRY)\
	MMS_HEADER_ITEM_DEF(9,   "From",                           MMS_HEADER_FROM)\
	MMS_HEADER_ITEM_DEF(10,  "X-Mms-Message-Class",            MMS_HEADER_X_MMS_MESSAGE_CLASS)\
	MMS_HEADER_ITEM_DEF(11,  "Message-ID",                     MMS_HEADER_MESSAGE_ID)\
	MMS_HEADER_ITEM_DEF(12,  "X-Mms-Message-Type",             MMS_HEADER_X_MMS_MESSAGE_TYPE)\
	MMS_HEADER_ITEM_DEF(13,  "X-Mms-MMS-Version",              MMS_HEADER_X_MMS_MMS_VERSION)\
	MMS_HEADER_ITEM_DEF(14,  "X-Mms-Message-Size",             MMS_HEADER_X_MMS_MESSAGE_SIZE)\
	MMS_HEADER_ITEM_DEF(15,  "X-Mms-Priority",                 MMS_HEADER_X_MMS_PRIORITY)\
	MMS_HEADER_ITEM_DEF(16,  "X-Mms-Read-Report",              MMS_HEADER_X_MMS_READ_REPORT)\
	MMS_HEADER_ITEM_DEF(17,  "X-Mms-Report-Allowed",           MMS_HEADER_X_MMS_REPORT_ALLOWED)\
	MMS_HEADER_ITEM_DEF(18,  "X-Mms-Response-Status",          MMS_HEADER_X_MMS_RESPONSE_STATUS)\
	MMS_HEADER_ITEM_DEF(19,  "X-Mms-Response-Text",            MMS_HEADER_X_MMS_RESPONSE_TEXT)\
	MMS_HEADER_ITEM_DEF(20,  "X-Mms-Sender-Visibility",        MMS_HEADER_X_MMS_SENDER_VISIBILITY)\
	MMS_HEADER_ITEM_DEF(21,  "X-Mms-Status",                   MMS_HEADER_X_MMS_STATUS)\
	MMS_HEADER_ITEM_DEF(22,  "Subject",                        MMS_HEADER_SUBJECT)\
	MMS_HEADER_ITEM_DEF(23,  "To",                             MMS_HEADER_TO)\
	MMS_HEADER_ITEM_DEF(24,  "X-Mms-Transaction-Id",           MMS_HEADER_X_MMS_TRANSACTION_ID)\
	MMS_HEADER_ITEM_DEF(25,  "X-Mms-Retrieve-Status",          MMS_HEADER_X_MMS_RETRIEVE_STATUS)\
	MMS_HEADER_ITEM_DEF(26,  "X-Mms-Retrieve-Text",            MMS_HEADER_X_MMS_RETRIEVE_TEXT)\
	MMS_HEADER_ITEM_DEF(27,  "X-Mms-Read-Status",              MMS_HEADER_X_MMS_READ_STATUS)\
	MMS_HEADER_ITEM_DEF(28,  "X-Mms-Reply-Charging",           MMS_HEADER_X_MMS_REPLY_CHARGING)\
	MMS_HEADER_ITEM_DEF(29,  "X-Mms-Reply-Charging-Deadline",  MMS_HEADER_X_MMS_REPLY_CHARGING_DEADLINE)\
	MMS_HEADER_ITEM_DEF(30,  "X-Mms-Reply-Charging-ID",        MMS_HEADER_X_MMS_REPLY_CHARGING_ID)\
	MMS_HEADER_ITEM_DEF(31,  "X-Mms-Reply-Charging-Size",      MMS_HEADER_X_MMS_REPLY_CHARGING_SIZE)\
	MMS_HEADER_ITEM_DEF(32,  "X-Mms-Previously-Sent-By",       MMS_HEADER_X_MMS_PREVIOUSLY_SENT_BY)\
	MMS_HEADER_ITEM_DEF(33,  "X-Mms-Previously-Sent-Date",     MMS_HEADER_X_MMS_PREVIOUSLY_SENT_DATE)

#undef  MMS_HEADER_ITEM_DEF
#define MMS_HEADER_ITEM_DEF(value, name, id) id = value,
enum 
{
	MMS_HEADERS
	MMS_HEADER_NR
};
#undef  MMS_HEADER_ITEM_DEF

#define MMS_VALS \
   MMS_CODEC_DEF(MMS_VAL_ABSOLUTE_TOKEN,                    128,  "Absolute-token")\
   MMS_CODEC_DEF(MMS_VAL_ADDRESS_PRESENT_TOKEN,             128,  "Address-present-token")\
   MMS_CODEC_DEF(MMS_VAL_ADVERTISEMENT,                     129,  "Advertisement")\
   MMS_CODEC_DEF(MMS_VAL_DEFERRED,                          131,  "Deferred")\
   MMS_CODEC_DEF(MMS_VAL_ERROR_CONTENT_NOT_ACCEPTED,        135,  "Error-content-not-accepted")\
   MMS_CODEC_DEF(MMS_VAL_ERROR_MESSAGE_FORMAT_CORRUPT,      131,  "Error-message-format-corrupt")\
   MMS_CODEC_DEF(MMS_VAL_ERROR_MESSAGE_NOT_FOUND,           133,  "Error-message-not-found")\
   MMS_CODEC_DEF(MMS_VAL_ERROR_NETWORK_PROBLEM,             134,  "Error-network-problem")\
   MMS_CODEC_DEF(MMS_VAL_ERROR_SENDING_ADDRESS_UNRESOLVED,  132,  "Error-sending-address-unresolved")\
   MMS_CODEC_DEF(MMS_VAL_ERROR_SERVICE_DENIED,              130,  "Error-service-denied")\
   MMS_CODEC_DEF(MMS_VAL_ERROR_UNSPECIFIED,                 129,  "Error-unspecified")\
   MMS_CODEC_DEF(MMS_VAL_ERROR_UNSUPPORTED_MESSAGE,         136,  "Error-unsupported-message")\
   MMS_CODEC_DEF(MMS_VAL_EXPIRED,                           128,  "Expired")\
   MMS_CODEC_DEF(MMS_VAL_HIDE,                              128,  "Hide")\
   MMS_CODEC_DEF(MMS_VAL_HIGH,                              130,  "High")\
   MMS_CODEC_DEF(MMS_VAL_INFORMATIONAL,                     130,  "Informational")\
   MMS_CODEC_DEF(MMS_VAL_INSERT_ADDRESS_TOKEN,              129,  "Insert-address-token")\
   MMS_CODEC_DEF(MMS_VAL_LOW,                               128,  "Low")\
   MMS_CODEC_DEF(MMS_VAL_M_ACKNOWLEDGE_IND,                 133,  "m-acknowledge-ind")\
   MMS_CODEC_DEF(MMS_VAL_M_DELIVERY_IND,                    134,  "m-delivery-ind")\
   MMS_CODEC_DEF(MMS_VAL_M_NOTIFICATION_IND,                130,  "m-notification-ind")\
   MMS_CODEC_DEF(MMS_VAL_M_NOTIFYRESP_IND,                  131,  "m-notifyresp-ind")\
   MMS_CODEC_DEF(MMS_VAL_M_RETRIEVE_CONF,                   132,  "m-retrieve-conf")\
   MMS_CODEC_DEF(MMS_VAL_M_SEND_CONF,                       129,  "m-send-conf")\
   MMS_CODEC_DEF(MMS_VAL_M_SEND_REQ,                        128,  "m-send-req")\
   MMS_CODEC_DEF(MMS_VAL_NO,                                129,  "No")\
   MMS_CODEC_DEF(MMS_VAL_NORMAL,                            129,  "Normal")\
   MMS_CODEC_DEF(MMS_VAL_OK,                                128,  "Ok")\
   MMS_CODEC_DEF(MMS_VAL_PERSONAL,                          128,  "Personal")\
   MMS_CODEC_DEF(MMS_VAL_REJECTED,                          130,  "Rejected")\
   MMS_CODEC_DEF(MMS_VAL_RELATIVE_TOKEN,                    129,  "Relative-token")\
   MMS_CODEC_DEF(MMS_VAL_RETRIEVED,                         129,  "Retrieved")\
   MMS_CODEC_DEF(MMS_VAL_SHOW,                              129,  "Show")\
   MMS_CODEC_DEF(MMS_VAL_UNRECOGNISED,                      132,  "Unrecognised")\
   MMS_CODEC_DEF(MMS_VAL_YES,                               128,  "Yes") 

#undef MMS_CODEC_DEF
#define MMS_CODEC_DEF(id, value, name) id = value,
enum
{
	MMS_VALS
};
#undef MMS_CODEC_DEF

int              mms_header_name_to_id(const char* name);
const char*      mms_header_id_to_name(int id);
HeaderItem*      mms_header_unpack_item(MmsInputBuffer* input);
MmsResult        mms_header_pack_item(HeaderItem* item, MmsOutputBuffer* output);


#endif/*MMS_MMS_HEADER_CODEC_H*/

