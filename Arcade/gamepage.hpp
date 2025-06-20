#ifndef GAMEPAGE_HPP
#define GAMEPAGE_HPP

#include<iostream>
#include<mysql/mysql.h>
#include "mysqlconnector.hpp"

class Gamepage{
private:
    std::string username;
    MySQLConnector& db;
public:
    Gamepage(std::string uname, MySQLConnector& database);
    bool show();
};

#endif