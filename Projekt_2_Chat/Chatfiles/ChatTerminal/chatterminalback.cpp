#include "chatterminalback.h"
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "../Config.h"
// Database connection string
const std::string CONNECTION_STRING = DBConfig::getConnectionString();

bool sendMessage(int senderId, int receiverId, const std::string& content) {
    try {
        pqxx::connection conn(CONNECTION_STRING);
        
        if (!conn.is_open()) {
            std::cerr << "Failed to open database connection\n";
            return false;
        }
        
        pqxx::work txn(conn);
        
        txn.exec_params(
            "INSERT INTO user_chat (sender_id, receiver_id, content) "
            "VALUES ($1, $2, $3)",
            senderId,
            receiverId,
            content
        );
        
        txn.commit();
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
        return false;
    }
}

std::vector<Message> getChatHistory(int userId, int otherUserId, int limit) {
    std::vector<Message> messages;
    
    try {
        pqxx::connection conn(CONNECTION_STRING);
        
        if (!conn.is_open()) {
            std::cerr << "Failed to open database connection\n";
            return messages;
        }
        
        pqxx::work txn(conn);
        
        // Get messages between the two users (in both directions)
        pqxx::result res = txn.exec_params(
            "SELECT sender_id, receiver_id, content, send_date "
            "FROM user_chat "
            "WHERE (sender_id = $1 AND receiver_id = $2) "
            "   OR (sender_id = $2 AND receiver_id = $1) "
            "ORDER BY send_date DESC "
            "LIMIT $3",
            userId,
            otherUserId,
            limit
        );
        
        for (const auto& row : res) {
            Message msg;
            msg.senderId = row["sender_id"].as<int>();
            msg.receiverId = row["receiver_id"].as<int>();
            msg.content = row["content"].as<std::string>();
            msg.timestamp = row["send_date"].as<std::string>();
            
            messages.push_back(msg);
        }
        
        txn.commit();
    }
    catch (const std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
    
    return messages;
}