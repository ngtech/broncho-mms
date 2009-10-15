#include "mms_type.h"

void  mms_socket_global_init(void)
{
	static inited = FALSE;
	if(!inited)
	{
		int err = 0;
		WSADATA wsaData = {0};
		unsigned short wVersionRequested = MAKEWORD( 2, 2 );
		err = WSAStartup( wVersionRequested, &wsaData );
		
		inited = TRUE;
	}

	return ;
}
