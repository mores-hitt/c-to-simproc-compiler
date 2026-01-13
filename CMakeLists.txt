cmake_minimum_required(VERSION 3.20)
project(SimuprocCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(SimuprocCompiler ${SOURCES})

target_compile_options(SimuprocCompiler PRIVATE
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