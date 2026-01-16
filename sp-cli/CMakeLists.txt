# Emulator library with all components
#add_library(sp-cli_lib STATIC
    #cpu/cpu.cpp
    #memory/memory.cpp
    #instructions/instructions.cpp
    # Add more as needed:
    # io/io.cpp
    # debugger/debugger.cpp
#)

# Include the emulator directory so headers can be included as:
# #include "cpu/cpu.h"
#target_include_directories(sp-cli_lib PUBLIC 
#    ${CMAKE_CURRENT_SOURCE_DIR}
#)

#target_compile_options(simuproc_lib PRIVATE ${SCC_WARNINGS})
#target_compile_options(simuproc_lib PRIVATE
#    "$<$<CONFIG:Debug>:-ggdb;-pedantic-errors;-Werror>"
#    "$<$<CONFIG:Release>:-O3>"
#)

# Main executable
add_executable(sp-cli main.cpp)
#target_link_libraries(sp-cli PRIVATE sp-cli_lib)

target_compile_options(sp-cli PRIVATE ${SCC_WARNINGS})
target_compile_options(sp-cli PRIVATE
    "$<$<CONFIG:Debug>:-ggdb;-pedantic-errors;-Werror>"
    "$<$<CONFIG:Release>:-O3>"
)

target_compile_definitions(sp-cli PRIVATE
    "$<$<CONFIG:Debug>:DEBUG_MODE>"
)

# Install target
install(TARGETS sp-cli DESTINATION bin)