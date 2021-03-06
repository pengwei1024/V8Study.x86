cmake_minimum_required(VERSION 3.17)
project(V8Study_x86_)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE "Debug")
# 不设置报错如下: Embedder-vs-V8 build configuration mismatch.
# On embedder side pointer compression is DISABLED while on V8 side it's ENABLED.
add_compile_options(-DV8_COMPRESS_POINTERS)

#add_compile_options(
#        "-Wall" "-Wpedantic" "-Wextra" "-fexceptions"
#        "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
#)

INCLUDE_DIRECTORIES(
        lib/v8/include
        lib/
)


add_executable(V8Study_x86_ src/main.cpp src/V8Engine.cpp src/V8Snapshot.cpp src/GlobalObject.cpp src/Utils.hpp src/Console.cpp
        src/TimeOps.cpp
        )

MESSAGE(STATUS "SOURCE_DIR = ${PROJECT_SOURCE_DIR}.")

find_library(zlib z)
#是否启用 monolith ON 开启 /  OFF 关闭
#set(ENABLE_MONOLITH ON)

if (ENABLE_MONOLITH)
    MESSAGE(STATUS "### ENABLE_MONOLITH!!!")
    target_link_libraries(
            V8Study_x86_
            ${zlib}

            #        ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libchrome_zlib.a
            ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libcompression_utils_portable.a
            #        ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libminizip.a
            #        ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libv8_init.a
            #        ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libv8_initializers.a
            ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libv8_libbase.a
            ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libv8_libplatform.a
            ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libv8_snapshot.a
            ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libv8_compiler.a
            ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libcrdtp.a
            ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libcrdtp_platform.a
            ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libtorque_base.a
            ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libtorque_generated_definitions.a
            ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libtorque_generated_initializers.a
            ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libcppgc_base.a
            ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libv8_cppgc_shared.a
            ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/snapshot/libv8_base_without_compiler.a
#            v8pp
            ${PROJECT_SOURCE_DIR}/lib/v8pp/libv8pp.a
    )
else ()
    MESSAGE(STATUS "$$$ DISABLE_MONOLITH!!!")
    set(V8_DIR ${PROJECT_SOURCE_DIR}/lib/v8/platform/mac/libv8_monolith.a)
#    优先读取根目录下的 libv8_monolith.a
    if (EXISTS "${PROJECT_SOURCE_DIR}/libv8_monolith.a")
        set(V8_DIR ${PROJECT_SOURCE_DIR}/libv8_monolith.a)
    endif()
    target_link_libraries(
            V8Study_x86_
            ${zlib}
            ${PROJECT_SOURCE_DIR}/lib/v8pp/libv8pp.a
            ${V8_DIR}
    )
endif ()





