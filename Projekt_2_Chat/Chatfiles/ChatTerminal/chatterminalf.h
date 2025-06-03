#ifndef CHATTERMINALF_H
#define CHATTERMINALF_H

#include <string>

// Show chat terminal for direct messaging between users
int showChatTerminal(int userId, int otherUserId, const std::string& userName, const std::string& userNameId, const std::string& otherUserName = "", const std::string& otherUserNameId = "");

#endif // CHATTERMINALF_H