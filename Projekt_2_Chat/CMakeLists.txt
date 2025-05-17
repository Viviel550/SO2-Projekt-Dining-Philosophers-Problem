cmake_minimum_required(VERSION 3.10)
project(ChatApp)

set(CMAKE_CXX_STANDARD 17)

# Find PostgreSQL and libpqxx
find_package(PostgreSQL REQUIRED)
find_path(PQXX_INCLUDE_DIR pqxx/pqxx)
find_library(PQXX_LIBRARY pqxx)

# Include directories
include_directories(${PostgreSQL_INCLUDE_DIRS} ${PQXX_INCLUDE_DIR})

# Source files
file(GLOB_RECURSE SOURCES "*.cpp")

# Create executable
add_executable(chat_app ${SOURCES})

# Link libraries
target_link_libraries(chat_app ${PostgreSQL_LIBRARIES} ${PQXX_LIBRARY})