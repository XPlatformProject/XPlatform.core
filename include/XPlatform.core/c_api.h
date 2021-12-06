#ifndef _INC_XPLATFORM_C_API_H
#define _INC_XPLATFORM_C_API_H

#include <stddef.h>
#include <stdint.h>

typedef uint8_t XPResult;

typedef void(*pfnXPlatfromMessageCallBack)(
	const char* msg,
	XPResult status,
	int32_t ext_id
);

#define XPLATFORM_SUCCESS 0
#define XPLATFORM_FAILED 1

#endif