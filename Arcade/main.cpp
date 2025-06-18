#include "mysqlconnector.hpp" 
#include "homepage.hpp"
#include "gamepage.hpp"
using namespace std;
int main() {
    Homepage homepage("localhost", "root", "l9hfG&7a", "mydatabase");
    std::string currentuser = homepage.show();
    Gamepage gamepage(currentuser);
    gamepage.show();
    cout << "Thank you!" << endl;
    return 0;
}
