INCLUDES=-DHAS_EXPAT -DLINUX
SUBDIRS=testdata
autotestdir=$(prefix)/test/libware/mms
autotest_PROGRAMS=mms_body_test\
	mms_body_part_test\
	mms_test\
	mms_header_test\
	mms_content_type_test\
	mms_input_buffer_test\
	mms_list_test\
	mms_notify_test\
	wap_push_test\
	mms_output_buffer_test\
	mms_smil_test\
	mms_util_test\
	mms_header_codec_test\
	wsp_header_codec_test\
	mms_xml_test\
	http_transaction_test\
	mms_socket_test\
	mms_transaction_test\
	mms_charset_test

autotest_SCRIPTS=autotest.sh

autotest.sh:
	echo "autotest.sh ok"
	
lib_LTLIBRARIES=libmms.la

libmms_la_SOURCES=\
	http_transaction.c  mms_content_type.c  mms_linux.h          mms_socket.c       mms_xml.c\
	http_transaction.h  mms_content_type.h  mms_list.c           mms_socket.h       mms_xml.h\
	mms_body.c          mms.h               mms_list.h           mms_transaction.c\
	mms_body.h          mms_header.c        mms_notify.c         mms_transaction.h\
	mms_body_part.c     mms_header_codec.c  mms_notify.h         wsp_header_codec.c\
	mms_body_part.h     mms_header_codec.h  mms_output_buffer.c  wsp_header_codec.h\
	mms.c               mms_header.h        mms_output_buffer.h  mms_type.h\
	mms_charset.c       mms_input_buffer.c  mms_smil.c           mms_util.c\
	mms_charset.h       mms_input_buffer.h  mms_smil.h           mms_util.h \
	wap_push.h          wap_push.c

libmms_la_LDFLAGS=-lexpat
libmms_la_CFLAGS=

COMMON_SOURCS=mms_output_buffer.c mms_input_buffer.c mms_util.c mms_list.c mms_xml.c mms_xml.h mms_charset.h mms_charset.c

mms_body_test_SOURCES=mms_body.c $(COMMON_SOURCS) mms_header_codec.c mms_content_type.c mms_header.c\
                      mms_body_part.c wsp_header_codec.c mms_smil.c
mms_body_test_CFLAGS=-DMMS_BODY_TEST
mms_body_test_LDFLAGS=-lexpat

mms_body_part_test_SOURCES=mms_body_part.c $(COMMON_SOURCS) mms_header_codec.c mms_content_type.c mms_header.c \
                           wsp_header_codec.c
mms_body_part_test_CFLAGS=-DMMS_BODY_PART_TEST
mms_body_part_test_LDFLAGS=-lexpat

mms_test_SOURCES=mms.c mms_body.c $(COMMON_SOURCS) mms_header_codec.c mms_content_type.c mms_header.c\
                      mms_body_part.c wsp_header_codec.c mms_smil.c
mms_test_CFLAGS=-DMMS_TEST
mms_test_LDFLAGS=-lexpat

mms_header_test_SOURCES=mms_header.c $(COMMON_SOURCS) mms_header_codec.c mms_content_type.c
mms_header_test_CFLAGS=-DMMS_HEADER_TEST
mms_header_test_LDFLAGS=-lexpat

mms_content_type_test_SOURCES=mms_content_type.c $(COMMON_SOURCS)
mms_content_type_test_CFLAGS=-DMMS_CONTENT_TYPE_TEST
mms_content_type_test_LDFLAGS=-lexpat

mms_input_buffer_test_SOURCES=mms_input_buffer.c 
mms_input_buffer_test_CFLAGS=-DMMS_INPUT_BUFFER_TEST

mms_list_test_SOURCES=mms_list.c
mms_list_test_CFLAGS=-DMMS_LIST_TEST

mms_notify_test_SOURCES=mms_notify.c $(COMMON_SOURCS) mms_header_codec.c mms_content_type.c mms_header.c\
                        wsp_header_codec.c 
mms_notify_test_CFLAGS=-DMMS_NOTIFY_TEST
mms_notify_test_LDFLAGS=-lexpat

wap_push_test_SOURCES=wap_push.c wap_push.h $(COMMON_SOURCS) mms_content_type.c mms_header.c wsp_header_codec.c
wap_push_test_CFLAGS=-DWAP_PUSH_TEST
wap_push_test_LDFLAGS=-lexpat

mms_output_buffer_test_SOURCES=mms_output_buffer.c mms_input_buffer.c
mms_output_buffer_test_CFLAGS=-DMMS_OUTPUT_BUFFER_TEST

mms_smil_test_SOURCES=mms_smil.c $(COMMON_SOURCS) mms_header_codec.c mms_content_type.c mms_header.c\
                     wsp_header_codec.c
mms_smil_test_CFLAGS=-DMMS_SMIL_TEST
mms_smil_test_LDFLAGS=-lexpat

mms_util_test_SOURCES=mms_util.c mms_content_type.c mms_input_buffer.c mms_output_buffer.c \
                      mms_header_codec.c mms_charset.c
mms_util_test_CFLAGS=-DMMS_UTIL_TEST

mms_header_codec_test_SOURCES=mms_header_codec.c $(COMMON_SOURCS) mms_content_type.c
mms_header_codec_test_CFLAGS=-DMMS_HEADER_CODER_TEST
mms_header_codec_test_LDFLAGS=-lexpat

wsp_header_codec_test_SOURCES=wsp_header_codec.c $(COMMON_SOURCS) mms_content_type.c mms_header_codec.c
wsp_header_codec_test_CFLAGS=-DWSP_HEADER_CODEC_TEST
wsp_header_codec_test_LDFLAGS=-lexpat

mms_xml_test_SOURCES=mms_xml.c mms_xml.h mms_output_buffer.c mms_input_buffer.c mms_util.c mms_list.c\
                     mms_header_codec.c mms_content_type.c mms_header.c\
                     wsp_header_codec.c mms_charset.c
mms_xml_test_LDFLAGS=-lexpat
mms_xml_test_CFLAGS=-DMMS_XML_TEST 

http_transaction_test_SOURCES= http_transaction.c wsp_header_codec.c $(COMMON_SOURCS) mms_content_type.c mms_header_codec.c \
                               mms_header.c  mms_socket.c
http_transaction_test_CFLAGS=-DHTTP_TRANSACTION_TEST
http_transaction_test_LDFLAGS=-lexpat

mms_socket_test_SOURCES= mms_socket.c
mms_socket_test_CFLAGS=-DMMS_SOCKET_TEST

mms_transaction_test_SOURCES=mms_transaction.c $(mms_test_SOURCES) http_transaction.c mms_socket.c
mms_transaction_test_CFLAGS=-DMMS_TRANSACTION_TEST
mms_transaction_test_LDFLAGS=-lexpat

mms_charset_test_SOURCES=$(COMMON_SOURCS) mms_content_type.c
mms_charset_test_CFLAGS=-DMMS_CHARSET_TEST
mms_charset_test_LDFLAGS=-lexpat

pkgconfigdir=$(libdir)/pkgconfig
pkgconfig_DATA=mms.pc

mmsincludedir=$(includedir)/mms
mmsinclude_HEADERS=\
	http_transaction.h  mms.h               mms_list.h           mms_transaction.h  mms_xml.h\
	mms_body.h          mms_header_codec.h  mms_notify.h         wsp_header_codec.h\
	mms_body_part.h     mms_header.h        mms_output_buffer.h  mms_type.h\
	mms_charset.h       mms_input_buffer.h  mms_smil.h           mms_util.h\
	mms_content_type.h  mms_linux.h         mms_socket.h         wap_push.h 

EXTRA_DIST=testdata mms.def mms_linux.h  mms_tw.c  mms_tw.h  mms_win32.c  mms_win32.h
