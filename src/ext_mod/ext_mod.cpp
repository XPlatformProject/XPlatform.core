#include"ext_mod.h"
#include <cassert>

#define _DONT_INCLUDE_ext_module
#include "..//engine/engine.h"

extern char* c_xplatform_alloc_string(uint32_t size);

void XPlatform::core::XPlatformExtensionModule::Internal_PrintError(const char* Path){
	std::string Msg = "[XPlatform][core][XPlatformExtensionModule]: Failed to load extension: '";
	Msg += Path;
	Msg += "':\n";

#ifdef WIN32
	DWORD ErrorMsgId = GetLastError();
	char* MessageBuffer = NULL;

	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		ErrorMsgId,
		MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
		(LPSTR)&MessageBuffer, 0, NULL
	);

	Msg += MessageBuffer;

#else
	Msg += dlerror();
#endif

	XPlatform::core::Engine::GetInstance()->
		CallMsgCallback(Msg.c_str(), XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED, 0);

	Msg.clear();
}

XPlatform::core::XPlatformExtensionModule::XPlatformExtensionModule() : m_Handle(NULL){
}

XPlatform::Api::XPResult XPlatform::core::XPlatformExtensionModule::Load(const std::string& Path){
	if (m_Handle != NULL) {
		UnLoad();
	}

#ifdef WIN32
	m_Handle = LoadLibraryA(Path.c_str());
#else
	m_Handle = dlopen(Path.c_str(), RTLD_LAZY);
#endif

	if (m_Handle == NULL) {
		Internal_PrintError(Path.c_str());

		return XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED;
	}

	return XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS;
}

XPlatform::core::XPlatformExtensionModule::RawXPlatformExtensionModuleHandle 
XPlatform::core::XPlatformExtensionModule::GetRawHandle()const {
	return m_Handle;
}

void* XPlatform::core::XPlatformExtensionModule::GetProc(const std::string& r_Name)const {
	assert(m_Handle != NULL);

#ifdef WIN32
	return GetProcAddress((HMODULE)m_Handle, r_Name.c_str());
#else
	return dlsym(m_Handle, r_Name.c_str());
#endif
}

void XPlatform::core::XPlatformExtensionModule::UnLoad(){
	if (m_Handle != NULL) {
#ifdef WIN32
		FreeLibrary(m_Handle);
#else
		dlclose(m_Handle);
#endif
		m_Handle = NULL;
	}
}

XPlatform::core::XPlatformExtensionModule::~XPlatformExtensionModule(){
	UnLoad();
}
