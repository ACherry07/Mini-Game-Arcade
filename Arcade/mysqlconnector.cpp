#include "mysqlconnector.hpp"
MySQLConnector::MySQLConnector(const char* server,
                               const char* user,
                               const char* password,
                               const char* database)
    : server(server), user(user), password(password), database(database), connection(nullptr) {}

MySQLConnector::~MySQLConnector() {
    if (connection) {
        mysql_close(connection);
    }
}

bool MySQLConnector::connect() {
    connection = mysql_init(nullptr);
    if (!mysql_real_connect(connection, server, user, password, database, 0, NULL, 0)) {
        std::cout << "Connection Error: " << mysql_error(connection) << std::endl;
        return false;
    }
    return true;
}

MYSQL_RES* MySQLConnector::executeQuery(const char* query) {
    if (mysql_query(connection, query)) {
        std::cout << "MySQL Query Error: " << mysql_error(connection) << std::endl;
        return nullptr;
    }
    return mysql_use_result(connection);
}

MYSQL* MySQLConnector::getConnection() {
    return connection;
}