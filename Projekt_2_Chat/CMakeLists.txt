cmake_minimum_required(VERSION 3.10)
project(ChatApp)

set(CMAKE_CXX_STANDARD 17)

# Find PostgreSQL and libpqxx
find_package(PostgreSQL REQUIRED)
find_path(PQXX_INCLUDE_DIR pqxx/pqxx)
find_library(PQXX_LIBRARY pqxx)

# Include directories
include_directories(${PostgreSQL_INCLUDE_DIRS} ${PQXX_INCLUDE_DIR})

# Source files - explicitly list them instead of using GLOB
set(SOURCES
    main.cpp
    Chatfiles/Login/login.cpp
    Chatfiles/Login/loginback.cpp
    Chatfiles/Register/register.cpp
    Chatfiles/Register/registerback.cpp
    Chatfiles/MainTerminal/mainterminal.cpp
    Chatfiles/MainTerminal/mainterminalback.cpp
    Chatfiles/ChatTerminal/chatterminal.cpp
    Chatfiles/ChatTerminal/chatterminalback.cpp
)

# Create executable
add_executable(chat_app ${SOURCES})

# Link libraries
target_link_libraries(chat_app ${PostgreSQL_LIBRARIES} ${PQXX_LIBRARY})