cmake_minimum_required(VERSION 3.21)
project(cpp_parser)

set(CMAKE_CXX_STANDARD 23)

include_directories(work/src/dependencies/CDS/include)

add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/tmp/scanner.o
            ${CMAKE_CURRENT_BINARY_DIR}/tmp/syntax.cpp
            ${CMAKE_CURRENT_BINARY_DIR}/tmp/syntax.hpp
        COMMAND bash ../work/tools/clean.sh
        COMMAND bash ../work/tools/pre-build.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(
        cpp_parser

        ${CMAKE_CURRENT_BINARY_DIR}/tmp/scanner.o
        ${CMAKE_CURRENT_BINARY_DIR}/tmp/syntax.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/tmp/syntax.hpp

        work/src/run/main.cpp
)
