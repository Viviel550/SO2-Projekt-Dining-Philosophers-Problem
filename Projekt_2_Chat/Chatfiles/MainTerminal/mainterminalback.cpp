#include "mainterminalback.h"
#include "../Config.h"
#include <iostream>
#include <pqxx/pqxx>
#include <string>

// Use the centralized connection string

int pickUserChat(const std::string& otherUserName, const std::string& otherUserNameId) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    try {
        pqxx::connection conn(CONNECTION_STRING);
        if (!conn.is_open()) {
            std::cerr << "Failed to open database connection\n";
            return -1;
        }
        pqxx::work txn(conn);
        // Get user ID using user_name and user_name_id
        pqxx::result res = txn.exec(
            "SELECT user_id FROM users WHERE user_name = $1 AND user_name_id = $2",
            pqxx::params(otherUserName, otherUserNameId) 
        );
        
        if (res.empty()) {
            return -1; // User not found
        }
        int otherUserId = res[0][0].as<int>();
        
        txn.commit();
        
        return otherUserId;
    }
    catch (const std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
        return -1;
    }
}

int pickChatRoom(const std::string& chatRoomName, const std::string& chatRoomPassword) {
    // Not implemented yet
    return 0;
}