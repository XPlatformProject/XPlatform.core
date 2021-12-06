#ifndef _INC_XPLATFROM_CORE_ENGINE_H
#define _INC_XPLATFROM_CORE_ENGINE_H

#include <xplatform.core/api.h>
#ifndef _XPLATFORM_DONT_INCLUDE_EXT_MODULE_H
#include <xplatform.core/ext_module.h>
#endif
#include <string>
#include <vector>

#ifdef _XPLATFROM_INCLUDE_JSON
#include <json.hpp>
#endif

namespace XPlatform {
	namespace core {
		typedef void* (*pfn_xplatform_ext_create_class)(uint32_t id);
		typedef void	(*pfn_xplatform_ext_delete_class)(uint32_t id, void* ptr);

#ifndef _INC_XPLATFROM_LOADER_H

		struct XPlatformVersion {
			XPlatform::Api::u8 major;
			XPlatform::Api::u8 minor;
			XPlatform::Api::u8 patch;
			XPlatform::Api::u8 alpha;
		};
#endif

		struct EngineProjectInfo {
			std::string s_Name;
			XPlatform::core::XPlatformVersion v_Version;
		};

		struct XPlatformExtensionInfo {
			std::string s_Name;
			std::string s_Path;
			int32_t ExtId;
			XPlatform::core::XPlatformVersion v_Version;
		};

		struct XPlatformInformation {
			std::string s_EngineDirectory;
			XPlatform::core::XPlatformVersion v_EngineVersion;
		};

		class XPLATFORM_API Engine {

			static Engine* p_Instance;

			XPlatform::Api::pfnXPlatfromMessageCallBack p_XPlatfromMessageCallBack;

			//Project information storage
			EngineProjectInfo* p_EPI;
			//Engine information storage
			XPlatformInformation* p_XI;

			std::vector<XPlatformExtensionInfo> m_ExtensionsInfo;
#ifndef _XPLATFORM_DONT_INCLUDE_EXT_MODULE_H

			std::vector<XPlatform::core::XPlatformExtensionModule*> m_ExtensionsModules;

#endif

			std::vector<XPlatform::core::pfn_xplatform_ext_create_class> m_ExtensionsCreatePfns;
			std::vector<XPlatform::core::pfn_xplatform_ext_delete_class> m_ExtensionsDeletePfns;

		public:

			static Engine* GetInstance();

			Engine();

			void SetMsgCallback(XPlatform::Api::pfnXPlatfromMessageCallBack p_Callback);
			XPlatform::Api::pfnXPlatfromMessageCallBack GetMsgCallback();
			XPlatform::Api::pfnXPlatfromMessageCallBack CallMsgCallback(const char* msg, XPlatform::Api::XPResult status, XPlatform::Api::i32 ext_id);

			XPlatform::Api::XPResult LoadEngine(const std::string& r_ProjectConfigFileName);

			XPlatform::Api::XPResult LoadEngineInternally(const std::string& r_ProjectName, const XPlatform::core::XPlatformVersion& v_ProjectVersion);

			XPlatform::Api::XPResult LoadExtension(const XPlatform::core::XPlatformExtensionInfo& r_XEI);
			XPlatform::Api::XPResult GetExtensionInfo(const std::string& r_Name, XPlatform::core::XPlatformExtensionInfo* p_OutInformation = NULL);

			void* CreateExtensionClass(const uint32_t ExtId, const uint32_t ClassId);
			void  DeleteExtensionClass(const uint32_t ExtId, const uint32_t ClassId, void* ptr);

#ifndef _XPLATFORM_DONT_INCLUDE_EXT_MODULE_H

			const XPlatform::core::XPlatformExtensionModule* GetExtensionModule(const uint32_t Id) const;

#endif

			const XPlatformInformation* GetEngineInfo();
			const EngineProjectInfo* GetProjectInfo();

			~Engine();

		};
	}
}

#endif