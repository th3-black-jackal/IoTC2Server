# Use GoogleTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(c2_server_tests
    C2ServerTests.cpp
    ../src/Logger.cpp
)

target_link_libraries(c2_server_tests
    gtest
    gmock
    gtest_main
    pthread
)

add_test(NAME C2ServerTests COMMAND c2_server_tests)
