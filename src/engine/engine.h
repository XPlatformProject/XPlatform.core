#ifndef _XPLATFROM_ENGINE_H
#define _XPLATFROM_ENGINE_H
#define _INC_XPLATFROM_CORE_ENGINE_H

#include <string>

#include <XPlatform.core/api.h>
#ifndef _DONT_INCLUDE_ext_module
#include <XPlatform.core/ext_module.h>
#endif

#ifndef _XPLATFROM_DONT_INCLUDE_JSON
#include <json.hpp>
#endif

#include <vector>

namespace XPlatform {
	namespace core {
		typedef void*	(*pfn_xplatform_ext_create_class)(uint32_t id);
		typedef void	(*pfn_xplatform_ext_delete_class)(uint32_t id, void* ptr);

		struct XPlatformVersion {
			XPlatform::Api::u8 major;
			XPlatform::Api::u8 minor;
			XPlatform::Api::u8 patch;
			XPlatform::Api::u8 alpha;
		};

		struct EngineProjectInfo {
			std::string s_Name;
			XPlatform::core::XPlatformVersion v_Version;
		};

		struct XPlatformExtensionInfo {
			void inline operator=(const XPlatformExtensionInfo& _This) {
				this->ExtId = _This.ExtId;
				this->s_Name = _This.s_Name;
				this->s_Path = _This.s_Path;
				this->v_Version = _This.v_Version;
			}

			std::string s_Name;
			std::string s_Path;
			uint32_t ExtId;
			XPlatform::core::XPlatformVersion v_Version;
		};

		struct XPlatformInformation {
			std::string s_EngineDirectory;
			XPlatform::core::XPlatformVersion v_EngineVersion;
		};

		class XPLATFORM_API_EXPORT Engine {

			static Engine* p_Instance;
			
			XPlatform::Api::pfnXPlatfromMessageCallBack p_XPlatfromMessageCallBack;
			
			//Project information storage
			EngineProjectInfo* p_EPI;
			//Engine information storage
			XPlatformInformation* p_XI;

			std::vector<XPlatformExtensionInfo> m_ExtensionsInfo;
			std::vector<XPlatform::core::XPlatformExtensionModule*> m_ExtensionsModules;

			std::vector<XPlatform::core::pfn_xplatform_ext_create_class> m_ExtensionsCreatePfns;
			std::vector<XPlatform::core::pfn_xplatform_ext_delete_class> m_ExtensionsDeletePfns;

#ifndef _XPLATFROM_DONT_INCLUDE_JSON
			XPlatform::Api::XPResult CheckConfigFileNamespaces(
				nlohmann::json& j_ConfigFile,
				const std::string& FileName,
				const std::vector<const char*> namespaces
			);

			XPlatform::Api::XPResult ParseConfigFile(
				const std::string& FileName,
				nlohmann::json& j_ConfigOutput
			);

			XPlatform::Api::XPResult GetVersionFromJson(
				nlohmann::json& j_Config,
				const std::string& FileName,
				XPlatform::core::XPlatformVersion& Version
			);

			XPlatform::Api::XPResult LoadEngineExtensionInfoFromJson(nlohmann::json& j_Extension, 
				const std::string& FileName,
				const uint32_t id);
#endif

			XPlatform::Api::XPResult LoadEngineExtensions(const std::string& Dir);

			void  UnloadEngineExtensions();

			void InitEPI();
			void InitXI();

		public:

			static Engine* GetInstance();

			Engine();

			void SetMsgCallback(XPlatform::Api::pfnXPlatfromMessageCallBack p_Callback);
			XPlatform::Api::pfnXPlatfromMessageCallBack GetMsgCallback();
			void CallMsgCallback(const char* msg, XPlatform::Api::XPResult status, XPlatform::Api::i32 ext_id);

			XPlatform::Api::XPResult LoadEngine(const std::string& r_ProjectConfigFileName);

			XPlatform::Api::XPResult LoadEngineInternally(const std::string& r_ProjectName, const XPlatform::core::XPlatformVersion& v_ProjectVersion);

			XPlatform::Api::XPResult LoadExtension(const XPlatform::core::XPlatformExtensionInfo& r_XEI);
			XPlatform::Api::XPResult GetExtensionInfo(const std::string& r_Name, XPlatform::core::XPlatformExtensionInfo* p_OutInformation = NULL);
			
			void* CreateExtensionClass(const uint32_t ExtId, const uint32_t ClassId);
			void  DeleteExtensionClass(const uint32_t ExtId, const uint32_t ClassId, void* ptr);

			const std::vector<XPlatform::core::XPlatformExtensionInfo>& GetExtensionsInfoList() const;

#ifndef _DONT_INCLUDE_ext_module

			const XPlatform::core::XPlatformExtensionModule* GetExtensionModule(const uint32_t Id) const;

#endif

			const XPlatformInformation* GetEngineInfo();
			const EngineProjectInfo* GetProjectInfo();

			~Engine();

		};
	}
}

#endif