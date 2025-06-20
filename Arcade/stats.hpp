#ifndef STATS_HPP
#define STATS_HPP

#include<iostream>
#include<mysql/mysql.h>
#include "mysqlconnector.hpp"
using namespace std;
namespace Stats{
void runStats(string username, MySQLConnector& db){
    db.connect();
    int choice;
    while(true){
        cout << "What stats do you wish to see?" << endl;
        cout << "1. Snake" << endl;
        cout << "2. Othello" << endl;
        cout << "3. Tic Tac Toe" << endl;
        cout << "4. Go Back" << endl;
        cin >> choice;
        if(choice == 1){
            char query[256];
            sprintf(query, "SELECT * from snake where user = '%s';", username.c_str());
            MYSQL_RES* res = db.executeQuery(query);
            MYSQL_ROW row;
            cout << "User: " << username << endl;
            while((row=mysql_fetch_row(res))!=nullptr){
                cout << endl;
                cout << "Matches: " << row[1] << endl;
                cout << "High Score: " << row[2] << endl;
                cout << endl;
            }
        }
        else if (choice == 2){
            char query[256];
            sprintf(query, "SELECT * from othello where user = '%s';", username.c_str());
            MYSQL_RES* res = db.executeQuery(query);
            MYSQL_ROW row;
            cout << "User: " << username << endl;
            while((row=mysql_fetch_row(res))!=nullptr){
                cout << endl;
                cout << "Mode: " << row[1] << endl;
                cout << "Matches: " << row[2] << endl;
                cout << "Wins: " << row[3] << endl;
                cout << "Draws: " << row[4] << endl;
                cout << "Losses: " << row[5] << endl;
                cout << endl;
            }
        }
        else if(choice == 3){
            char query[256];
            sprintf(query, "SELECT * from tictactoe where user = '%s';", username.c_str());
            MYSQL_RES* res = db.executeQuery(query);
            MYSQL_ROW row;
            cout << "User: " << username << endl;
            while((row=mysql_fetch_row(res))!=nullptr){
                cout << endl;
                cout << "Mode: " << row[1] << endl;
                cout << "Matches: " << row[2] << endl;
                cout << "Wins: " << row[3] << endl;
                cout << "Draws: " << row[4] << endl;
                cout << "Losses: " << row[5] << endl;
                cout << endl;
            }
        }
        else{
            break;
        }

    }    
}
}
#endif