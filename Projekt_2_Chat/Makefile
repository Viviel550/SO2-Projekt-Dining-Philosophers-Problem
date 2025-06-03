CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
LDFLAGS = -lpqxx -lpq -lncursesw -lwinmm -lws2_32

SRCDIR = .
OBJDIR = build
BINDIR = bin

# Define all source files explicitly to match CMakeLists.txt
SOURCES = main.cpp \
          Chatfiles/Login/login.cpp \
          Chatfiles/Login/loginback.cpp \
          Chatfiles/Register/register.cpp \
          Chatfiles/Register/registerback.cpp \
          Chatfiles/MainTerminal/mainterminal.cpp \
          Chatfiles/MainTerminal/mainterminalback.cpp \
          Chatfiles/ChatTerminal/chatterminal.cpp \
          Chatfiles/ChatTerminal/chatterminalback.cpp \
          Chatfiles/ChatTerminal/chatcommands.cpp \
          Chatfiles/FriendList/friendlist.cpp \
          Chatfiles/FriendList/friendlistback.cpp \
          Chatfiles/Database/AsyncQueryManager.cpp \
          Chatfiles/Database/ConnectionPool.cpp

OBJECTS := $(SOURCES:%.cpp=$(OBJDIR)/%.o)
EXECUTABLE = $(BINDIR)/chat_app

# Default target
all: $(EXECUTABLE)

# Create executable
$(EXECUTABLE): $(OBJECTS)
    @mkdir -p $(@D)
    $(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile source files to object files
$(OBJDIR)/%.o: %.cpp
    @mkdir -p $(@D)
    $(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
    rm -rf $(OBJDIR) $(BINDIR)

# Install dependencies (for MSYS2/MinGW64)
install-deps:
    @echo "Installing dependencies with pacman..."
    pacman -S mingw-w64-x86_64-postgresql mingw-w64-x86_64-libpqxx mingw-w64-x86_64-ncurses

# Help target
help:
    @echo "Available targets:"
    @echo "  all         - Build the chat application"
    @echo "  clean       - Remove build artifacts"
    @echo "  install-deps- Install required dependencies (MSYS2/MinGW64)"
    @echo "  help        - Show this help message"

.PHONY: all clean install-deps help