cmake_minimum_required(VERSION 3.24)
project(PASCC)

# command>  cmake --build . --target PASCC -j 6
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include_directories("include")
include_directories("thirdparty")

file(GLOB_RECURSE include "include/*.h" "thirdparty/*.h" "thirdparty/*.hpp")
file(GLOB_RECURSE src "src/*.cc")
list(REMOVE_ITEM src "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc")
file(GLOB_RECURSE test "test/*.cc")
file(GLOB_RECURSE tools "thirdparty/*.c" "thirdparty/*.cc")

add_executable(PASCC
        ${tools}
        ${src}
        src/main.cc
        )

add_executable(PASCC_TEST
        ${tools}
        ${src}
        ${test}
        )