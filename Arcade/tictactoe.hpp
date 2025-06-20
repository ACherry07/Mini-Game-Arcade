#ifndef TICTACTOE_HPP
#define TICTACTOE_HPP

#include<bits/stdc++.h>
#include<mysql/mysql.h>
#include "mysqlconnector.hpp"
using namespace std;

namespace TicTacToe{
enum class PlayerType{
    Human,
    Computer
};

class Player{
    private:
        char self;
        char opponent;
        PlayerType type;
    public:
        Player(){

        }
        void setValues(PlayerType t, char c){
            type = t;
            self = c;
            opponent = 'X'+'O'-self;
        }
        pair<int,int> getMove(vector<vector<char>> &board, int level = 0){
            if(type == PlayerType::Human){
                while(true){
                    cout << "Enter your move as 'row column' (eg - 1 3):" << endl;
                    int row, col;
                    cin >> row >> col;
                    if(board[row-1][col-1] == ' '){
                        return {row-1,col-1};
                    }
                    else{
                        cout << "Invalid Move, please try again.";
                    }
                }
            }
            else{
                return getBestMove(board,level);
            }
        }



        pair<int, int> getBestMove(const vector<vector<char>>& board, const int level) {
            vector<char> row1 = board[0];
            vector<char> row2 = board[1];
            vector<char> row3 = board[2];

            vector<char> col1 = {row1[0], row2[0], row3[0]};
            vector<char> col2 = {row1[1], row2[1], row3[1]};
            vector<char> col3 = {row1[2], row2[2], row3[2]};

            vector<char> diag1 = {row1[0], row2[1], row3[2]};
            vector<char> diag2 = {row1[2], row2[1], row3[0]};

            vector<vector<char>> lists = {row1, row2, row3, col1, col2, col3, diag1, diag2};

            pair<int, int> best = {1, 1}; //centre is the best by default

            vector<pair<int, int>> order = {
                {1, 1}, {0, 0}, {0, 2}, {2, 2}, {2, 0},
                {0, 1}, {1, 2}, {2, 1}, {1, 0}
            };

            for (const auto& pos : order) {
                int p1 = pos.first;
                int p2 = pos.second;
                if (board[p1][p2] == ' ') {
                    best = pos;
                    break;
                }
            }
            cout << "initial best" << best.first << " " << best.second << endl;
            for (int i = 0; i < lists.size(); ++i) {
                const auto& line = lists[i];
                if (count(line.begin(), line.end(), self) == 2 &&
                    count(line.begin(), line.end(), opponent) == 0) {
                    int x = find(line.begin(), line.end(), ' ') - line.begin();
                    cout << "case 1:" << x << endl;
                    if (i < 3)
                        best = {i, x}; // row
                    else if (i < 6)
                        best = {x, i - 3}; // column
                    else if (i == 6)
                        best = {x, x}; // diag1
                    else
                        best = {x, 2 - x}; // diag2
                    break;
                } else if (count(line.begin(), line.end(), opponent) == 2 &&
                        count(line.begin(), line.end(), self) == 0) {
                    int x = find(line.begin(), line.end(), ' ') - line.begin();
                    cout << "case 2:" << x << endl;
                    if (i < 3)
                        best = {i, x};
                    else if (i < 6)
                        best = {x, i - 3};
                    else if (i == 6)
                        best = {x, x};
                    else
                        best = {x, 2 - x};
                }
            }
            cout << best.first << " " << best.second << " before random choice" << endl;

            auto random_choice = [&](vector<pair<int, int>>& vec) {
                cout << "Random" << endl;
                while (!vec.empty()) {
                    long int t = static_cast<long int>(time(NULL));
                    long int idx = t%vec.size();
                    auto pos = vec[idx];
                    int p1 = pos.first;
                    int p2 = pos.second;
                    if (board[p1][p2] == ' ') {
                        best = pos;
                        break;
                    } else {
                        vec.erase(vec.begin() + idx);
                    }
                }
            };
            long int t = static_cast<long int> (time(NULL));
            cout << t << endl;
            if (level == 0 && 1+t%10 > 1) random_choice(order);
            if (level == 1 && 1+t%10 > 3) random_choice(order);
            if (level == 2 && 1+t%20 > 10) random_choice(order);
            if (level == 3 && 1+t%40 > 30) random_choice(order);
            if (level == 4 && 1+t%100 > 99) random_choice(order);
            cout << best.first << " " << best.second << endl;
            return best;
        }


};


class TicTacToe{
    private:
        MySQLConnector& db;
        string username;
        char user;
        vector<vector<char>> board;
        int mode;
        int level;
        int state;
        unordered_map<char,Player> players;        
    public:
        TicTacToe(string uname, MySQLConnector& database)
        :username(uname), db(database), level(0){
            db.connect();
            board.resize(3,vector<char>(3,' '));
            setMode();
            if(mode == 1){
                setLevel();
            }
            setPlayers();
            play();
        }
        void setMode(){
            cout << "Do you wish to play\n1. vs Computer\n2. vs Player?\n";
            cin >> mode; 
        }
        void setLevel(){
            cout << "Choose your difficulty level:\n1. Useless\n2. Easy\n3. Medium\n4. Hard\n5. Impossible to win!" << endl;
            cin >> level;
        }
        void setPlayers(){
            if(mode == 1){
                cout << username << " are you X or O?(X starts)" << endl;
                char playertype;
                cin >> playertype;
                playertype = toupper(playertype);
                user = playertype;
                players[playertype].setValues(PlayerType::Human,playertype);
                players['X' + 'O' - playertype].setValues(PlayerType::Computer,'X'+'O'-playertype);
            }
            else{
                cout << username << "are you X or O?(X starts)" << endl;
                cin >> user;
                toupper(user);
                players['X'].setValues(PlayerType::Human,'X');
                players['O'].setValues(PlayerType::Human,'O');
            }
        }
        void play(){
            char currentplayer = 'X';
            while(true){
                //cout << "\033c\033[H";
                displayBoard();
                cout << "Player " << currentplayer << " ";
                auto[row,col] = players[currentplayer].getMove(board, level);
                board[row][col] = currentplayer;
                if(checkGameOver()){
                    cout << currentplayer << " wins!!!" << endl;
                    char query[256];
                    if(user == currentplayer){
                        sprintf(query, "UPDATE tictactoe SET matches = matches + 1, wins = wins + 1 WHERE user = '%s' AND mode = '%s';", username.c_str(), (mode==1?"computer":"player"));
                    }
                    else{
                        sprintf(query, "UPDATE tictactoe SET matches = matches + 1, losses = losses + 1 WHERE user = '%s' AND mode = '%s';", username.c_str(), (mode==1?"computer":"player"));
                    }
                    db.executeQuery(query);
                    break;
                }
                if(checkDraw()){
                    cout << "Draw Game" << endl;
                    char query[256];
                    sprintf(query, "UPDATE tictactoe SET matches = matches + 1, draws = draws + 1 WHERE user = '%s' AND mode = '%s';", username.c_str(), (mode==1?"computer":"player"));
                    db.executeQuery(query);
                    break;
                }
                currentplayer = 'X' + 'O' - currentplayer;
            }

        }
        bool checkGameOver(){
            return checkRow() || checkCol() || checkDiag();
        }
        bool checkDraw(){
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++){
                    if(board[i][j] == ' '){
                        return false;
                    }
                }
            }
            return true;
        }
        bool checkRow(){
            for(int i = 0; i <3; i++){
                if(board[i][0] == board[i][1] && board[i][0] == board[i][2] && board[i][0] != ' '){
                    return true;
                }
            }
            return false;
        }
        bool checkCol(){
            for(int j = 0; j<3; j++){
                if (board[0][j] == board[1][j] && board[0][j] == board[2][j] && board[0][j] != ' '){
                    return true;
                }
            }
            return false;
        }
        bool checkDiag(){
            if(board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ' ||
            board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' '){
                return true;
            }
            return false;
        }
        void displayBoard(){
            cout << "       1   2   3   " << endl;
            cout << "     --------------" << endl;
            for(int i = 0; i < 3; i++){
                cout << i+1;
                cout << "    | ";
                for(int j = 0; j < 3; j++){
                    cout << board[i][j] << " | ";
                }
                cout << endl;
                cout << "     --------------" << endl;
            }
        }

};

void runTicTacToe(string username, MySQLConnector& database){
    TicTacToe Game(username, database);
    Game.displayBoard();
    return;
}
}
#endif