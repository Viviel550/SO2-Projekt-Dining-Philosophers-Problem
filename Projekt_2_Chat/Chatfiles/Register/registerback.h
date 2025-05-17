#ifndef REGISTERBACK_H
#define REGISTERBACK_H

#include <string>

bool checkUsernameExists(const std::string& username);
bool registerUser(const std::string& username, const std::string& password);

#endif