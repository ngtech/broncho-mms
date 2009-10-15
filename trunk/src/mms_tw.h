#ifndef MMS_TW_H
#define MMS_TW_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tw_mem.h"
#include "os_api.h"
#include "twfs.h"

/*debug util*/
#ifdef NDEBUG
#define MMS_PRINTF
#define mms_assert
#else
#define MMS_PRINTF OS_TRACE
#define mms_assert OS_ASSERT
#endif

/*memory management*/
void    mms_free(void* p);
#define mms_calloc              TW_Calloc
#define mms_realloc             TW_Realloc
#define mms_malloc              TW_Malloc

/*file i/o*/
#define MMS_FILE                TW_FILE
TW_FILE *mms_fopen(const char *filename, const char *mode);
#define mms_fclose              TW_fclose
#define mms_fwrite              TW_fwrite
#define mms_fread               TW_fread
#define mms_fseek               TW_fseek
#define mms_ftell               TW_ftell

#define snprintf             _snprintf
#define strcasecmp           stricmp

#include "tw_socket.h"

#endif	//MMS_TW_H
