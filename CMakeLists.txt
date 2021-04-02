cmake_minimum_required(VERSION 3.17)
project(V8Study_x86_)
set(CMAKE_CXX_STANDARD 11)
# 不设置报错如下: Embedder-vs-V8 build configuration mismatch.
# On embedder side pointer compression is DISABLED while on V8 side it's ENABLED.
add_compile_options(-DV8_COMPRESS_POINTERS)

INCLUDE_DIRECTORIES(
        lib/v8/include
        )

add_executable(V8Study_x86_ main.cpp V8Engine.cpp)

MESSAGE( STATUS "SOURCE_DIR = ${PROJECT_SOURCE_DIR}.")

target_link_libraries(
        V8Study_x86_
        ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/libv8_monolith.a
)



