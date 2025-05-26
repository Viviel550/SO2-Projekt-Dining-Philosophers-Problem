#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include <pqxx/pqxx>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

class ConnectionPool {
private:
    std::queue<std::unique_ptr<pqxx::connection>> pool;
    std::mutex poolMutex;
    std::condition_variable condition;
    std::string connectionString;
    size_t maxConnections;
    size_t currentConnections;

public:
    ConnectionPool(const std::string& connStr, size_t maxConn = 10);
    ~ConnectionPool();
    
    std::unique_ptr<pqxx::connection> getConnection();
    void returnConnection(std::unique_ptr<pqxx::connection> conn);
    
private:
    std::unique_ptr<pqxx::connection> createConnection();
};

#endif