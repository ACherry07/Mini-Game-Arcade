// #ifndef HOMEPAGE_HPP
// #define HOMEPAGE_HPP

// #include<iostream>
// #include<mysql/mysql.h>
// #include "mysqlconnector.hpp"

// std::string getText(std::string s);
// bool homepage();
// bool login(MySQLConnector& db);
// bool signup(MySQLConnector& db);

// #endif
#ifndef HOMEPAGE_HPP
#define HOMEPAGE_HPP

#include<iostream>
#include<mysql/mysql.h>
#include "mysqlconnector.hpp"

class Homepage {
private:
    MySQLConnector db;
    std::string currentuser;
    std::string getText(std::string s);
    bool login();
    bool signup();
public:
    Homepage(const char* server, const char* user, const char* password, const char* database);
    std::string show();
};

#endif
