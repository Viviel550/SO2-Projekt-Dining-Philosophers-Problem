#include "loginback.h"
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include "../Config.h"

// Authnticate user with email and password
bool authenticateUser(const std::string& email, const std::string& password) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    try {
        pqxx::connection conn(CONNECTION_STRING);
        
        if (!conn.is_open()) {
            std::cerr << "Failed to open database connection\n";
            return false;
        }
        
        pqxx::work txn(conn);
        
        // Check if email and password match
        pqxx::result res = txn.exec(
            "SELECT COUNT(*) FROM users WHERE email = $1 AND password = $2",
            pqxx::params(email, password)
        );
        
        int count = res[0][0].as<int>();
        
        txn.commit();
        
        return count > 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
        return false;
    }
}

// Get user ID by email
UserData getUserIdByEmail(const std::string& email) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    UserData User = {-1, -1, ""}; // Default values for userId and userNameId
    try {
        pqxx::connection conn(CONNECTION_STRING);
        
        if (!conn.is_open()) {
            std::cerr << "Failed to open database connection\n";
            return User;
        }
        
        pqxx::work txn(conn);
        
        pqxx::result res = txn.exec(
            "SELECT user_id, user_name_id, user_name FROM users WHERE email = $1",
            pqxx::params(email)
        );
        
        if (res.empty()) {
            return User; // User not found
        }
        
        User.UserId = res[0][0].as<int>();
        User.userNameId = res[0][1].as<int>();
        User.userName = res[0][2].as<std::string>();
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
    return User;
}