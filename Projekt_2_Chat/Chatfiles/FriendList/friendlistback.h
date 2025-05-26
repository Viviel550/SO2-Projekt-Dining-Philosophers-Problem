#ifndef FRIENDLISTBACK_H
#define FRIENDLISTBACK_H

#include <string>
#include <vector>

struct FriendInfo {
    int userId;
    std::string nickname;
    std::string userNameId;
    bool hasNewMessages = false;
};

// Friend management functions
bool sendFriendRequest(int senderId, int receiverId);
bool acceptFriendRequest(int userId1, int userId2);
bool removeFriend(int userId1, int userId2);
std::vector<FriendInfo> getFriendsList(int userId);
int findUserByNicknameAndId(const std::string& nickname, const std::string& userNameId);
bool hasNewMessagesFrom(int userId, int otherUserId);
void markMessagesAsRead(int userId, int otherUserId);
#endif // FRIENDLISTBACK_H