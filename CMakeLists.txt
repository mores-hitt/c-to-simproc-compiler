cmake_minimum_required(VERSION 3.20)
project(scc VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler warnings
set(SCC_WARNINGS
    -Wall -Wextra -Wpedantic
    -Wconversion -Wsign-conversion
    -Weffc++
)

add_library(scc_lib STATIC
    src/lexer.cpp
    src/token.cpp
)

target_include_directories(scc_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_compile_options(scc_lib PRIVATE ${SCC_WARNINGS})
target_compile_options(scc_lib PRIVATE
    "$<$<CONFIG:Debug>:-ggdb;-pedantic-errors;-Werror>"
    "$<$<CONFIG:Release>:-O3>"
)

# Main executable (just the entry point)
add_executable(scc src/main.cpp)
target_link_libraries(scc PRIVATE scc_lib)
target_compile_options(scc PRIVATE ${SCC_WARNINGS})
target_compile_options(scc PRIVATE
    "$<$<CONFIG:Debug>:-ggdb;-pedantic-errors;-Werror>"
    "$<$<CONFIG:Release>:-O3>"
)

target_compile_definitions(scc PRIVATE
    "$<$<CONFIG:Debug>:DEBUG_MODE>"
)

# Testing (optional)
option(SCC_BUILD_TESTS "Build tests" OFF)
if(SCC_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()