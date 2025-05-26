#include "chatcommands.h"
#include "chatterminalback.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <mutex>

extern std::vector<Message> globalChatHistory;
extern std::mutex chatMutex;
extern int scrollPosition;
extern int totalLoadedMessages;

// Utility function to get current timestamp
std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
    return std::string(buffer);
}

// Exit command handler
bool handleExitCommand(bool& chatting) {
    chatting = false;
    return true;
}

// Help command handler
bool handleHelpCommand(int userId, int otherUserId, std::string& currentInput) {
    // Create a local message for help
    Message helpMsg;
    helpMsg.senderId = userId;
    helpMsg.content = "Available commands: /exit, /quit, /help, /clear, /status, /time";
    helpMsg.timestamp = getCurrentTimestamp();
    
    // Add to local chat history only (not database)
    {
        std::lock_guard<std::mutex> lock(chatMutex);
        globalChatHistory.push_back(helpMsg);
    }
    
    currentInput = "";
    return true;
}

// Clear command handler
bool handleClearCommand() {
    // Clear the chat display (not the database)
    std::lock_guard<std::mutex> lock(chatMutex);
    globalChatHistory.clear();
    totalLoadedMessages = 0;
    scrollPosition = 0;
    return true;
}

// Status command handler
bool handleStatusCommand(int userId, int otherUserId, std::string& currentInput) {
    // Create a local status message
    Message statusMsg;
    statusMsg.senderId = userId;
    
    std::lock_guard<std::mutex> lock(chatMutex);
    int messageCount = globalChatHistory.size();
    statusMsg.content = "Chat status: " + std::to_string(messageCount) + " messages loaded, scroll position: " + std::to_string(scrollPosition);
    statusMsg.timestamp = getCurrentTimestamp();
    
    globalChatHistory.push_back(statusMsg);
    currentInput = "";
    return true;
}

// Time command handler
bool handleTimeCommand(int userId, int otherUserId, std::string& currentInput) {
    // Create a local time message
    Message timeMsg;
    timeMsg.senderId = userId;
    timeMsg.content = "Current time: " + getCurrentTimestamp();
    timeMsg.timestamp = getCurrentTimestamp();
    
    {
        std::lock_guard<std::mutex> lock(chatMutex);
        globalChatHistory.push_back(timeMsg);
    }
    
    currentInput = "";
    return true;
}

// Unknown command handler
bool handleUnknownCommand(const std::string& command, int userId, int otherUserId, std::string& currentInput) {
    // Create a local error message
    Message errorMsg;
    errorMsg.senderId = userId;
    errorMsg.content = "Unknown command: " + command + ". Type /help for available commands.";
    errorMsg.timestamp = getCurrentTimestamp();
    
    // Add to local chat history only (not database)
    {
        std::lock_guard<std::mutex> lock(chatMutex);
        globalChatHistory.push_back(errorMsg);
    }
    
    currentInput = "";
    return true;
}

// Main command processor
bool processCommand(const std::string& command, int userId, int otherUserId, std::string& currentInput, bool& chatting, bool& needsRefresh) {
    if (command.empty() || command[0] != '/') {
        return false; // Not a command
    }
    
    // Extract command name (everything before first space)
    std::string cmdName = command;
    std::string cmdArgs = "";
    
    size_t spacePos = command.find(' ');
    if (spacePos != std::string::npos) {
        cmdName = command.substr(0, spacePos);
        cmdArgs = command.substr(spacePos + 1);
    }
    
    // Process different commands
    if (cmdName == "/exit" || cmdName == "/quit") {
        return handleExitCommand(chatting);
    }
    else if (cmdName == "/help") {
        bool result = handleHelpCommand(userId, otherUserId, currentInput);
        if (result) {
            needsRefresh = true;
        }
        return result;
    }
    else if (cmdName == "/clear") {
        bool result = handleClearCommand();
        if (result) {
            needsRefresh = true;
        }
        return result;
    }
    else if (cmdName == "/status") {
        bool result = handleStatusCommand(userId, otherUserId, currentInput);
        if (result) {
            needsRefresh = true;
        }
        return result;
    }
    else if (cmdName == "/time") {
        bool result = handleTimeCommand(userId, otherUserId, currentInput);
        if (result) {
            needsRefresh = true;
        }
        return result;
    }
    else {
        // Unknown command
        bool result = handleUnknownCommand(command, userId, otherUserId, currentInput);
        if (result) {
            needsRefresh = true;
        }
        return result;
    }
}