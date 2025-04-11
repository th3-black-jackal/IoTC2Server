# Use GoogleTest
find_package(GTest REQUIRED)
find_package(SQLite3 REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})




add_executable(c2_server_tests
    ../src/ConfigManager.cpp
    ../src/SQLiteConnection.cpp
    ../repositories/ClientRepository.cpp
    ../models/Client.cpp
    ../src/Logger.cpp
    C2ServerTests.cpp
    ClientRepositoryTests.cpp
    
)


target_link_libraries(c2_server_tests
    gtest
    gmock
    gtest_main
    pthread
    ${SQLite3_LIBRARIES}
)

add_test(NAME C2ServerTests COMMAND c2_server_tests)
