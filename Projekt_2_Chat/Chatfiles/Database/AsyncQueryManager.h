#ifndef ASYNC_QUERY_MANAGER_H
#define ASYNC_QUERY_MANAGER_H

#include <future>
#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include "ConnectionPool.h"

// Forward declaration for FriendInfo
struct FriendInfo;

class AsyncQueryManager {
private:
    std::unique_ptr<ConnectionPool> pool;
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex taskMutex;
    std::condition_variable condition;
    std::atomic<bool> stop;

public:
    AsyncQueryManager(size_t numWorkers = 4);
    ~AsyncQueryManager();
    
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type> {
        
        using return_type = typename std::result_of<F(Args...)>::type;
        
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        std::future<return_type> res = task->get_future();
        
        {
            std::unique_lock<std::mutex> lock(taskMutex);
            if (stop) {
                throw std::runtime_error("enqueue on stopped AsyncQueryManager");
            }
            
            tasks.emplace([task](){ (*task)(); });
        }
        condition.notify_one();
        
        return res;
    }
    
    // Specialized methods for common operations
    std::future<void> markMessagesAsReadAsync(int userId, int friendId);
    std::future<std::vector<FriendInfo>> getFriendsListAsync(int userId);
    std::future<bool> hasNewMessagesFromAsync(int userId, int friendId);
    
private:
    void worker();
};

#endif