include(FetchContent)

# 1. Download GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(sp-cli_tests
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

target_link_libraries(sp-cli_tests PRIVATE
    GTest::gtest_main
    sp-cli_lib
)

target_compile_options(sp-cli_tests PRIVATE ${SCC_WARNINGS})
target_compile_features(sp-cli_tests PRIVATE cxx_std_17)

gtest_discover_tests(sp-cli_tests)

add_executable(scc_tests
    main_test.cpp
    scc/ex_test.cpp
)

target_link_libraries(scc_tests PRIVATE
    GTest::gtest_main
    scc_lib
)

target_compile_options(sp-cli_tests PRIVATE ${SCC_WARNINGS})
target_compile_features(sp-cli_tests PRIVATE cxx_std_17)

gtest_discover_tests(scc_tests)