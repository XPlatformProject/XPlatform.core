#ifndef _XPLATFORM_OBJECT_INTERFACE_H
#define _XPLATFORM_OBJECT_INTERFACE_H

namespace XPlatform {
	namespace core {
		class XPlatformCrossModuleObject {
		public:
			virtual void Release() = 0;
		};
	}
}

#endif