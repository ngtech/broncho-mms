/*
 * File:    mms_charset.h
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
 * 2007-01-04 Li XianJing <lixianjing@broncho.cn> created
 *
 */

#ifndef MMS_CHAESET_H

#include "mms_type.h"

#define MMS_CHAESET_H
#define MMS_CHARSETS \
	MMS_CHARSET_DEF(CHARSET_ANSI_X3_4_1968,                  3,     "ANSI_X3.4-1968")\
	MMS_CHARSET_DEF(CHARSET_ISO_10646_UTF_1,                 27,    "ISO-10646-UTF-1")\
	MMS_CHARSET_DEF(CHARSET_ISO_646_BASIC_1983,              28,    "ISO_646.basic:1983")\
	MMS_CHARSET_DEF(CHARSET_INVARIANT,                       29,    "INVARIANT")\
	MMS_CHARSET_DEF(CHARSET_ISO_646_IRV_1983,                30,    "ISO_646.irv:1983")\
	MMS_CHARSET_DEF(CHARSET_BS_4730,                         20,    "BS_4730")\
	MMS_CHARSET_DEF(CHARSET_NATS_SEFI,                       31,    "NATS-SEFI")\
	MMS_CHARSET_DEF(CHARSET_NATS_SEFI_ADD,                   32,    "NATS-SEFI-ADD")\
	MMS_CHARSET_DEF(CHARSET_NATS_DANO,                       33,    "NATS-DANO")\
	MMS_CHARSET_DEF(CHARSET_NATS_DANO_ADD,                   34,    "NATS-DANO-ADD")\
	MMS_CHARSET_DEF(CHARSET_SEN_850200_B,                    35,    "SEN_850200_B")\
	MMS_CHARSET_DEF(CHARSET_SEN_850200_C,                    21,    "SEN_850200_C")\
	MMS_CHARSET_DEF(CHARSET_KS_C_5601_1987,                  36,    "KS_C_5601-1987")\
	MMS_CHARSET_DEF(CHARSET_ISO_2022_KR,                     37,    "ISO-2022-KR")\
	MMS_CHARSET_DEF(CHARSET_EUC_KR,                          38,    "EUC-KR")\
	MMS_CHARSET_DEF(CHARSET_ISO_2022_JP,                     39,    "ISO-2022-JP")\
	MMS_CHARSET_DEF(CHARSET_ISO_2022_JP_2,                   40,    "ISO-2022-JP-2")\
	MMS_CHARSET_DEF(CHARSET_ISO_2022_CN,                     104,   "ISO-2022-CN")\
	MMS_CHARSET_DEF(CHARSET_ISO_2022_CN_EXT,                 105,   "ISO-2022-CN-EXT")\
	MMS_CHARSET_DEF(CHARSET_JIS_C6220_1969_JP,               41,    "JIS_C6220-1969-jp")\
	MMS_CHARSET_DEF(CHARSET_JIS_C6220_1969_RO,               42,    "JIS_C6220-1969-ro")\
	MMS_CHARSET_DEF(CHARSET_IT,                              22,    "IT")\
	MMS_CHARSET_DEF(CHARSET_PT,                              43,    "PT")\
	MMS_CHARSET_DEF(CHARSET_ES,                              23,    "ES")\
	MMS_CHARSET_DEF(CHARSET_GREEK7_OLD,                      44,    "greek7-old")\
	MMS_CHARSET_DEF(CHARSET_LATIN_GREEK,                     45,    "latin-greek")\
	MMS_CHARSET_DEF(CHARSET_DIN_66003,                       24,    "DIN_66003")\
	MMS_CHARSET_DEF(CHARSET_NF_Z_62_010__1973_,              46,    "NF_Z_62-010_(1973)")\
	MMS_CHARSET_DEF(CHARSET_LATIN_GREEK_1,                   47,    "Latin-greek-1")\
	MMS_CHARSET_DEF(CHARSET_ISO_5427,                        48,    "ISO_5427")\
	MMS_CHARSET_DEF(CHARSET_JIS_C6226_1978,                  49,    "JIS_C6226-1978")\
	MMS_CHARSET_DEF(CHARSET_BS_VIEWDATA,                     50,    "BS_viewdata")\
	MMS_CHARSET_DEF(CHARSET_INIS,                            51,    "INIS")\
	MMS_CHARSET_DEF(CHARSET_INIS_8,                          52,    "INIS-8")\
	MMS_CHARSET_DEF(CHARSET_INIS_CYRILLIC,                   53,    "INIS-cyrillic")\
	MMS_CHARSET_DEF(CHARSET_ISO_5427_1981,                   54,    "ISO_5427:1981")\
	MMS_CHARSET_DEF(CHARSET_ISO_5428_1980,                   55,    "ISO_5428:1980")\
	MMS_CHARSET_DEF(CHARSET_GB_1988_80,                      56,    "GB_1988-80")\
	MMS_CHARSET_DEF(CHARSET_GB_2312_80,                      57,    "GB_2312-80")\
	MMS_CHARSET_DEF(CHARSET_NS_4551_1,                       25,    "NS_4551-1")\
	MMS_CHARSET_DEF(CHARSET_NS_4551_2,                       58,    "NS_4551-2")\
	MMS_CHARSET_DEF(CHARSET_NF_Z_62_010,                     26,    "NF_Z_62-010")\
	MMS_CHARSET_DEF(CHARSET_VIDEOTEX_SUPPL,                  59,    "videotex-suppl")\
	MMS_CHARSET_DEF(CHARSET_PT2,                             60,    "PT2")\
	MMS_CHARSET_DEF(CHARSET_ES2,                             61,    "ES2")\
	MMS_CHARSET_DEF(CHARSET_MSZ_7795_3,                      62,    "MSZ_7795.3")\
	MMS_CHARSET_DEF(CHARSET_JIS_C6226_1983,                  63,    "JIS_C6226-1983")\
	MMS_CHARSET_DEF(CHARSET_GREEK7,                          64,    "greek7")\
	MMS_CHARSET_DEF(CHARSET_ASMO_449,                        65,    "ASMO_449")\
	MMS_CHARSET_DEF(CHARSET_ISO_IR_90,                       66,    "iso-ir-90")\
	MMS_CHARSET_DEF(CHARSET_JIS_C6229_1984_A,                67,    "JIS_C6229-1984-a")\
	MMS_CHARSET_DEF(CHARSET_JIS_C6229_1984_B,                68,    "JIS_C6229-1984-b")\
	MMS_CHARSET_DEF(CHARSET_JIS_C6229_1984_B_ADD,            69,    "JIS_C6229-1984-b-add")\
	MMS_CHARSET_DEF(CHARSET_JIS_C6229_1984_HAND,             70,    "JIS_C6229-1984-hand")\
	MMS_CHARSET_DEF(CHARSET_JIS_C6229_1984_HAND_ADD,         71,    "JIS_C6229-1984-hand-add")\
	MMS_CHARSET_DEF(CHARSET_JIS_C6229_1984_KANA,             72,    "JIS_C6229-1984-kana")\
	MMS_CHARSET_DEF(CHARSET_ISO_2033_1983,                   73,    "ISO_2033-1983")\
	MMS_CHARSET_DEF(CHARSET_ANSI_X3_110_1983,                74,    "ANSI_X3.110-1983")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_1_1987,                 4,     "ISO_8859-1:1987")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_2_1987,                 5,     "ISO_8859-2:1987")\
	MMS_CHARSET_DEF(CHARSET_T_61_7BIT,                       75,    "T.61-7bit")\
	MMS_CHARSET_DEF(CHARSET_T_61_8BIT,                       76,    "T.61-8bit")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_3_1988,                 6,     "ISO_8859-3:1988")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_4_1988,                 7,     "ISO_8859-4:1988")\
	MMS_CHARSET_DEF(CHARSET_ECMA_CYRILLIC,                   77,    "ECMA-cyrillic")\
	MMS_CHARSET_DEF(CHARSET_CSA_Z243_4_1985_1,               78,    "CSA_Z243.4-1985-1")\
	MMS_CHARSET_DEF(CHARSET_CSA_Z243_4_1985_2,               79,    "CSA_Z243.4-1985-2")\
	MMS_CHARSET_DEF(CHARSET_CSA_Z243_4_1985_GR,              80,    "CSA_Z243.4-1985-gr")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_6_1987,                 9,     "ISO_8859-6:1987")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_6_E,                    81,    "ISO_8859-6-E")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_6_I,                    82,    "ISO_8859-6-I")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_7_1987,                 10,    "ISO_8859-7:1987")\
	MMS_CHARSET_DEF(CHARSET_T_101_G2,                        83,    "T.101-G2")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_8_1988,                 11,    "ISO_8859-8:1988")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_8_E,                    84,    "ISO_8859-8-E")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_8_I,                    85,    "ISO_8859-8-I")\
	MMS_CHARSET_DEF(CHARSET_CSN_369103,                      86,    "CSN_369103")\
	MMS_CHARSET_DEF(CHARSET_JUS_I_B1_002,                    87,    "JUS_I.B1.002")\
	MMS_CHARSET_DEF(CHARSET_ISO_6937_2_ADD,                  14,    "ISO_6937-2-add")\
	MMS_CHARSET_DEF(CHARSET_IEC_P27_1,                       88,    "IEC_P27-1")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_5_1988,                 8,     "ISO_8859-5:1988")\
	MMS_CHARSET_DEF(CHARSET_JUS_I_B1_003_SERB,               89,    "JUS_I.B1.003-serb")\
	MMS_CHARSET_DEF(CHARSET_JUS_I_B1_003_MAC,                90,    "JUS_I.B1.003-mac")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_9_1989,                 12,    "ISO_8859-9:1989")\
	MMS_CHARSET_DEF(CHARSET_GREEK_CCITT,                     91,    "greek-ccitt")\
	MMS_CHARSET_DEF(CHARSET_NC_NC00_10_81,                   92,    "NC_NC00-10:81")\
	MMS_CHARSET_DEF(CHARSET_ISO_6937_2_25,                   93,    "ISO_6937-2-25")\
	MMS_CHARSET_DEF(CHARSET_GOST_19768_74,                   94,    "GOST_19768-74")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_SUPP,                   95,    "ISO_8859-supp")\
	MMS_CHARSET_DEF(CHARSET_ISO_10367_BOX,                   96,    "ISO_10367-box")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_10,                     13,    "ISO-8859-10")\
	MMS_CHARSET_DEF(CHARSET_LATIN_LAP,                       97,    "latin-lap")\
	MMS_CHARSET_DEF(CHARSET_JIS_X0212_1990,                  98,    "JIS_X0212-1990")\
	MMS_CHARSET_DEF(CHARSET_DS_2089,                         99,    "DS_2089")\
	MMS_CHARSET_DEF(CHARSET_US_DK,                           100,   "us-dk")\
	MMS_CHARSET_DEF(CHARSET_DK_US,                           101,   "dk-us")\
	MMS_CHARSET_DEF(CHARSET_JIS_X0201,                       15,    "JIS_X0201")\
	MMS_CHARSET_DEF(CHARSET_KSC5636,                         102,   "KSC5636")\
	MMS_CHARSET_DEF(CHARSET_ISO_10646_UCS_2,                 1000,  "ISO-10646-UCS-2")\
	MMS_CHARSET_DEF(CHARSET_ISO_10646_UCS_4,                 1001,  "ISO-10646-UCS-4")\
	MMS_CHARSET_DEF(CHARSET_DEC_MCS,                         2008,  "DEC-MCS")\
	MMS_CHARSET_DEF(CHARSET_HP_ROMAN8,                       2004,  "hp-roman8")\
	MMS_CHARSET_DEF(CHARSET_MACINTOSH,                       2027,  "macintosh")\
	MMS_CHARSET_DEF(CHARSET_IBM037,                          2028,  "IBM037")\
	MMS_CHARSET_DEF(CHARSET_IBM038,                          2029,  "IBM038")\
	MMS_CHARSET_DEF(CHARSET_IBM273,                          2030,  "IBM273")\
	MMS_CHARSET_DEF(CHARSET_IBM274,                          2031,  "IBM274")\
	MMS_CHARSET_DEF(CHARSET_IBM275,                          2032,  "IBM275")\
	MMS_CHARSET_DEF(CHARSET_IBM277,                          2033,  "IBM277")\
	MMS_CHARSET_DEF(CHARSET_IBM278,                          2034,  "IBM278")\
	MMS_CHARSET_DEF(CHARSET_IBM280,                          2035,  "IBM280")\
	MMS_CHARSET_DEF(CHARSET_IBM281,                          2036,  "IBM281")\
	MMS_CHARSET_DEF(CHARSET_IBM284,                          2037,  "IBM284")\
	MMS_CHARSET_DEF(CHARSET_IBM285,                          2038,  "IBM285")\
	MMS_CHARSET_DEF(CHARSET_IBM290,                          2039,  "IBM290")\
	MMS_CHARSET_DEF(CHARSET_IBM297,                          2040,  "IBM297")\
	MMS_CHARSET_DEF(CHARSET_IBM420,                          2041,  "IBM420")\
	MMS_CHARSET_DEF(CHARSET_IBM423,                          2042,  "IBM423")\
	MMS_CHARSET_DEF(CHARSET_IBM424,                          2043,  "IBM424")\
	MMS_CHARSET_DEF(CHARSET_IBM437,                          2011,  "IBM437")\
	MMS_CHARSET_DEF(CHARSET_IBM500,                          2044,  "IBM500")\
	MMS_CHARSET_DEF(CHARSET_IBM775,                          2087,  "IBM775")\
	MMS_CHARSET_DEF(CHARSET_IBM850,                          2009,  "IBM850")\
	MMS_CHARSET_DEF(CHARSET_IBM851,                          2045,  "IBM851")\
	MMS_CHARSET_DEF(CHARSET_IBM852,                          2010,  "IBM852")\
	MMS_CHARSET_DEF(CHARSET_IBM855,                          2046,  "IBM855")\
	MMS_CHARSET_DEF(CHARSET_IBM857,                          2047,  "IBM857")\
	MMS_CHARSET_DEF(CHARSET_IBM860,                          2048,  "IBM860")\
	MMS_CHARSET_DEF(CHARSET_IBM861,                          2049,  "IBM861")\
	MMS_CHARSET_DEF(CHARSET_IBM862,                          2013,  "IBM862")\
	MMS_CHARSET_DEF(CHARSET_IBM863,                          2050,  "IBM863")\
	MMS_CHARSET_DEF(CHARSET_IBM864,                          2051,  "IBM864")\
	MMS_CHARSET_DEF(CHARSET_IBM865,                          2052,  "IBM865")\
	MMS_CHARSET_DEF(CHARSET_IBM866,                          2086,  "IBM866")\
	MMS_CHARSET_DEF(CHARSET_IBM868,                          2053,  "IBM868")\
	MMS_CHARSET_DEF(CHARSET_IBM869,                          2054,  "IBM869")\
	MMS_CHARSET_DEF(CHARSET_IBM870,                          2055,  "IBM870")\
	MMS_CHARSET_DEF(CHARSET_IBM871,                          2056,  "IBM871")\
	MMS_CHARSET_DEF(CHARSET_IBM880,                          2057,  "IBM880")\
	MMS_CHARSET_DEF(CHARSET_IBM891,                          2058,  "IBM891")\
	MMS_CHARSET_DEF(CHARSET_IBM903,                          2059,  "IBM903")\
	MMS_CHARSET_DEF(CHARSET_IBM904,                          2060,  "IBM904")\
	MMS_CHARSET_DEF(CHARSET_IBM905,                          2061,  "IBM905")\
	MMS_CHARSET_DEF(CHARSET_IBM918,                          2062,  "IBM918")\
	MMS_CHARSET_DEF(CHARSET_IBM1026,                         2063,  "IBM1026")\
	MMS_CHARSET_DEF(CHARSET_EBCDIC_AT_DE,                    2064,  "EBCDIC-AT-DE")\
	MMS_CHARSET_DEF(CHARSET_EBCDIC_AT_DE_A,                  2065,  "EBCDIC-AT-DE-A")\
	MMS_CHARSET_DEF(CHARSET_EBCDIC_CA_FR,                    2066,  "EBCDIC-CA-FR")\
	MMS_CHARSET_DEF(CHARSET_EBCDIC_DK_NO,                    2067,  "EBCDIC-DK-NO")\
	MMS_CHARSET_DEF(CHARSET_EBCDIC_DK_NO_A,                  2068,  "EBCDIC-DK-NO-A")\
	MMS_CHARSET_DEF(CHARSET_EBCDIC_FI_SE,                    2069,  "EBCDIC-FI-SE")\
	MMS_CHARSET_DEF(CHARSET_EBCDIC_FI_SE_A,                  2070,  "EBCDIC-FI-SE-A")\
	MMS_CHARSET_DEF(CHARSET_EBCDIC_FR,                       2071,  "EBCDIC-FR")\
	MMS_CHARSET_DEF(CHARSET_EBCDIC_IT,                       2072,  "EBCDIC-IT")\
	MMS_CHARSET_DEF(CHARSET_EBCDIC_PT,                       2073,  "EBCDIC-PT")\
	MMS_CHARSET_DEF(CHARSET_EBCDIC_ES,                       2074,  "EBCDIC-ES")\
	MMS_CHARSET_DEF(CHARSET_EBCDIC_ES_A,                     2075,  "EBCDIC-ES-A")\
	MMS_CHARSET_DEF(CHARSET_EBCDIC_ES_S,                     2076,  "EBCDIC-ES-S")\
	MMS_CHARSET_DEF(CHARSET_EBCDIC_UK,                       2077,  "EBCDIC-UK")\
	MMS_CHARSET_DEF(CHARSET_EBCDIC_US,                       2078,  "EBCDIC-US")\
	MMS_CHARSET_DEF(CHARSET_UNKNOWN_8BIT,                    2079,  "UNKNOWN-8BIT")\
	MMS_CHARSET_DEF(CHARSET_MNEMONIC,                        2080,  "MNEMONIC")\
	MMS_CHARSET_DEF(CHARSET_MNEM,                            2081,  "MNEM")\
	MMS_CHARSET_DEF(CHARSET_VISCII,                          2082,  "VISCII")\
	MMS_CHARSET_DEF(CHARSET_VIQR,                            2083,  "VIQR")\
	MMS_CHARSET_DEF(CHARSET_KOI8_R,                          2084,  "KOI8-R")\
	MMS_CHARSET_DEF(CHARSET_KOI8_U,                          2088,  "KOI8-U")\
	MMS_CHARSET_DEF(CHARSET_IBM00858,                        2089,  "IBM00858")\
	MMS_CHARSET_DEF(CHARSET_IBM00924,                        2090,  "IBM00924")\
	MMS_CHARSET_DEF(CHARSET_IBM01140,                        2091,  "IBM01140")\
	MMS_CHARSET_DEF(CHARSET_IBM01141,                        2092,  "IBM01141")\
	MMS_CHARSET_DEF(CHARSET_IBM01142,                        2093,  "IBM01142")\
	MMS_CHARSET_DEF(CHARSET_IBM01143,                        2094,  "IBM01143")\
	MMS_CHARSET_DEF(CHARSET_IBM01144,                        2095,  "IBM01144")\
	MMS_CHARSET_DEF(CHARSET_IBM01145,                        2096,  "IBM01145")\
	MMS_CHARSET_DEF(CHARSET_IBM01146,                        2097,  "IBM01146")\
	MMS_CHARSET_DEF(CHARSET_IBM01147,                        2098,  "IBM01147")\
	MMS_CHARSET_DEF(CHARSET_IBM01148,                        2099,  "IBM01148")\
	MMS_CHARSET_DEF(CHARSET_IBM01149,                        2100,  "IBM01149")\
	MMS_CHARSET_DEF(CHARSET_BIG5_HKSCS,                      2101,  "Big5-HKSCS")\
	MMS_CHARSET_DEF(CHARSET_IBM1047,                         2102,  "IBM1047")\
	MMS_CHARSET_DEF(CHARSET_PTCP154,                         2103,  "PTCP154")\
	MMS_CHARSET_DEF(CHARSET_AMIGA_1251,                      2104,  "Amiga-1251")\
	MMS_CHARSET_DEF(CHARSET_KOI7_SWITCHED,                   2105,  "KOI7-switched")\
	MMS_CHARSET_DEF(CHARSET_UNICODE_1_1,                     1010,  "UNICODE-1-1")\
	MMS_CHARSET_DEF(CHARSET_SCSU,                            1011,  "SCSU")\
	MMS_CHARSET_DEF(CHARSET_UTF_7,                           1012,  "UTF-7")\
	MMS_CHARSET_DEF(CHARSET_UTF_16BE,                        1013,  "UTF-16BE")\
	MMS_CHARSET_DEF(CHARSET_UTF_16LE,                        1014,  "UTF-16LE")\
	MMS_CHARSET_DEF(CHARSET_UTF_16,                          1015,  "UTF-16")\
	MMS_CHARSET_DEF(CHARSET_CESU_8,                          1016,  "CESU-8")\
	MMS_CHARSET_DEF(CHARSET_UTF_32,                          1017,  "UTF-32")\
	MMS_CHARSET_DEF(CHARSET_UTF_32BE,                        1018,  "UTF-32BE")\
	MMS_CHARSET_DEF(CHARSET_UTF_32LE,                        1019,  "UTF-32LE")\
	MMS_CHARSET_DEF(CHARSET_BOCU_1,                          1020,  "BOCU-1")\
	MMS_CHARSET_DEF(CHARSET_UNICODE_1_1_UTF_7,               103,   "UNICODE-1-1-UTF-7")\
	MMS_CHARSET_DEF(CHARSET_UTF_8,                           106,   "UTF-8")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_13,                     109,   "ISO-8859-13")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_14,                     110,   "ISO-8859-14")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_15,                     111,   "ISO-8859-15")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_16,                     112,   "ISO-8859-16")\
	MMS_CHARSET_DEF(CHARSET_GBK,                             113,   "GBK")\
	MMS_CHARSET_DEF(CHARSET_GB18030,                         114,   "GB18030")\
	MMS_CHARSET_DEF(CHARSET_OSD_EBCDIC_DF04_15,              115,   "OSD_EBCDIC_DF04_15")\
	MMS_CHARSET_DEF(CHARSET_OSD_EBCDIC_DF03_IRV,             116,   "OSD_EBCDIC_DF03_IRV")\
	MMS_CHARSET_DEF(CHARSET_OSD_EBCDIC_DF04_1,               117,   "OSD_EBCDIC_DF04_1")\
	MMS_CHARSET_DEF(CHARSET_JIS_ENCODING,                    16,    "JIS_Encoding")\
	MMS_CHARSET_DEF(CHARSET_SHIFT_JIS,                       17,    "Shift_JIS")\
	MMS_CHARSET_DEF(CHARSET_EUC_PACKED_FORMAT_FOR_JP,        18,    "Extended_UNIX_Code_Packed_Format_for_Japanese")\
	MMS_CHARSET_DEF(CHARSET_EUC_FIXED_WIDTH_FOR_JP,          19,    "Extended_UNIX_Code_Fixed_Width_for_Japanese")\
	MMS_CHARSET_DEF(CHARSET_ISO_10646_UCS_BASIC,             1002,  "ISO-10646-UCS-Basic")\
	MMS_CHARSET_DEF(CHARSET_ISO_10646_UNICODE_LATIN1,        1003,  "ISO-10646-Unicode-Latin1")\
	MMS_CHARSET_DEF(CHARSET_ISO_UNICODE_IBM_1261,            1005,  "ISO-Unicode-IBM-1261")\
	MMS_CHARSET_DEF(CHARSET_ISO_UNICODE_IBM_1268,            1006,  "ISO-Unicode-IBM-1268")\
	MMS_CHARSET_DEF(CHARSET_ISO_UNICODE_IBM_1276,            1007,  "ISO-Unicode-IBM-1276")\
	MMS_CHARSET_DEF(CHARSET_ISO_UNICODE_IBM_1264,            1008,  "ISO-Unicode-IBM-1264")\
	MMS_CHARSET_DEF(CHARSET_ISO_UNICODE_IBM_1265,            1009,  "ISO-Unicode-IBM-1265")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_1_WINDOWS_3_0_LATIN_1,  2000,  "ISO-8859-1-Windows-3.0-Latin-1")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_1_WINDOWS_3_1_LATIN_1,  2001,  "ISO-8859-1-Windows-3.1-Latin-1")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_2_WINDOWS_LATIN_2,      2002,  "ISO-8859-2-Windows-Latin-2")\
	MMS_CHARSET_DEF(CHARSET_ISO_8859_9_WINDOWS_LATIN_5,      2003,  "ISO-8859-9-Windows-Latin-5")\
	MMS_CHARSET_DEF(CHARSET_ADOBE_STANDARD_ENCODING,         2005,  "Adobe-Standard-Encoding")\
	MMS_CHARSET_DEF(CHARSET_VENTURA_US,                      2006,  "Ventura-US")\
	MMS_CHARSET_DEF(CHARSET_VENTURA_INTERNATIONAL,           2007,  "Ventura-International")\
	MMS_CHARSET_DEF(CHARSET_PC8_DANISH_NORWEGIAN,            2012,  "PC8-Danish-Norwegian")\
	MMS_CHARSET_DEF(CHARSET_PC8_TURKISH,                     2014,  "PC8-Turkish")\
	MMS_CHARSET_DEF(CHARSET_IBM_SYMBOLS,                     2015,  "IBM-Symbols")\
	MMS_CHARSET_DEF(CHARSET_IBM_THAI,                        2016,  "IBM-Thai")\
	MMS_CHARSET_DEF(CHARSET_HP_LEGAL,                        2017,  "HP-Legal")\
	MMS_CHARSET_DEF(CHARSET_HP_PI_FONT,                      2018,  "HP-Pi-font")\
	MMS_CHARSET_DEF(CHARSET_HP_MATH8,                        2019,  "HP-Math8")\
	MMS_CHARSET_DEF(CHARSET_ADOBE_SYMBOL_ENCODING,           2020,  "Adobe-Symbol-Encoding")\
	MMS_CHARSET_DEF(CHARSET_HP_DESKTOP,                      2021,  "HP-DeskTop")\
	MMS_CHARSET_DEF(CHARSET_VENTURA_MATH,                    2022,  "Ventura-Math")\
	MMS_CHARSET_DEF(CHARSET_MICROSOFT_PUBLISHING,            2023,  "Microsoft-Publishing")\
	MMS_CHARSET_DEF(CHARSET_WINDOWS_31J,                     2024,  "Windows-31J")\
	MMS_CHARSET_DEF(CHARSET_GB2312,                          2025,  "GB2312")\
	MMS_CHARSET_DEF(CHARSET_BIG5,                            2026,  "Big5")\
	MMS_CHARSET_DEF(CHARSET_WINDOWS_1250,                    2250,  "windows-1250")\
	MMS_CHARSET_DEF(CHARSET_WINDOWS_1251,                    2251,  "windows-1251")\
	MMS_CHARSET_DEF(CHARSET_WINDOWS_1252,                    2252,  "windows-1252")\
	MMS_CHARSET_DEF(CHARSET_WINDOWS_1253,                    2253,  "windows-1253")\
	MMS_CHARSET_DEF(CHARSET_WINDOWS_1254,                    2254,  "windows-1254")\
	MMS_CHARSET_DEF(CHARSET_WINDOWS_1255,                    2255,  "windows-1255")\
	MMS_CHARSET_DEF(CHARSET_WINDOWS_1256,                    2256,  "windows-1256")\
	MMS_CHARSET_DEF(CHARSET_WINDOWS_1257,                    2257,  "windows-1257")\
	MMS_CHARSET_DEF(CHARSET_WINDOWS_1258,                    2258,  "windows-1258")\
	MMS_CHARSET_DEF(CHARSET_TIS_620,                         2259,  "TIS-620")\
	MMS_CHARSET_DEF(CHARSET_HZ_GB_2312,                      2085,  "HZ-GB-2312")\
	MMS_CHARSET_DEF(CHARSET_ISO_11548_1,                     118,   "ISO-11548-1")\
	MMS_CHARSET_DEF(CHARSET_KZ_1048,                         119,   "KZ-1048")\
	MMS_CHARSET_DEF(CHARSET_BRF,                             2106,  "BRF")

#undef MMS_CHARSET_DEF
#define MMS_CHARSET_DEF(id, value, name) id = value,
enum {MMS_CHARSETS};
#undef MMS_CHARSET_DEF

int         mms_charset_name_to_id(const char* name);
const char* mms_charset_id_to_name(int id);

#endif/*MMS_CHARSET_H*/

