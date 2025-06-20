#include<mysql/mysql.h>
#include "mysqlconnector.hpp" 
#include "homepage.hpp"
#include "gamepage.hpp"

using namespace std;
int main() {
    MySQLConnector db("localhost", "root", "l9hfG&7a", "mydatabase");
    db.connect();
    Homepage homepage(db);
    std::string currentuser = homepage.show();
    Gamepage gamepage(currentuser, db);
    gamepage.show();
    cout << "Thank you!" << endl;
    return 0;
}
