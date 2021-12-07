<h1 align="center"> types </h1>

## [go back](Wiki.md)

---

## Enums:
- XPResult[api.h] - stores the result of the function:
    - XPLATFORM_RESULT_SUCCESS - the function worked successfully
    - XPLATFORM_RESULT_NEUTRAL - the function worked successfully, but there were not critical problems
    - XPLATFORM_RESULT_FAILED - An error occurred in the function
    - XPLATFORM_RESULT_FAILED_TO_OPEN_FILE  - the function failed to open the file 
    - XPLATFORM_RESULT_FAILED_TO_PARSE_CONFIGURATION_FILE - the function failed to parse the configuration file.
    - XPLATFORM_RESULT_FAILED_TO_ALLOCATE_MEMORY - the function could not allocate memory

    ---

## structs:

- XPlatformVersion[engine.h] - stores version  

- EngineProjectInfo[engine.h] - stores project information
    - std::string s_Name - stores project name
    - XPlatform::core::XPlatformVersion v_Version - stores project version

- XPlatformExtensionInfo[engine.h] - stores extension info
    - std::string s_Name - stores extension name
    - std::string s_Path - stores extension module path
    - uint32_t ExtId - stores extension id in xplatform

- XPlatformInformation[engine.h] - stores engine information
    - std::string s_EngineDirectory - store the path of the engine directory
    - XPlatform::core::XPlatformVersion v_EngineVersion - stores engine version

---

## classes:
- XPlatformExtensionModule[ext_module.h] - makes it convenient to work with dynamic libraries

    - RawXPlatformExtensionModuleHandle m_Handle - raw dynamic library handle
    
    Methods:
    - XPlatform::Api::XPResult [Load](const std::string& Path)
        - Return Codes: <br>
            On success: <br> - XPLATFORM_RESULT_SUCCESS <br>
            On failure: <br> - XPLATFORM_RESULT_FAILED

    -  XPlatform::core::XPlatformExtensionModule::RawXPlatformExtensionModuleHandle GetRawHandle() const - return m_Handle

    -  void* GetProc(const std::string& r_Name)const - return the address of the exported function

    - void UnLoad() - unload the library

- Engine[engine.h] -  it is singletone, use "static Engine* GetInstance();" for work with this class, singletone initialize after calling "XPlatform::Api::XPResult 
XPlatfromInit();"
and is destroyed after the call "void XPlatfromShutdown();"

    Methods:
    
    - XPlatform::Api::XPResult LoadEngine(const std::string& r_ProjectConfigFileName)
        
        - Return Codes: <br>
            On success: <br> - XPLATFORM_RESULT_SUCCESS <br>
            On failure: <br> - XPLATFORM_RESULT_FAILED <br> - XPLATFORM_RESULT_FAILED_TO_ALLOCATE_MEMORY <br> -XPLATFORM_RESULT_FAILED_TO_PARSE_CONFIGURATION_FILE <br> - XPLATFORM_RESULT_FAILED_TO_OPEN_FILE

    - XPlatform::Api::XPResult LoadExtension(const XPlatform::core::XPlatformExtensionInfo& r_XEI) - the method loads a dynamic library

        - Return Codes: <br>
        On success: <br> XPLATFORM_RESULT_SUCCESS <br>
        On failure: <br>  - XPLATFORM_RESULT_FAILED

    - XPlatform::Api::XPResult GetExtensionInfo(const std::string& r_Name, XPlatform::core::XPlatformExtensionInfo* p_OutInformation = NULL) - the method looks for an extension, if successful, fills in p_OutInformation if it is not NULL

         - Return Codes: <br>
        On success: <br> XPLATFORM_RESULT_SUCCESS <br>
        On failure: <br>  - XPLATFORM_RESULT_FAILED

    - void* CreateExtensionClass(const uint32_t ExtId, const uint32_t ClassId) - The method provides an api for creating a class

    - void  DeleteExtensionClass(const uint32_t ExtId, const uint32_t ClassId, void* ptr); - The method provides an api for deleting a class object(void* ptr)

--- 
## [go back](Wiki.md)