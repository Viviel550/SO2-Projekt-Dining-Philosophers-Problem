#include "friendlistback.h"
#include "../Database/AsyncQueryManager.h"
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>
#include <thread>
#include "../Config.h"

void debugLog(const std::string& message) {
    std::ofstream logFile("debug.log", std::ios::app);
    if (logFile.is_open()) {
        auto timeNow = std::time(nullptr);
        logFile << "[" << std::ctime(&timeNow) << "] " << message << std::endl;
        logFile.close();
    } else {
        std::cerr << "Failed to open debug log file." << std::endl;
    }
}

bool sendFriendRequest(int senderId, int receiverId) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    try {
        pqxx::connection conn(CONNECTION_STRING);
        pqxx::work txn(conn);
        
        debugLog("sendFriend: Sender=" + std::to_string(senderId) + ", Receiver=" + std::to_string(receiverId));
        
        // Ensure senderId != receiverId
        if (senderId == receiverId) {
            debugLog("sendFriend FAILED: Cannot add yourself");
            return false;
        }
        
        // Determine the correct order manually (avoid LEAST/GREATEST with mixed types)
        int user1, user2;
        if (senderId < receiverId) {
            user1 = senderId;
            user2 = receiverId;
        } else {
            user1 = receiverId;
            user2 = senderId;
        }
        
        debugLog("sendFriend: Checking friendship user1=" + std::to_string(user1) + ", user2=" + std::to_string(user2));
        
        // Check if friendship already exists
        auto checkResult = txn.exec(
            "SELECT COUNT(*) FROM friends WHERE user_1 = $1 AND user_2 = $2",
            pqxx::params(user1, user2)
        );
        int count = checkResult[0][0].as<int>();  
        debugLog("sendFriend: Existing friendships found: " + std::to_string(count));
        
        if (count > 0) {
            debugLog("sendFriend FAILED: Friendship already exists");
            return false;
        }
        
        // Insert new friendship
        auto insertResult = txn.exec(
            "INSERT INTO friends (user_1, user_2, accepted) VALUES ($1, $2, TRUE)",
            pqxx::params(user1, user2)
        );
        
        debugLog("sendFriend: Inserted " + std::to_string(insertResult.affected_rows()) + " rows");
        txn.commit();
        debugLog("sendFriend SUCCESS: Friendship added");
        return true;
    } catch (const std::exception& e) {
        debugLog("sendFriend ERROR: " + std::string(e.what()));
        return false;
    }
}

bool acceptFriendRequest(int userId1, int userId2) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    try {
        pqxx::connection conn(CONNECTION_STRING);
        pqxx::work txn(conn);
        
        // Determine correct order
        int user1, user2;
        if (userId1 < userId2) {
            user1 = userId1;
            user2 = userId2;
        } else {
            user1 = userId2;
            user2 = userId1;
        }
        
        auto result = txn.exec(
            "UPDATE friends "
            "SET accepted = TRUE, accepted_at = CURRENT_TIMESTAMP "
            "WHERE user_1 = $1 AND user_2 = $2 AND accepted = FALSE",
            pqxx::params(user1, user2)
        );
        
        txn.commit();
        return result.affected_rows() > 0;
    } catch (const std::exception& e) {
        debugLog("acceptFriend ERROR: " + std::string(e.what()));
        return false;
    }
}

bool removeFriend(int userId1, int userId2) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    try {
        pqxx::connection conn(CONNECTION_STRING);
        pqxx::work txn(conn);
        
        // Determine correct order
        int user1, user2;
        if (userId1 < userId2) {
            user1 = userId1;
            user2 = userId2;        
        } else {
            user1 = userId2;
            user2 = userId1;
        }
        
        auto result = txn.exec(
            "DELETE FROM friends WHERE user_1 = $1 AND user_2 = $2",
            pqxx::params(user1, user2)
        );
        
        txn.commit();
        return result.affected_rows() > 0;
    } catch (const std::exception& e) {
        debugLog("removeFriend ERROR: " + std::string(e.what()));
        return false;
    }
}

bool hasNewMessagesFrom(int userId, int friendId) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    try {
        pqxx::connection conn(CONNECTION_STRING);
        pqxx::work txn(conn);
        
        // Check for unread messages from this friend
        auto result = txn.exec(
            "SELECT COUNT(*) FROM user_chat "
            "WHERE sender_id = $1 AND receiver_id = $2 AND read_status = FALSE",
            pqxx::params(friendId, userId)
        );
        
        int unreadCount = result[0][0].as<int>();
        txn.commit();
        
        return unreadCount > 0;
    } catch (const std::exception& e) {
        debugLog("hasNewMessagesFrom ERROR: " + std::string(e.what()));
        return false;
    }
}

void markMessagesAsRead(int userId, int friendId) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    try {
        pqxx::connection conn(CONNECTION_STRING);
        pqxx::work txn(conn);
        
        // Mark all messages from this friend as read
        txn.exec(
            "UPDATE user_chat SET read_status = TRUE "
            "WHERE sender_id = $1 AND receiver_id = $2 AND read_status = FALSE",
            pqxx::params(friendId, userId)
        );
        
        txn.commit();
        debugLog("markMessagesAsRead: Marked messages as read from friend " + std::to_string(friendId));
    } catch (const std::exception& e) {
        debugLog("markMessagesAsRead ERROR: " + std::string(e.what()));
    }
}

// NEW: Async mark messages as read using detached thread
void markMessagesAsReadAsync(int userId, int friendId) {
    std::thread([userId, friendId]() {
        try {
            std::string CONNECTION_STRING = DBConfig::getConnectionString();
            pqxx::connection conn(CONNECTION_STRING);
            pqxx::work txn(conn);
            txn.exec(
                "UPDATE user_chat SET read_status = TRUE "
                "WHERE sender_id = $1 AND receiver_id = $2 AND read_status = FALSE",
                pqxx::params(friendId, userId)
            );
            txn.commit();
            debugLog("markMessagesAsReadAsync: Marked messages as read from friend " + std::to_string(friendId));
        } catch (const std::exception& e) {
            debugLog("markMessagesAsReadAsync ERROR: " + std::string(e.what()));
        }
    }).detach();
}

void markMessagesAsReadAsyncWithManager(AsyncQueryManager* queryManager, int userId, int friendId) {
    if (queryManager) {
        queryManager->markMessagesAsReadAsync(userId, friendId);
    } else {
        // Fallback to simple async version
        markMessagesAsReadAsync(userId, friendId);
    }
}

std::vector<FriendInfo> getFriendsList(int userId) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    std::vector<FriendInfo> friends;
    
    try {
        pqxx::connection conn(CONNECTION_STRING);
        pqxx::work txn(conn);
        
        auto result = txn.exec(
            "SELECT "
            "  CASE WHEN f.user_1 = $1 THEN f.user_2 ELSE f.user_1 END as friend_id, "
            "  u.user_name, "
            "  u.user_name_id "
            "FROM friends f "
            "JOIN users u ON ("
            "  CASE WHEN f.user_1 = $1 THEN f.user_2 = u.user_id ELSE f.user_1 = u.user_id END"
            ") "
            "WHERE (f.user_1 = $1 OR f.user_2 = $1) AND f.accepted = TRUE "
            "ORDER BY u.user_name",
            pqxx::params(userId)
        );
        
        for (const auto& row : result) {
            FriendInfo friend_info;
            friend_info.userId = row["friend_id"].as<int>();
            friend_info.nickname = row["user_name"].as<std::string>();
            friend_info.userNameId = row["user_name_id"].as<std::string>();
            
            // Check for new messages from this friend
            friend_info.hasNewMessages = hasNewMessagesFrom(userId, friend_info.userId);
            
            friends.push_back(friend_info);
        }
        
        txn.commit();
    } catch (const std::exception& e) {
        debugLog("getFriendsList ERROR: " + std::string(e.what()));
    }
    
    return friends;
}

int findUserByNicknameAndId(const std::string& nickname, const std::string& userNameId) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    try {
        pqxx::connection conn(CONNECTION_STRING);
        pqxx::work txn(conn);
        
        auto result = txn.exec(
            "SELECT user_id FROM users WHERE user_name = $1 AND user_name_id = $2",
            pqxx::params(nickname, userNameId)
        );
        if (!result.empty()) {
            return result[0]["user_id"].as<int>();
        }
        
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "Error finding user: " << e.what() << std::endl;
    }
    
    return -1;
}