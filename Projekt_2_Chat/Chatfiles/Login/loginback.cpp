#include "loginback.h"
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include "../Config.h"

const std::string CONNECTION_STRING = DBConfig::getConnectionString();

// Authenticate user with email and password
bool authenticateUser(const std::string& email, const std::string& password) {
    try {
        pqxx::connection conn(CONNECTION_STRING);
        
        if (!conn.is_open()) {
            std::cerr << "Failed to open database connection\n";
            return false;
        }
        
        pqxx::work txn(conn);
        
        // Check if email and password match
        pqxx::result res = txn.exec_params(
            "SELECT COUNT(*) FROM users WHERE email = $1 AND password = $2",
            email,
            password  // Note: In a real app, you should hash passwords!
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
int getUserIdByEmail(const std::string& email) {
    try {
        pqxx::connection conn(CONNECTION_STRING);
        
        if (!conn.is_open()) {
            std::cerr << "Failed to open database connection\n";
            return -1;
        }
        
        pqxx::work txn(conn);
        
        pqxx::result res = txn.exec_params(
            "SELECT user_id FROM users WHERE email = $1",
            email
        );
        
        if (res.empty()) {
            return -1; // User not found
        }
        
        int userId = res[0][0].as<int>();
        
        txn.commit();
        
        return userId;
    }
    catch (const std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
        return -1;
    }
}