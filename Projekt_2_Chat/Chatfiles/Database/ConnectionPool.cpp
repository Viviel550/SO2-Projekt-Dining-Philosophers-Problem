#include "ConnectionPool.h"
#include "../Config.h"

ConnectionPool::ConnectionPool(const std::string& connStr, size_t maxConn) 
    : connectionString(connStr), maxConnections(maxConn), currentConnections(0) {
    
    // Pre-create some connections
    for (size_t i = 0; i < std::min(maxConnections, size_t(3)); ++i) {
        pool.push(createConnection());
        currentConnections++;
    }
}

ConnectionPool::~ConnectionPool() {
    std::lock_guard<std::mutex> lock(poolMutex);
    while (!pool.empty()) {
        pool.pop();
    }
}

std::unique_ptr<pqxx::connection> ConnectionPool::getConnection() {
    std::unique_lock<std::mutex> lock(poolMutex);
    
    while (pool.empty() && currentConnections >= maxConnections) {
        condition.wait(lock);
    }
    
    if (!pool.empty()) {
        auto conn = std::move(pool.front());
        pool.pop();
        return conn;
    }
    
    if (currentConnections < maxConnections) {
        currentConnections++;
        return createConnection();
    }
    
    return nullptr;
}

void ConnectionPool::returnConnection(std::unique_ptr<pqxx::connection> conn) {
    if (conn && conn->is_open()) {
        std::lock_guard<std::mutex> lock(poolMutex);
        pool.push(std::move(conn));
        condition.notify_one();
    }
}

std::unique_ptr<pqxx::connection> ConnectionPool::createConnection() {
    return std::make_unique<pqxx::connection>(connectionString);
}