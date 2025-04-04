cmake_minimum_required(VERSION 3.10)
project(C2Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Add include path
include_directories(include)

# Source files
add_executable(c2_server
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


# Tests
enable_testing()
add_subdirectory(tests)
