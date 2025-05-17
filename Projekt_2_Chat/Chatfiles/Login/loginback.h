#ifndef LOGINBACK_H
#define LOGINBACK_H
#include <string>

// Authenticate user with email and password
bool authenticateUser(const std::string& email, const std::string& password);

// Get user ID using email
int getUserIdByEmail(const std::string& email);

#endif