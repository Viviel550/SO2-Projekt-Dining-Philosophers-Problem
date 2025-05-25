#include "chatterminalback.h"
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "../Config.h"

bool sendMessage(int senderId, int receiverId, const std::string& content) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    try {
        pqxx::connection conn(CONNECTION_STRING);
        
        if (!conn.is_open()) {
            std::cerr << "Failed to open database connection\n";
            return false;
        }
        
        pqxx::work txn(conn);
        
        txn.exec(
            "INSERT INTO user_chat (sender_id, receiver_id, content) "
            "VALUES ($1, $2, $3)",
            pqxx::params(senderId, receiverId, content)
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
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    std::vector<Message> messages;
    try {
        pqxx::connection conn(CONNECTION_STRING);
        
        if (!conn.is_open()) {
            std::cerr << "Failed to open database connection\n";
            return messages;
        }
        
        pqxx::work txn(conn);
        
        pqxx::result res = txn.exec(
            "SELECT sender_id, receiver_id, content, send_date "
            "FROM user_chat "
            "WHERE (sender_id = $1 AND receiver_id = $2) "
            "   OR (sender_id = $2 AND receiver_id = $1) "
            "ORDER BY send_date ASC "
            "LIMIT $3",
            pqxx::params(userId, otherUserId, limit)
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

std::vector<Message> getOlderChatHistory(int userId, int otherUserId, int limit, int offset) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    std::vector<Message> messages;
    try {
        pqxx::connection conn(CONNECTION_STRING);
        
        if (!conn.is_open()) {
            std::cerr << "Failed to open database connection\n";
            return messages;
        }
        
        pqxx::work txn(conn);
        
        // Get older messages by using OFFSET to skip already loaded messages
        pqxx::result res = txn.exec(
            "SELECT sender_id, receiver_id, content, send_date "
            "FROM user_chat "
            "WHERE (sender_id = $1 AND receiver_id = $2) "
            "   OR (sender_id = $2 AND receiver_id = $1) "
            "ORDER BY send_date DESC "
            "LIMIT $3 OFFSET $4",
            pqxx::params(userId, otherUserId, limit, offset)
        );
        
        for (const auto& row : res) {
            Message msg;
            msg.senderId = row["sender_id"].as<int>();
            msg.receiverId = row["receiver_id"].as<int>();
            msg.content = row["content"].as<std::string>();
            msg.timestamp = row["send_date"].as<std::string>();
            
            messages.push_back(msg);
        }
        
        // Reverse to maintain chronological order (oldest first)
        std::reverse(messages.begin(), messages.end());
        
        txn.commit();
    }
    catch (const std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
    
    return messages;
}

std::vector<Message> getLatestChatHistory(int userId, int otherUserId, int limit) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    std::vector<Message> messages;
    try {
        pqxx::connection conn(CONNECTION_STRING);
        
        if (!conn.is_open()) {
            std::cerr << "Failed to open database connection\n";
            return messages;
        }
        
        pqxx::work txn(conn);
        
        // Get the most recent messages
        pqxx::result res = txn.exec(
            "SELECT sender_id, receiver_id, content, send_date "
            "FROM user_chat "
            "WHERE (sender_id = $1 AND receiver_id = $2) "
            "   OR (sender_id = $2 AND receiver_id = $1) "
            "ORDER BY send_date DESC "
            "LIMIT $3",
            pqxx::params(userId, otherUserId, limit)
        );
        
        for (const auto& row : res) {
            Message msg;
            msg.senderId = row["sender_id"].as<int>();
            msg.receiverId = row["receiver_id"].as<int>();
            msg.content = row["content"].as<std::string>();
            msg.timestamp = row["send_date"].as<std::string>();
            
            messages.push_back(msg);
        }
        
        // Reverse to maintain chronological order (oldest first)
        std::reverse(messages.begin(), messages.end());
        
        txn.commit();
    }
    catch (const std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
    
    return messages;
}