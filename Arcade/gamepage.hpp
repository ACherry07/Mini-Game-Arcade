#ifndef GAMEPAGE_HPP
#define GAMEPAGE_HPP

#include<iostream>

class Gamepage{
private:
    std::string username;
public:
    Gamepage(std::string uname);
    bool show();
};

#endif