cmake_minimum_required(VERSION 3.1)
project(PASCC)
# command>  cmake --build . --target PASCC -j 6

find_package(FLEX)
find_package(BISON)
if(FLEX_FOUND AND BISON_FOUND)
  message(STATUS "FLEX and BISON found")
  BISON_TARGET(Parser ${CMAKE_BINARY_DIR}/../scripts/parser.y  ${CMAKE_BINARY_DIR}/../src/parser.tab.cc  DEFINES_FILE  ${CMAKE_BINARY_DIR}/../include/parser.tab.h)
  FLEX_TARGET(Lexer  ${CMAKE_BINARY_DIR}/../scripts/lex.l  ${CMAKE_BINARY_DIR}/../src/lex.yy.cc)
  ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include_directories("include")
include_directories("thirdparty")
# Windows下的编译指令
if(WIN32)
  message(STATUS "Windows")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32 -Wno-error=register")
  file(COPY "thirdparty/clang-format.exe" DESTINATION ${CMAKE_BINARY_DIR}/../bin/)
# Linux下的编译指令
elseif(UNIX)
  message(STATUS "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__linux__ -Wno-error=register")
  file(COPY "/usr/bin/clang" DESTINATION ${CMAKE_BINARY_DIR}/../bin/)
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
        src/parser.tab.cc)

add_executable(PASCC_TEST
        ${tools}
        ${src}
        src/lex.yy.cc
        src/parser.tab.cc
        ${test}
        )

