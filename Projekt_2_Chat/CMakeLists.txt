cmake_minimum_required(VERSION 3.10)
project(TerminalChat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use pkg-config to find libpqxx
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

# Create directory structure if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Chatfiles/Register)

# Add the executable
add_executable(chat
    main.cpp
    Chatfiles/Register/register.cpp
    Chatfiles/Register/registerback.cpp
)

# Include directories
include_directories(${PQXX_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Link the libraries
target_link_libraries(chat PRIVATE ${PQXX_LIBRARIES})