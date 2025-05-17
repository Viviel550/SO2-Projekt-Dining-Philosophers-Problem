#include "registerback.h"
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <random>

// Database connection string
const std::string CONNECTION_STRING = "dbname=Chat user=postgres password=123 hostaddr=127.0.0.1 port=5432";

// Check if a username already exists in the database
bool checkUsernameExists(const std::string& username) {
    try {
        pqxx::connection conn(CONNECTION_STRING);
        
        if (!conn.is_open()) {
            std::cerr << "Failed to open database connection\n";
            return false;
        }
        
        pqxx::work txn(conn);
        
        pqxx::result res = txn.exec_params(
            "SELECT COUNT(*) FROM users WHERE user_name = $1",
            username
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

// Generate a unique user_name_id (numeric suffix)
std::string generateUserNameId(const std::string& username) {
    // Create a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1000, 9999);
    
    // Generate a 4-digit random number
    return std::to_string(distrib(gen));
}

// Register a new user in the database
bool registerUser(const std::string& username, const std::string& password) {
    try {
        pqxx::connection conn(CONNECTION_STRING);
        
        if (!conn.is_open()) {
            std::cerr << "Failed to open database connection\n";
            return false;
        }
        
        std::string userNameId = generateUserNameId(username);
        
        pqxx::work txn(conn);
        
        txn.exec_params(
            "INSERT INTO users (user_name, user_name_id, password) VALUES ($1, $2, $3)",
            username,
            userNameId,
            password  // Note: In a real app, you should hash passwords!
        );
        
        txn.commit();
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
        return false;
    }
}