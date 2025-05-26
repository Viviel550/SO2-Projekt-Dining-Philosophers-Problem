#include "registerback.h"
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <random>
#include "../Config.h" 

// Database connection string
// Check if a username already exists in the database
bool checkEmailExists(const std::string& email) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    try {
        pqxx::connection conn(CONNECTION_STRING);
        
        if (!conn.is_open()) {
            std::cerr << "Failed to open database connection\n";
            return false;
        }
        
        pqxx::work txn(conn);
        
        pqxx::result res = txn.exec(
            "SELECT COUNT(*) FROM users WHERE email = $1",
            pqxx::params(email)
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
    // Create a random number generator, come back in the future to make sure generated id is unique for the user
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1000, 9999);
    
    // Generate a 4-digit random number
    return std::to_string(distrib(gen));
}

// Register a new user in the database
bool registerUser(const std::string& username, const std::string& password, const std::string& email) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    try {
        pqxx::connection conn(CONNECTION_STRING);
        
        if (!conn.is_open()) {
            std::cerr << "Failed to open database connection\n";
            return false;
        }
        
        std::string userNameId = generateUserNameId(username);
        
        pqxx::work txn(conn);
        
        txn.exec(
            "INSERT INTO users (user_name, user_name_id, password, email) VALUES ($1, $2, $3, $4)",
            pqxx::params(username, userNameId, password, email)
        );
        
        txn.commit();
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
        return false;
    }
}