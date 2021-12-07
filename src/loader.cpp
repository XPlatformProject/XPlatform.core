#include <XPlatform.core/engine.h>

#include <memory>

namespace XPlatform {
	namespace core {

		XPLATFORM_API_EXPORT XPlatform::Api::XPResult XPlatfromInit();

		XPLATFORM_API_EXPORT void XPlatfromShutdown();

	}
}


XPlatform::Api::XPResult XPLATFORM_API_EXPORT XPlatform::core::XPlatfromInit() {
	new XPlatform::core::Engine;

	return XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS;
}

void XPLATFORM_API_EXPORT XPlatform::core::XPlatfromShutdown() {
	delete XPlatform::core::Engine::GetInstance();
}