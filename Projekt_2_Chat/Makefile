CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -lpqxx -lpq

SRCDIR = .
OBJDIR = build
BINDIR = bin

SOURCES := $(shell find $(SRCDIR) -name "*.cpp")
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
EXECUTABLE = $(BINDIR)/chat_app

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
    @mkdir -p $(@D)
    $(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
    @mkdir -p $(@D)
    $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
    rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean