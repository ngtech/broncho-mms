#ifndef MMS_LINUX_H
#define MMS_LINUX_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

/*debug util*/
#ifdef NDEBUG
#define MMS_PRINTF
#define mms_assert
#else
#define MMS_PRINTF printf
#define mms_assert assert
#endif

/*memory management*/
#define mms_free             free
#define mms_calloc           calloc
#define mms_realloc          realloc
#define mms_malloc           malloc

/*file i/o*/
#define MMS_FILE             FILE
#define mms_fopen            fopen
#define mms_fclose           fclose
#define mms_fwrite           fwrite
#define mms_fread            fread
#define mms_fseek            fseek
#define mms_ftell            ftell

/*socket*/
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#define close_socket(fd) close(fd)
#define mms_socket_global_init()

#endif/*MMS_LINUX_H*/

