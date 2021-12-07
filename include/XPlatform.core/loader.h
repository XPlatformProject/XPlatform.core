#ifndef _INC_XPLATFROM_LOADER_H
#define _INC_XPLATFROM_LOADER_H

#include<XPlatform.core/api.h>

namespace XPlatform {
	namespace core {
#ifndef _INC_XPLATFROM_CORE_ENGINE_H

		struct XPlatformVersion {
			XPlatform::Api::u8 major;
			XPlatform::Api::u8 minor;
			XPlatform::Api::u8 patch;
			XPlatform::Api::u8 alpha;
		};

#endif

		//call it function for initialize xplatfrom.core extension before use any core extension functions!
		XPLATFORM_API XPlatform::Api::XPResult XPlatfromInit();

		//call after instance of engine destroyed!
		XPLATFORM_API void XPlatfromShutdown();

	}
}

#endif