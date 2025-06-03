#include "AsyncQueryManager.h"
#include "../Config.h"
#include "../FriendList/friendlistback.h"
#include <pqxx/pqxx>

AsyncQueryManager::AsyncQueryManager(size_t numWorkers) : stop(false) {
    pool = std::make_unique<ConnectionPool>(DBConfig::getConnectionString(), 10);
    
    for (size_t i = 0; i < numWorkers; ++i) {
        workers.emplace_back(&AsyncQueryManager::worker, this);
    }
}

AsyncQueryManager::~AsyncQueryManager() {
    {
        std::unique_lock<std::mutex> lock(taskMutex);
        stop = true;
    }
    condition.notify_all();
    
    for (std::thread &worker : workers) {
        worker.join();
    }
}

std::future<void> AsyncQueryManager::markMessagesAsReadAsync(int userId, int friendId) {
    return enqueue([this, userId, friendId]() {
        auto conn = pool->getConnection();
        try {
            pqxx::work txn(*conn);
            txn.exec(
                "UPDATE user_chat SET read_status = TRUE "
                "WHERE sender_id = $1 AND receiver_id = $2 AND read_status = FALSE",
                pqxx::params(friendId, userId)
            );
            txn.commit();
        } catch (const std::exception& e) {
            // Log error
        }
        pool->returnConnection(std::move(conn));
    });
}

std::future<std::vector<FriendInfo>> AsyncQueryManager::getFriendsListAsync(int userId) {
    return enqueue([this, userId]() -> std::vector<FriendInfo> {
        auto conn = pool->getConnection();
        std::vector<FriendInfo> friends;
        
        try {
            pqxx::work txn(*conn);
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
                friend_info.hasNewMessages = false; // Will be updated separately
                friends.push_back(friend_info);
            }
            
            txn.commit();
        } catch (const std::exception& e) {
            // Log error
        }
        
        pool->returnConnection(std::move(conn));
        return friends;
    });
}

// ADD THIS MISSING FUNCTION:
std::future<bool> AsyncQueryManager::hasNewMessagesFromAsync(int userId, int friendId) {
    return enqueue([this, userId, friendId]() -> bool {
        auto conn = pool->getConnection();
        bool hasNew = false;
        
        try {
            pqxx::work txn(*conn);
            auto result = txn.exec(
                "SELECT COUNT(*) FROM user_chat "
                "WHERE sender_id = $1 AND receiver_id = $2 AND read_status = FALSE",
                pqxx::params(friendId, userId)
            );
            
            int count = result[0][0].as<int>();
            hasNew = count > 0;
            
            txn.commit();
        } catch (const std::exception& e) {
            // Log error
        }
        
        pool->returnConnection(std::move(conn));
        return hasNew;
    });
}

void AsyncQueryManager::worker() {
    for (;;) {
        std::function<void()> task;
        
        {
            std::unique_lock<std::mutex> lock(taskMutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            
            if (stop && tasks.empty()) {
                return;
            }
            
            task = std::move(tasks.front());
            tasks.pop();
        }
        
        task();
    }
}