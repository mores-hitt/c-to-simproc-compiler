include(FetchContent)

# 1. Download GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    main_test.cpp
    sp-cli/cpu_test.cpp
    sp-cli/memory_test.cpp
    sp-cli/cmp/equal_test.cpp
    sp-cli/cmp/bigger_test.cpp
    sp-cli/cmp/smaller_test.cpp
    sp-cli/float/add/medium_normal_test.cpp
    sp-cli/float/add/medium_zero_test.cpp
    sp-cli/float/add/medium_almost_zero_test.cpp
)

target_link_libraries(unit_tests PRIVATE
    GTest::gtest_main
    scc_lib
    sp-cli_lib
)

target_compile_options(unit_tests PRIVATE ${SCC_WARNINGS})
target_compile_features(unit_tests PRIVATE cxx_std_17)

include(GoogleTest)
gtest_discover_tests(unit_tests)