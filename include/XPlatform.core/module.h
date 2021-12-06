#ifndef _INC_XPLATFROM_CORE_MODULE_H
#define _INC_XPLATFROM_CORE_MODULE_H

#include<xplatform.core/api.h>

namespace XPlatform {
	namespace core {
		class XPModule {

		public:

			virtual void* GetAddr(const char* name) = 0;

			virtual XPlatform::Api::XPResult Load(const char* path) = 0;

		};
	}
}

#endif