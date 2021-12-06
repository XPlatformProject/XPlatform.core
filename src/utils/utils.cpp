#include <stdint.h>
#include <memory>
#include <cassert>
#include <json.hpp>
#include <xplatform.core/api.h>

char* c_xplatform_alloc_string(uint32_t size) {
	size++;

	char* src = reinterpret_cast<char*>(malloc(size));
	assert(src);

	std::memset(src, 0, size);

	return src;
}

XPlatform::Api::XPResult j_xplatform_check_namespace(
	nlohmann::json& json,
	const char* name,
	const std::string& file_name,
	const int32_t ext_id,
	XPlatform::Api::pfnXPlatfromMessageCallBack p_XPlatfromMessageCallBack) {

	try {
		if (json[name].is_null()) {
			char* msg = c_xplatform_alloc_string((43 + 37 + 3) + file_name.size() + strlen(name));

			strcat(msg, "[XPlatfrom][core][Engine]: Failed to get \'");
			strcat(msg, name);
			strcat(msg, "\' namespace from json config file: \'");
			strcat(msg,
				file_name.c_str());
			strcat(msg, "\'!");

			p_XPlatfromMessageCallBack(msg, XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED, 0);

			free(msg);

			return XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED;
		}
	}
	catch (const nlohmann::json::exception& ex) {
		const char* Err = ex.what();
		char* msg = c_xplatform_alloc_string((43 + 37 + 4 + 20) + file_name.size() + strlen(name) + strlen(Err));

		strcat(msg, "[XPlatfrom][core][Engine]: Failed to get \'");
		strcat(msg, name);
		strcat(msg, "\' namespace from json config file: \'");
		strcat(msg, file_name.c_str());
		strcat(msg, "\'!\n");
		strcat(msg, "Json Parser Error:\n");
		strcat(msg, Err);

		p_XPlatfromMessageCallBack(msg, XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED, ext_id);

		free(msg);

		return XPlatform::Api::XPResult::XPLATFORM_RESULT_FAILED_TO_PARSE_CONFIGURATION_FILE;
	}

	return XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS;
}
