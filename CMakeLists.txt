cmake_minimum_required(VERSION 3.24)
project(PASCC)

include_directories("include")

file(GLOB_RECURSE include "include/*.h")
file(GLOB_RECURSE src "src/*.cc")

add_executable(PASCC
        ${include}
        ${src}
        )