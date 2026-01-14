cmake_minimum_required(VERSION 3.20)
project(scc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES "src/*.cpp")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
file(COPY ${CMAKE_SOURCE_DIR}/examples DESTINATION ${CMAKE_BINARY_DIR})
endif()

add_executable(scc ${SOURCES})

target_compile_options(scc PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Weffc++
    -fdiagnostics-color=always

    $<$<CONFIG:Debug>:
        -ggdb
        -pedantic-errors
        -Werror
        -DDEBUG_MODE
    >
    $<$<CONFIG:Release>:
        -O3
    >
)