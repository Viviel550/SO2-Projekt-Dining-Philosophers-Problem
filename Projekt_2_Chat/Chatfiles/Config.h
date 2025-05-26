#ifndef CONFIG_H
#define CONFIG_H
#include <string>

namespace DBConfig {
    // Database connection parameters
    const std::string DB_NAME = "Chat";
    const std::string DB_USER = "postgres";
    const std::string DB_PASSWORD = "123";
    const std::string DB_HOST = "127.0.0.1";
    const std::string DB_PORT = "5432";

    // Function to get the connection string
    inline std::string getConnectionString() {
        return "dbname=" + DB_NAME + 
               " user=" + DB_USER + 
               " password=" + DB_PASSWORD + 
               " hostaddr=" + DB_HOST + 
               " port=" + DB_PORT;
    }
}

#endif 