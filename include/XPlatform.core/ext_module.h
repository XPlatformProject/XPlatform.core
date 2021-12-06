#ifndef _INC_XPLATFORM_EXTENSION_MODULE_H
#define _INC_XPLATFORM_EXTENSION_MODULE_H

#include<string>
#include<xplatform.core/api.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#else
#include <dlfcn.h>
#endif

namespace XPlatform {
	namespace core {
		class XPLATFORM_API XPlatformExtensionModule {
		public:
#ifdef WIN32
			typedef HMODULE RawXPlatformExtensionModuleHandle;
#else
			typedef void* RawXPlatformExtensionModuleHandle;
#endif

		private:
			RawXPlatformExtensionModuleHandle m_Handle;

		public:

			XPlatformExtensionModule();

			XPlatform::Api::XPResult Load(const std::string& Path);

			XPlatform::core::XPlatformExtensionModule::RawXPlatformExtensionModuleHandle GetRawHandle()const;

			void* GetProc(const std::string& r_Name)const;

			void UnLoad();

			~XPlatformExtensionModule();

		};
	}
}

#endif