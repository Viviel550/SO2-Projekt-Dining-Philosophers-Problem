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
    bool operator==(const Message& other) const {
        return senderId == other.senderId &&
               receiverId == other.receiverId &&
               content == other.content &&
               timestamp == other.timestamp;
    }
};

struct UserInfo {
    int userId;
    std::string userName;
    std::string userNameId;
};


// Send a message from one user to another
bool sendMessage(int senderId, int receiverId, const std::string& content);

// Get chat history between two users
std::vector<Message> getChatHistory(int userId, int otherUserId, int limit);
std::vector<Message> getOlderChatHistory(int userId, int otherUserId, int limit, int offset);
std::vector<Message> getLatestChatHistory(int userId, int otherUserId, int limit);
UserInfo getUserInfo(int userId);

#endif // CHATTERMINALBACK_H