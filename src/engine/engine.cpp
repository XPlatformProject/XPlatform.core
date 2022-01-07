#include "engine.h"

#include <fstream>
#include <cassert>
#include <json.hpp>
#include <stdio.h>
#include <iostream>

#define XPLATFORM_EXTENSION_SHUTDOWN_FUNCTION_NAME "xplatform_extension_shutdown"
#define XPLATFORM_EXTENSION_INIT_FUNCTION_NAME "xplatform_extension_init"

// this is internal functions:
extern char* c_xplatform_alloc_string(uint32_t size);

template<class type_t>
XPlatform::Api::XPResult j_xplatform_get_safe_value_from_json(
	nlohmann::json& j_json,
	const std::string& name,
	type_t& output,
	const std::string& file_name,
	const int32_t ext_id,
	XPlatform::Api::pfnXPlatfromMessageCallBack p_XPlatfromMessageCallBack) {

	try {
		output = j_json[name].get<type_t>();
	}
	catch (const nlohmann::json::exception& ex) {
		const char* err = ex.what();

		char* msg = c_xplatform_alloc_string((52 + name.size() + 27 + file_name.size() + 18) + strlen(err));

		strcat(msg, "[XPlatfrom][core][Engine]: Failed to get value of \'");
		strcat(msg, name.c_str());
		strcat(msg, "\' from json config file: \'");
		strcat(msg, file_name.c_str());
		strcat(msg, "\'!\nParser error:\n");
		strcat(msg, err);

		p_XPlatfromMessageCallBack(msg, XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED, ext_id);

		free(msg);

		return XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED_TO_PARSE_CONFIGURATION_FILE;
	}

	return XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS;
}

XPlatform::core::Engine* XPlatform::core::Engine::p_Instance = NULL;

void XPlatfromMessageCallBack(
	const char* msg,
	XPlatform::Api::XPResult status,
	XPlatform::Api::i32 ext_id) {

	printf("msg: %s\nstatus: %i\nextension: %i\n", msg, status, ext_id);

}

void XPlatform::core::Engine::InitEPI(){
	if (p_EPI == NULL) p_EPI = new XPlatform::core::EngineProjectInfo;
}

void XPlatform::core::Engine::InitXI() {
	if(p_XI == NULL) p_XI = new XPlatform::core::XPlatformInformation;
}

XPlatform::core::Engine* XPlatform::core::Engine::GetInstance(){
	return XPlatform::core::Engine::p_Instance;
}

const XPlatform::core::XPlatformInformation* XPlatform::core::Engine::GetEngineInfo(){
	assert(p_XI != NULL);
	return p_XI;
}

const XPlatform::core::XPlatformExtensionModule* XPlatform::core::Engine::GetExtensionModule(const uint32_t id)const{
	assert(m_ExtensionsModules.size() >= id);
	return m_ExtensionsModules[id];
}

XPlatform::Api::XPResult XPlatform::core::Engine::GetExtensionInfo(const std::string& r_Name, XPlatform::core::XPlatformExtensionInfo* p_OutInformation) {

	if(p_OutInformation != NULL)
		for (XPlatform::core::XPlatformExtensionInfo& r_ExtInfo : m_ExtensionsInfo){
			if (r_ExtInfo.s_Name == r_Name) { *p_OutInformation = r_ExtInfo; return XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS; };
		}
	else
		for (XPlatform::core::XPlatformExtensionInfo& r_ExtInfo : m_ExtensionsInfo) {
			if (r_ExtInfo.s_Name == r_Name) return XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS;
		}

	return XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED;
}

void* XPlatform::core::Engine::CreateExtensionClass(const uint32_t ExtId, const uint32_t ClassId){
	assert(m_ExtensionsCreatePfns.size() >= ExtId);
	return m_ExtensionsCreatePfns[ExtId](ClassId);
}

void XPlatform::core::Engine::DeleteExtensionClass(const uint32_t ExtId, const uint32_t ClassId, void* ptr){
	m_ExtensionsDeletePfns[ExtId](ClassId, ptr);
}

const std::vector<XPlatform::core::XPlatformExtensionInfo>& XPlatform::core::Engine::GetExtensionsInfoList() const{
	return m_ExtensionsInfo;
}

void XPlatform::core::Engine::SetMsgCallback(XPlatform::Api::pfnXPlatfromMessageCallBack p_CallBack){
	p_XPlatfromMessageCallBack = p_CallBack;
}

XPlatform::Api::pfnXPlatfromMessageCallBack XPlatform::core::Engine::GetMsgCallback(){
	return p_XPlatfromMessageCallBack;
}

void XPlatform::core::Engine::CallMsgCallback(const char* msg, XPlatform::Api::XPResult status, XPlatform::Api::i32 ext_id){
	p_XPlatfromMessageCallBack(msg, status, ext_id);
}

XPlatform::Api::XPResult XPlatform::core::Engine::LoadExtension(const XPlatform::core::XPlatformExtensionInfo& r_XEI){

	XPlatform::Api::XPResult res = GetExtensionInfo(r_XEI.s_Name);
	if (res == XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED) {
		if (m_ExtensionsInfo.size() <= r_XEI.ExtId) {
			m_ExtensionsInfo.resize(r_XEI.ExtId + 1);
		}
		m_ExtensionsInfo[r_XEI.ExtId] = r_XEI;
	}

	XPlatform::core::XPlatformExtensionModule* p_ExtModule = new XPlatform::core::XPlatformExtensionModule();

	res = p_ExtModule->Load(r_XEI.s_Path);
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) {
		delete p_ExtModule;
		
		return res;
	}

	if (m_ExtensionsModules.size() <= r_XEI.ExtId) {
		m_ExtensionsModules.resize(r_XEI.ExtId + 1);
	}

	res = XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS;

	void* p_InitFunction = p_ExtModule->GetProc(XPLATFORM_EXTENSION_INIT_FUNCTION_NAME);
	if (p_InitFunction != NULL) {
		XPlatform::Api::XPResult(*xplatform_extension_init)() = reinterpret_cast<XPlatform::Api::XPResult(*)()>(p_InitFunction);

		res = xplatform_extension_init();
	}

	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) return res;

	void* p_Function = p_ExtModule->GetProc("xplatform_ext_create_class");
	if (m_ExtensionsCreatePfns.size() <= r_XEI.ExtId) {
		m_ExtensionsCreatePfns.resize(r_XEI.ExtId + 1);
	}
	m_ExtensionsCreatePfns[r_XEI.ExtId] = reinterpret_cast<XPlatform::core::pfn_xplatform_ext_create_class>(p_Function);

	p_Function = p_ExtModule->GetProc("xplatform_ext_delete_class");
	if (m_ExtensionsDeletePfns.size() <= r_XEI.ExtId) {
		m_ExtensionsDeletePfns.resize(r_XEI.ExtId + 1);
	}
	m_ExtensionsDeletePfns[r_XEI.ExtId] = reinterpret_cast<XPlatform::core::pfn_xplatform_ext_delete_class>(p_Function);

	m_ExtensionsModules[r_XEI.ExtId] = p_ExtModule;

	return XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS;
}

void XPlatform::core::Engine::UnloadEngineExtensions() {
	for (XPlatform::core::XPlatformExtensionModule* Mdl : m_ExtensionsModules){
		if (Mdl != NULL) {
			void* p_ShutdownFunction = Mdl->GetProc(XPLATFORM_EXTENSION_SHUTDOWN_FUNCTION_NAME);

			if (p_ShutdownFunction != NULL) {
				void(*xplatform_extension_shutdown)() = reinterpret_cast<void(*)()>(p_ShutdownFunction);
				
				xplatform_extension_shutdown();
			}

			Mdl->UnLoad();
			delete Mdl;

			Mdl = NULL;
		}
	}

	m_ExtensionsModules.clear();
}

XPlatform::Api::XPResult XPlatform::core::Engine::LoadEngineExtensionInfoFromJson(
	nlohmann::json& j_Extension,
	const std::string& FileName,
	const uint32_t id){
	
	XPlatform::core::XPlatformExtensionInfo& r_ExtInfo = m_ExtensionsInfo[id];

	XPlatform::Api::XPResult res = j_xplatform_get_safe_value_from_json<std::string>(
		j_Extension, "Name",
		r_ExtInfo.s_Name,
		FileName,
		-1,
		p_XPlatfromMessageCallBack);
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS)return res;

	res = j_xplatform_get_safe_value_from_json<std::string>(
		j_Extension, "Path",
		r_ExtInfo.s_Path,
		FileName,
		-1,
		p_XPlatfromMessageCallBack);
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS)return res;

	res = GetVersionFromJson(j_Extension, FileName, r_ExtInfo.v_Version);
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS)return res;

	r_ExtInfo.ExtId = id;

	return XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS;
}

XPlatform::core::Engine::Engine() : p_EPI(NULL), p_XI(NULL) {
	this->p_XPlatfromMessageCallBack = XPlatfromMessageCallBack;
	p_Instance = this;
}

extern XPlatform::Api::XPResult j_xplatform_check_namespace(
	nlohmann::json& json,
	const char* name,
	const std::string& file_name,
	const int32_t ext_id,
	XPlatform::Api::pfnXPlatfromMessageCallBack p_XPlatfromMessageCallBack);

XPlatform::Api::XPResult XPlatform::core::Engine::LoadEngineInternally(const std::string& r_ProjectName, const XPlatform::core::XPlatformVersion& v_ProjectVersion) {

	InitEPI();

	p_EPI->s_Name = r_ProjectName;
	p_EPI->v_Version = v_ProjectVersion;

	InitXI();

	p_XI->s_EngineDirectory = "Internally";
	p_XI->v_EngineVersion = { 0, 0, 0, 1 };

	return XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS;
}

XPlatform::Api::XPResult XPlatform::core::Engine::LoadEngine(const std::string& r_ProjectConfigFileName){
	nlohmann::json j_ProjectJsonConfig;

	InitEPI();

	XPlatform::Api::XPResult res = ParseConfigFile(r_ProjectConfigFileName, j_ProjectJsonConfig);
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) return res;

	res = this->CheckConfigFileNamespaces(j_ProjectJsonConfig, r_ProjectConfigFileName,
		{"XPlatform"});
	if(res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) return res;

	res = this->CheckConfigFileNamespaces(j_ProjectJsonConfig["XPlatform"], r_ProjectConfigFileName,
		{ "Engine", "Project" });
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) return res;

	res = this->CheckConfigFileNamespaces(j_ProjectJsonConfig["XPlatform"]["Engine"], r_ProjectConfigFileName,
		{ "Dir" });
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) return res;

	res = this->CheckConfigFileNamespaces(j_ProjectJsonConfig["XPlatform"]["Project"], r_ProjectConfigFileName,
		{ "Name" });
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) return res;

	res = this->GetVersionFromJson(j_ProjectJsonConfig["XPlatform"]["Project"],
		r_ProjectConfigFileName, p_EPI->v_Version);
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) return res;

	std::string EngineDir;

	res = j_xplatform_get_safe_value_from_json<std::string>(
		j_ProjectJsonConfig["XPlatform"]["Engine"],
		"Dir",
		EngineDir,
		r_ProjectConfigFileName,
		-1,
		p_XPlatfromMessageCallBack
	);

	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) return res;

	res = j_xplatform_get_safe_value_from_json<std::string>(
		j_ProjectJsonConfig["XPlatform"]["Project"],
		"Name",
		p_EPI->s_Name,
		r_ProjectConfigFileName,
		-1,
		p_XPlatfromMessageCallBack
	);

	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) return res;

	res = LoadEngineExtensions(EngineDir);

	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) return res;

	return XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS;
}

XPlatform::core::Engine::~Engine(){
	if(p_EPI != NULL)
		free(p_EPI);
	if (p_XI != NULL)
		free(p_XI);
	if (!m_ExtensionsInfo.empty()) m_ExtensionsInfo.clear();
	if (!m_ExtensionsModules.empty()) {
		UnloadEngineExtensions();
	}
}

XPlatform::Api::XPResult XPlatform::core::Engine::LoadEngineExtensions(const std::string& s_Dir){
	const std::string s_EngineConfigPath = s_Dir + "XPlatform.json";
	
	InitXI();

	p_XI->s_EngineDirectory.erase(0);
	p_XI->s_EngineDirectory.resize(s_Dir.size());
	p_XI->s_EngineDirectory = s_Dir;

	nlohmann::json j_EngineConfig;
	XPlatform::Api::XPResult res =
		ParseConfigFile(s_EngineConfigPath, j_EngineConfig);
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS)
		return res;

	res = CheckConfigFileNamespaces(j_EngineConfig, s_EngineConfigPath, { "XPlatform" });
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS)
		return res;

	j_EngineConfig = j_EngineConfig["XPlatform"];
	
	res = GetVersionFromJson(j_EngineConfig, s_EngineConfigPath, p_XI->v_EngineVersion);
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS)
		return res;
	
	res = CheckConfigFileNamespaces(j_EngineConfig, s_EngineConfigPath, { "Extensions" });
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) return res;

	nlohmann::json& j_Extensions = j_EngineConfig["Extensions"];

	if (!j_Extensions.is_array()) {
		char* msg = c_xplatform_alloc_string((29 + 11 + 33 + 3) + s_EngineConfigPath.size());

		strcat(msg, "[XPlatfrom][core][Engine]: \'");
		strcat(msg, "Extensions");
		strcat(msg, "\' is not array in config file: \'");
		strcat(msg,
			s_EngineConfigPath.c_str());
		strcat(msg, "\'!");

		p_XPlatfromMessageCallBack(msg, XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED, 0);

		free(msg);

		return XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED;
	}

	m_ExtensionsInfo.resize(j_Extensions.size());

	res = XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS;

	//getting extensions info!
	for (uint32_t i = 0; i < j_Extensions.size(); i++){
		res = LoadEngineExtensionInfoFromJson(j_Extensions[i], s_EngineConfigPath, i);
		if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) return res;
	}

	bool IsFailed = false;

	for (XPlatform::core::XPlatformExtensionInfo& r_ExtInfo : m_ExtensionsInfo){
		res = LoadExtension(r_ExtInfo);
		if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) IsFailed = true;
	}

	if (IsFailed) return res;

	return XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS;
}

const XPlatform::core::EngineProjectInfo* XPlatform::core::Engine::GetProjectInfo(){
	assert(p_EPI != NULL);
	return p_EPI;
}
