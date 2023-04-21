cmake_minimum_required(VERSION 3.4)
project(PASCC)

find_package(FLEX)
find_package(BISON)

BISON_TARGET(Parser scripts/parser.y src/parser.tab.cc DEFINES_FILE include/parser.tab.h)
FLEX_TARGET(Lexer scripts/lex.l src/lex.yy.cc)
# command>  cmake --build . --target PASCC -j 6
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include_directories("include")
include_directories("thirdparty")
# Windows下的编译指令
if(WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_WIN32")
  file(COPY "thirdparty/clang-format.exe" DESTINATION ${CMAKE_BINARY_DIR}/../bin/)
# Linux下的编译指令
elseif(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__linux__")
  file(COPY "/usr/bin/clang-format" DESTINATION ${CMAKE_BINARY_DIR}/../bin/)
endif()

file(GLOB_RECURSE include "include/*.h" "thirdparty/*.h" "thirdparty/*.hpp")
file(GLOB_RECURSE src "src/*.cc")
list(REMOVE_ITEM src "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc")
file(GLOB_RECURSE test "test/*.cc")
file(GLOB_RECURSE tools "thirdparty/*.c" "thirdparty/*.cc")

add_executable(PASCC
        ${tools}
        ${src}
        src/lex.yy.cc
        src/parser.tab.cc
        )

add_executable(PASCC_TEST
        ${tools}
        ${src}
        ${test}
        )

