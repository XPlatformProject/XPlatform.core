cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project("nlohmann_json")

add_library(nlohmann_json INTERFACE)

target_include_directories(nlohmann_json INTERFACE "include/")