#ifndef MMS_WIN32_H
#define MMS_WIN32_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
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

#define snprintf             _snprintf
#define strcasecmp           stricmp

#include <winsock.h>
void  mms_socket_global_init(void);
#define close_socket(fd) closesocket(fd)

#endif/*MMS_WIN32_H*/

