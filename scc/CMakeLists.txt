# Compiler library with all components
add_library(scc_lib STATIC
    lexer/lexer.cpp
    lexer/token.cpp
)

target_include_directories(scc_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(scc_lib PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(scc_lib PRIVATE
        /W4
        /permissive-
        "$<$<CONFIG:Debug>:/Zi>"
        "$<$<CONFIG:Release>:/O2>"
    )
else()
    target_compile_options(scc_lib PRIVATE
        -Wall -Wextra -Wpedantic
        -Wconversion -Wsign-conversion
        -Weffc++
        "$<$<CONFIG:Debug>:-ggdb;-pedantic-errors;-Werror>"
        "$<$<CONFIG:Release>:-O3>"
    )
endif()

# Main executable
add_executable(scc main.cpp)
target_link_libraries(scc PRIVATE scc_lib)

target_compile_features(scc PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(scc PRIVATE
        /W4
        /permissive-
        "$<$<CONFIG:Debug>:/Zi>"
        "$<$<CONFIG:Release>:/O2>"
    )
else()
    target_compile_options(scc PRIVATE
        -Wall -Wextra -Wpedantic
        -Wconversion -Wsign-conversion
        -Weffc++
        "$<$<CONFIG:Debug>:-ggdb;-pedantic-errors;-Werror>"
        "$<$<CONFIG:Release>:-O3>"
    )
endif()

target_compile_definitions(scc PRIVATE
    "$<$<CONFIG:Debug>:DEBUG_MODE>"
)

# Install target
install(TARGETS scc DESTINATION bin)