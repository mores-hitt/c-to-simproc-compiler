# Compiler library with all components
add_library(scc_lib STATIC
    lexer/lexer.cpp
    lexer/token.cpp
    # Add more as you develop:
    # parser/parser.cpp
    # parser/ast.cpp
    # codegen/codegen.cpp
    # utils/error.cpp
)

# Include the compiler directory so headers can be included as:
# #include "lexer/lexer.h"
target_include_directories(scc_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(scc_lib PUBLIC cxx_std_17)

target_compile_options(scc_lib PRIVATE ${SCC_WARNINGS})
target_compile_options(scc_lib PRIVATE
    "$<$<CONFIG:Debug>:-ggdb;-pedantic-errors;-Werror>"
    "$<$<CONFIG:Release>:-O3>"
)

# Main executable
add_executable(scc main.cpp)
target_link_libraries(scc PRIVATE scc_lib)

target_compile_features(scc PUBLIC cxx_std_17)

target_compile_options(scc PRIVATE ${SCC_WARNINGS})
target_compile_options(scc PRIVATE
    "$<$<CONFIG:Debug>:-ggdb;-pedantic-errors;-Werror>"
    "$<$<CONFIG:Release>:-O3>"
)

target_compile_definitions(scc PRIVATE
    "$<$<CONFIG:Debug>:DEBUG_MODE>"
)

# Install target
install(TARGETS scc DESTINATION bin)