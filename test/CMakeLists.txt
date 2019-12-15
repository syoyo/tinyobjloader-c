cmake_minimum_required(VERSION 3.14)

project(tinyobjloader LANGUAGES C)

file(GLOB SOURCE_FILES "*.c")
file(GLOB HEADER_FILES "*.h")

add_executable(tinyobjloader ${SOURCE_FILES} ${HEADER_FILES} "../tinyobj_loader_c.h")
target_compile_definitions(tinyobjloader PUBLIC "_CRT_SECURE_NO_WARNINGS")
target_include_directories(tinyobjloader PUBLIC "../")
