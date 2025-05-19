#ifndef LOGINBACK_H
#define LOGINBACK_H
#include <string>

struct UserData {
    int UserId;
    int userNameId;
    std::string userName;
};

// Authenticate user with email and password
bool authenticateUser(const std::string& email, const std::string& password);

// Get user ID using email
UserData getUserIdByEmail(const std::string& email);

#endif