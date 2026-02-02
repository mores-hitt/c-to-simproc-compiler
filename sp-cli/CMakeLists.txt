add_library(sp-cli_lib STATIC
    register/address.cpp
    register/general_purpose.cpp
    register/flags.cpp
    memory/ram.cpp
    cpu/cpu.cpp
)

target_include_directories(sp-cli_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

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

target_compile_definitions(sp-cli_lib PUBLIC
    "$<$<CONFIG:Debug>:DEBUG_MODE>"
)

add_executable(sp-cli main.cpp)
target_link_libraries(sp-cli PRIVATE sp-cli_lib)

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

target_compile_definitions(sp-cli PRIVATE
    "$<$<CONFIG:Debug>:DEBUG_MODE>"
)