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
    MySQLConnector& db;
    std::string currentuser = "";
    std::string getText(std::string s);
    bool login();
    bool signup();
    void createtables(std:: string username);
public:
    Homepage(MySQLConnector& database);
    std::string show();
};

#endif
