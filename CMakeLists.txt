cmake_minimum_required(VERSION 3.10)
project(C2Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
find_package(SQLite3 REQUIRED)

# Add include path
include_directories(${SQLite3_INCLUDE_DIRS} include models respositories)
add_compile_options(-Wall -Wextra -pedantic)

# Source files
add_executable(c2_server
    src/ConfigManager.cpp
    src/SQLiteConnection.cpp
    repositories/ClientRepository.cpp
    models/Client.cpp
    src/TcpSocket.cpp
    src/C2Server.cpp
    src/Logger.cpp

    main.cpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
target_link_libraries(c2_server ${SQLite3_LIBRARIES})


# Tests
enable_testing()
add_subdirectory(tests)
