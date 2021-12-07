<h1 align="center"> Example </h1>

## [go back](Wiki.md)

---
## main.cpp:
``` c++
#include<XPlatform.core/loader.h>
#include<XPlatform.core/engine.h>

#include<iostream>

int main(int argc, char* argv[]) {

	XPlatform::core::XPlatfromInit();

	XPlatform::core::Engine* pEngine = XPlatform::core::Engine::GetInstance();

	XPlatform::Api::XPResult res = pEngine->LoadEngine("project.json");
	if (res != XPlatform::Api::XPResult::XPLATFORM_RESULT_SUCCESS) {
		XPlatform::core::XPlatfromShutdown();
		
		return -1;
	}

	const XPlatform::core::EngineProjectInfo* p_EPI;
	p_EPI = pEngine->GetProjectInfo();

	std::cout << "project name:" << p_EPI->s_Name << std::endl;

	const XPlatform::core::XPlatformInformation* p_XI;
	p_XI = pEngine->GetEngineInfo();

	std::cout << "Engine directory:" << p_XI->s_EngineDirectory << std::endl;

	XPlatform::core::XPlatfromShutdown();

	return 0;
}
```

## project.json:
``` json
{
  "XPlatform": {
    "Engine": {
      "Dir": "Engine/"
    },

    "Project": {
      "Name": "example",
      "Version": {
        "Major": 0,
        "Minor": 0,
        "Patch": 0,
        "Alpha": 1
      }
    }

  }
}
```

--- 
## [go back](Wiki.md)