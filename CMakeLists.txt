cmake_minimum_required(VERSION 3.21)
project(cpp_parser)

set(CMAKE_CXX_STANDARD 23)

include_directories(work/src/dependencies/CDS/include)

include_directories(work)
include_directories(work/src)
include_directories(work/src/lang)
include_directories(work/src/pre)
include_directories(work/src/run)

add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/tmp/scanner.o
            ${CMAKE_CURRENT_BINARY_DIR}/tmp/syntax.cpp
            ${CMAKE_CURRENT_BINARY_DIR}/tmp/syntax.hpp
            ${CMAKE_CURRENT_BINARY_DIR}/tmp/tokenUtils.o
            ${CMAKE_CURRENT_BINARY_DIR}/tmp/tokenUtils.c
        COMMAND bash ../work/tools/clean.sh
        COMMAND bash ../work/tools/pre-build.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(
        cpp_parser

        ${CMAKE_CURRENT_BINARY_DIR}/tmp/scanner.o
        ${CMAKE_CURRENT_BINARY_DIR}/tmp/syntax.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/tmp/syntax.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/tmp/tokenUtils.o

        work/src/run/main.cpp
)

add_executable(
        do_not_compile

        work/src/lang/tokenUtils.c
)

target_include_directories(do_not_compile PRIVATE work/src/lang)
target_include_directories(do_not_compile PRIVATE work/src/pre)
