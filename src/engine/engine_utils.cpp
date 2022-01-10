#include "engine.h"
#include <fstream>

XPlatform::Api::XPResult j_xplatform_check_namespace(
	nlohmann::json& json,
	const char* name,
	const std::string& file_name,
	const uint32_t ext_id,
	XPlatform::Api::pfnXPlatfromMessageCallBack p_XPlatfromMessageCallBack);

extern char* c_xplatform_alloc_string(uint32_t size);

template<class type_t>
XPlatform::Api::XPResult j_xplatform_get_safe_value_from_json(
	nlohmann::json& j_json,
	const std::string& name,
	type_t& output,
	const std::string& file_name,
	const uint32_t ext_id,
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

XPlatform::Api::XPResult XPlatform::core::Engine::GetVersionFromJson(
	nlohmann::json& j_Config,
	const std::string& FileName,
	XPlatform::core::XPlatformVersion& Version) {
	if (CheckConfigFileNamespaces(j_Config, FileName, { "Version" }) !=
		XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS)
		return XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED_TO_PARSE_CONFIGURATION_FILE;

	nlohmann::json& j_Version = j_Config["Version"];

	if (CheckConfigFileNamespaces(j_Version, FileName, { "Major", "Minor", "Patch", "Alpha"}) !=
		XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS)
		return XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED_TO_PARSE_CONFIGURATION_FILE;

	XPlatform::Api::XPResult res = j_xplatform_get_safe_value_from_json<uint8_t>(
		j_Version, "Major", Version.major, FileName, XPLATFORM_CORE_EXT_ID, p_XPlatfromMessageCallBack
	);
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) return res;

	res = j_xplatform_get_safe_value_from_json<uint8_t>(
		j_Version, "Minor", Version.minor, FileName, XPLATFORM_CORE_EXT_ID, p_XPlatfromMessageCallBack
	);
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) return res;

	res = j_xplatform_get_safe_value_from_json<uint8_t>(
		j_Version, "Patch", Version.minor, FileName, XPLATFORM_CORE_EXT_ID, p_XPlatfromMessageCallBack
	);
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) return res;

	res = j_xplatform_get_safe_value_from_json<uint8_t>(
		j_Version, "Alpha", Version.alpha, FileName, XPLATFORM_CORE_EXT_ID, p_XPlatfromMessageCallBack
	);
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) return res;

	return XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS;
}

XPlatform::Api::XPResult XPlatform::core::Engine::CheckConfigFileNamespaces(
	nlohmann::json& j_ConfigFile,
	const std::string& FileName,
	const std::vector<const char*> namespaces) {

	XPlatform::Api::XPResult res = XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS;
	
	for (const char* name : namespaces){
		res = 
			j_xplatform_check_namespace(j_ConfigFile, name, FileName, XPLATFORM_CORE_EXT_ID, p_XPlatfromMessageCallBack);

		if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS)return  res;
	}

	return res;

}

XPlatform::Api::XPResult XPlatform::core::Engine::ParseConfigFile(
	const std::string& FileName,
	nlohmann::json& j_ConfigOutput) {

	std::ifstream FileStream(FileName);

	if (!FileStream.is_open()) {
		char* msg = c_xplatform_alloc_string(48 + 4 + FileName.size());

		strcat(msg, "[XPlatfrom][core][Engine]:Failed to open file:\'");
		strcat(msg, FileName.c_str());
		strcat(msg, "\'!");

		p_XPlatfromMessageCallBack(msg, XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED, 0);

		free(msg);

		return XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED_TO_OPEN_FILE;
	}

	try {
		FileStream >> j_ConfigOutput;
	}
	catch (const nlohmann::json::exception& ex) {
		const char* Err = ex.what();

		char* msg = c_xplatform_alloc_string(54 + 20 + 4 + FileName.size() + strlen(Err));

		strcat(msg, "[XPlatfrom][core][Engine]:Failed to parse json file:\'");
		strcat(msg, FileName.c_str());
		strcat(msg, "\'!\n");
		strcat(msg, "Json Parser Error:\n");
		strcat(msg, Err);

		p_XPlatfromMessageCallBack(msg, XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED, XPLATFORM_CORE_EXT_ID);

		free(msg);

		return XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED_TO_PARSE_CONFIGURATION_FILE;
	}

	FileStream.close();

	return XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS;

}