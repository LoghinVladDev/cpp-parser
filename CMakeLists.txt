cmake_minimum_required(VERSION 3.21)
project(cpp_parser)

set(CMAKE_CXX_STANDARD 23)

include_directories(work/src/dependencies/CDS/include)

include_directories(work)
include_directories(work/src)
include_directories(work/src/lang)
include_directories(work/src/lang/obj)
include_directories(work/src/lang/preprocessor)
include_directories(work/src/lang/interpreter)
include_directories(work/src/lang/error)
include_directories(work/src/pre)
include_directories(work/src/run)

add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/.clean

        COMMAND bash ../work/tools/clean.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

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

        work/src/lang/obj/State.cpp
        work/src/lang/obj/State.hpp

        work/src/lang/preprocessor/Preprocessor.cpp
        work/src/lang/preprocessor/Preprocessor.hpp

        work/src/lang/preprocessor/Define.cpp
        work/src/lang/preprocessor/Define.hpp

        work/src/lang/interpreter/Interpreter.cpp
        work/src/lang/interpreter/Interpreter.hpp

        work/src/lang/error/Error.cpp
        work/src/lang/error/Error.hpp

        work/src/lang/error/Warning.cpp
        work/src/lang/error/Warning.hpp

        work/src/run/main.cpp
)

add_library(
        do_not_compile

        work/src/lang/tokenUtils.c
)

add_executable(
        _clean

        work/src/lang/tokenUtils.c

        ${CMAKE_CURRENT_BINARY_DIR}/.clean
)

target_include_directories(do_not_compile PRIVATE work/src/lang)
target_include_directories(do_not_compile PRIVATE work/src/pre)
