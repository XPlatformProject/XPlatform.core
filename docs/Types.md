<h1 align="center"> types </h1>

- XPResult - stores the result of the function:
    - XPLATFORM_RESULT_SUCCESS - the function worked successfully
    - XPLATFORM_RESULT_NEUTRAL - the function worked successfully, but there were not critical problems
    - XPLATFORM_RESULT_FAILED - An error occurred in the function
    - XPLATFORM_RESULT_FAILED_TO_OPEN_FILE  - the function failed to open the file 
    - XPLATFORM_RESULT_FAILED_TO_PARSE_CONFIGURATION_FILE - the function failed to parse the configuration file.
    - XPLATFORM_RESULT_FAILED_TO_ALLOCATE_MEMORY - the function could not allocate memory

- XPlatformVersion - stores version  

- EngineProjectInfo - stores project information
    - std::string s_Name - stores project name
    - XPlatform::core::XPlatformVersion v_Version - stores project version

- XPlatformExtensionInfo - stores extension info
    - std::string s_Name - stores extension name
    - std::string s_Path - stores extension module path
    - uint32_t ExtId - stores extension id in xplatform

- XPlatformInformation - stores engine information
    - std::string s_EngineDirectory - store the path of the engine directory
    - XPlatform::core::XPlatformVersion v_EngineVersion - stores engine version

    