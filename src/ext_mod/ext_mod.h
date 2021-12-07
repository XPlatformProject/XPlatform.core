#ifndef _XPLATFORM_EXTENSION_MODULE_H
#define _XPLATFORM_EXTENSION_MODULE_H

#include<string>
#include<XPlatform.core/api.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#else
#include <dlfcn.h>
#endif

namespace XPlatform {
	namespace core {
		class XPLATFORM_API_EXPORT XPlatformExtensionModule {
		public:
#ifdef WIN32
			typedef HINSTANCE RawXPlatformExtensionModuleHandle;
#else
			typedef void* RawXPlatformExtensionModuleHandle;
#endif

		private: 
			RawXPlatformExtensionModuleHandle m_Handle;

			void Internal_PrintError(const char* Path);

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