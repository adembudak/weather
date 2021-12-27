cmake_minimum_required(VERSION 3.18)

project(Weather LANGUAGES CXX)

find_package(cpprestsdk REQUIRED)

add_executable(weather main.cpp)
target_link_libraries(weather PRIVATE cpprestsdk::cpprest)
target_compile_features(weather PRIVATE cxx_std_11)

set_target_properties(weather PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
