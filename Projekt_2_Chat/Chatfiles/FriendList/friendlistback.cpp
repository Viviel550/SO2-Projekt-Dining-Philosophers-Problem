#include "friendlistback.h"
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "../Config.h"

bool sendFriendRequest(int senderId, int receiverId) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    try {
        pqxx::connection conn(CONNECTION_STRING);
        pqxx::work txn(conn);
        
        // Ensure senderId != receiverId
        if (senderId == receiverId) {
            return false;
        }
        
        // Check if friendship already exists
        auto checkResult = txn.exec(
            "SELECT COUNT(*) FROM friends WHERE user_1 = LEAST($1, $2) AND user_2 = GREATEST($1, $2)",
            pqxx::params(senderId, receiverId)
        );
        
        if (checkResult[0][0].as<int>() > 0) {
            return false; // Friendship already exists
        }
        
        txn.exec(
            "INSERT INTO friends (user_1, user_2, accepted) VALUES (LEAST($1, $2), GREATEST($1, $2), TRUE)",
            pqxx::params(senderId, receiverId)
        );
        
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error sending friend request: " << e.what() << std::endl;
        return false;
    }
}

bool acceptFriendRequest(int userId1, int userId2) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    try {
        pqxx::connection conn(CONNECTION_STRING);
        pqxx::work txn(conn);
        
        auto result = txn.exec(
            "UPDATE friends "
            "SET accepted = TRUE, accepted_at = CURRENT_TIMESTAMP "
            "WHERE user_1 = LEAST($1, $2) AND user_2 = GREATEST($1, $2) AND accepted = FALSE",
            pqxx::params(userId1, userId2)
        );
        
        txn.commit();
        return result.affected_rows() > 0;
    } catch (const std::exception& e) {
        std::cerr << "Error accepting friend request: " << e.what() << std::endl;
        return false;
    }
}

bool removeFriend(int userId1, int userId2) {
    std::string CONNECTION_STRING = DBConfig::getConnectionString();
    try {
        pqxx::connection conn(CONNECTION_STRING);
        pqxx::work txn(conn);
        
        auto result = txn.exec(
            "DELETE FROM friends "
            "WHERE user_1 = LEAST($1, $2) AND user_2 = GREATEST($1, $2)",
            pqxx::params(userId1, userId2)
        );
        
        txn.commit();
        return result.affected_rows() > 0;
    } catch (const std::exception& e) {
        std::cerr << "Error removing friend: " << e.what() << std::endl;
        return false;
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
            "  CASE WHEN f.user_1 = $1 THEN f.user_2 = u.id ELSE f.user_1 = u.id END"
            ") "
            "WHERE (f.user_1 = $1 OR f.user_2 = $1) AND f.accepted = TRUE "
            "ORDER BY u.nickname",
            pqxx::params(userId)
        );
        
        for (const auto& row : result) {
            FriendInfo friend_info;
            friend_info.userId = row["friend_id"].as<int>();
            friend_info.nickname = row["nickname"].as<std::string>();
            friend_info.userNameId = row["user_name_id"].as<std::string>();
            friends.push_back(friend_info);
        }
        
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "Error getting friends list: " << e.what() << std::endl;
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
        if (result.size() > 0) {
            return result[0]["user_id"].as<int>();
        }
        
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "Error finding user: " << e.what() << std::endl;
    }
    
    return -1;
}