#ifndef CHATTERMINALBACK_H
#define CHATTERMINALBACK_H
#include <string>
#include <vector>

// Structure to hold message data
struct Message {
    int senderId;
    int receiverId;
    std::string content;
    std::string timestamp;
};

// Send a message from one user to another
bool sendMessage(int senderId, int receiverId, const std::string& content);

// Get chat history between two users
std::vector<Message> getChatHistory(int userId, int otherUserId, int limit = 20);

#endif // CHATTERMINALBACK_H