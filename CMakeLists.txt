cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project("xplatform-core")

add_library(XPlatform.core SHARED
"src/loader.cpp" "include/XPlatform.core/loader.h"
"src/engine/engine.cpp" "src/engine/engine.h" "include/XPlatform.core/engine.h"
"src/engine/engine_utils.cpp" 
"src/utils/utils.cpp"

"src/ext_mod/ext_mod.cpp" "src/ext_mod/ext_mod.h"  "include/XPlatform.core/ext_module.h" "include/XPlatform.core/XP_IObject.h")

target_include_directories(XPlatform.core PUBLIC "include/")

add_subdirectory("ThirdParty")

if(UNIX)
set(CMAKE_STATIC_LIBRARY_PREFIX "")

target_link_libraries(XPlatform.core ${CMAKE_DL_LIBS})
endif(UNIX)
target_link_libraries(XPlatform.core nlohmann_json)