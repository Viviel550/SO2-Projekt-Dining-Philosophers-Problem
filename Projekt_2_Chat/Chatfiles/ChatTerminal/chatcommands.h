#ifndef CHATCOMMANDS_H
#define CHATCOMMANDS_H

#include <string>
#include <vector>
#include <mutex>

// Forward declaration for Message struct
struct Message;

// Command handler functions
bool handleExitCommand(bool& chatting);
bool handleHelpCommand(int userId, int otherUserId, std::string& currentInput);
bool handleClearCommand();
bool handleUnknownCommand(const std::string& command, int userId, int otherUserId, std::string& currentInput);
bool handleStatusCommand(int userId, int otherUserId, std::string& currentInput);
bool handleTimeCommand(int userId, int otherUserId, std::string& currentInput);

// Main command processor
bool processCommand(const std::string& command, int userId, int otherUserId, std::string& currentInput, bool& chatting, bool& needsRefresh);

// Utility functions
std::string getCurrentTimestamp();

// External variables that commands need access to
extern std::vector<Message> globalChatHistory;
extern std::mutex chatMutex;
extern int scrollPosition;
extern int totalLoadedMessages;

#endif // CHATCOMMANDS_H