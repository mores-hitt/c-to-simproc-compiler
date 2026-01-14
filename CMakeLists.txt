cmake_minimum_required(VERSION 3.20)
project(scc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(scc ${SOURCES})

target_compile_options(scc PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Weffc++
    -fdiagnostics-color=never

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