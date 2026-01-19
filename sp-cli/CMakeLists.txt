add_library(sp-cli_lib STATIC
    #register/address.cpp
    #register/general_purpose.cpp
    memory/ram.cpp
)

target_include_directories(sp-cli_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(sp-cli_lib PRIVATE ${SCC_WARNINGS})
target_compile_options(sp-cli_lib PRIVATE
    "$<$<CONFIG:Debug>:-ggdb;-pedantic-errors;-Werror>"
    "$<$<CONFIG:Release>:-O3>"
)

target_compile_definitions(sp-cli_lib PUBLIC
    "$<$<CONFIG:Debug>:DEBUG_MODE>"
)

add_executable(sp-cli main.cpp)
target_link_libraries(sp-cli PRIVATE sp-cli_lib)

target_compile_options(sp-cli PRIVATE ${SCC_WARNINGS})
target_compile_options(sp-cli PRIVATE
    "$<$<CONFIG:Debug>:-ggdb;-pedantic-errors;-Werror>"
    "$<$<CONFIG:Release>:-O3>"
)

target_compile_definitions(sp-cli PRIVATE
    "$<$<CONFIG:Debug>:DEBUG_MODE>"
)