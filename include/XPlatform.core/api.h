#ifndef _INC_XPLATFORM_API_H
#define _INC_XPLATFORM_API_H

#include <stddef.h>
#include <stdint.h>
#include <vector>

#ifdef WIN32
	#define XPLATFORM_API __declspec(dllimport)
	
	#define XPLATFORM_API_EXPORT __declspec(dllexport)
#else
	#define XPLATFORM_API __attribute__((dllimport))

	#define XPLATFORM_API_EXPORT __attribute__((dllexport))
#endif

#define XPLATFORM_CORE_EXT_ID UINT32_MAX

namespace XPlatform {
	namespace Api {
		typedef uint32_t u32;
		typedef uint16_t u16;
		typedef uint8_t u8;

		typedef int16_t i16;
		typedef int8_t i8;
		typedef int32_t i32;

		enum class XPResult : u8 {
			XPLATFORM_RESULT_SUCCESS = 0,
			XPLATFORM_RESULT_NEUTRAL = 1,
			XPLATFORM_RESULT_FAILED = 2,
			XPLATFORM_RESULT_FAILED_TO_OPEN_FILE = 3,
			XPLATFORM_RESULT_FAILED_TO_PARSE_CONFIGURATION_FILE = 4,
			XPLATFORM_RESULT_FAILED_TO_ALLOCATE_MEMORY = 5
		};

		typedef void(*pfnXPlatfromMessageCallBack)(
			const char* msg,
			XPlatform::Api::XPResult status,
			XPlatform::Api::i32 ext_id
			);

	}
}

#endif