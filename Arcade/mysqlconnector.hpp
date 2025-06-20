#ifndef MYSQLCONNECTOR_HPP
#define MYSQLCONNECTOR_HPP

#include <mysql/mysql.h>
#include <iostream>
#include <string>

class MySQLConnector {
private:
    const char* server;
    const char* user;
    const char* password;
    const char* database;
    MYSQL* connection;

public:
    MySQLConnector(const char* server,
                   const char* user,
                   const char* password,
                   const char* database);

    ~MySQLConnector();

    bool connect();
    MYSQL_RES* executeQuery(const char* query);
    MYSQL* getConnection();  // Optional, in case raw access is needed
};

#endif
