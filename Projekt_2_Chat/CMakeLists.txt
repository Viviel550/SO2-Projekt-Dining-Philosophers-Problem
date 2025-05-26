cmake_minimum_required(VERSION 3.10)
project(ChatApp)

set(CMAKE_CXX_STANDARD 17)

# Find PostgreSQL and libpqxx
find_package(PostgreSQL REQUIRED)
find_path(PQXX_INCLUDE_DIR pqxx/pqxx)
find_library(PQXX_LIBRARY pqxx)

# Find NCurses for MSYS2 MINGW64 - use the same path structure that works
find_library(CURSES_LIBRARY 
    NAMES ncursesw ncurses
    PATHS 
        ${CMAKE_PREFIX_PATH}/lib
        /mingw64/lib
        /usr/local/lib
        /usr/lib
)

# Look for ncurses.h in the ncurses subdirectory (same as your working test)
find_path(CURSES_INCLUDE_DIR 
    NAMES ncurses/ncurses.h
    PATHS 
        ${CMAKE_PREFIX_PATH}/include
        /mingw64/include
        /usr/local/include
        /usr/include
)

# Check if NCurses was found
if(NOT CURSES_LIBRARY OR NOT CURSES_INCLUDE_DIR)
    message(FATAL_ERROR "NCurses library not found. Install with: pacman -S mingw-w64-x86_64-ncurses")
endif()

message(STATUS "Found NCurses library: ${CURSES_LIBRARY}")
message(STATUS "Found NCurses include: ${CURSES_INCLUDE_DIR}")

# Include directories
include_directories(${PostgreSQL_INCLUDE_DIRS} ${PQXX_INCLUDE_DIR} ${CURSES_INCLUDE_DIR})

# Source files
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
    Chatfiles/ChatTerminal/chatcommands.cpp
    Chatfiles/FriendList/friendlist.cpp
    Chatfiles/FriendList/friendlistback.cpp
)

# Create executable
add_executable(chat_app ${SOURCES})

# Link libraries - use the same library that worked in your test
target_link_libraries(chat_app 
    ${PostgreSQL_LIBRARIES} 
    ${PQXX_LIBRARY}
    ncursesw
    winmm
    ws2_32
)