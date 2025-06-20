#ifndef OTHELLO_HPP
#define OTHELLO_HPP

#include<bits/stdc++.h>
#include<mysql/mysql.h>
#include "mysqlconnector.hpp"

using namespace std;
namespace Othello{
    enum class GameState{
        start,
        current,
        end
    };


    class Player{
        private:
            int player_number;
            char color;
            string user;

        public:
            Player(){}

            void setValues(char c, string name){
                color = c;
                user = name;
                player_number = (color == 'B')?1:2;
            }

            pair<int,int> getMove(vector<vector<char>>& board){
                while(true){
                    cout << "Player " << player_number << " please enter your move (eg - A1):" << endl;
                    string move;
                    cin >> move;
                    int row = move[1]-'1';
                    int col = toupper(move[0]) - 'A';
                    if(row < 0 || row > 7 || col < 0 || col > 7){
                        cout << "Entry out of bounds. Please try again!" << endl;
                        continue;
                    }
                    if(board[row][col] != ' '){
                        cout << "That spot is already taken. Please try again!" << endl;
                        continue;
                    }
                    if(!isValid(board,row,col)){
                        cout << "Invalid move. Does not lead to turnovers. Please try again!" << endl;
                        continue;
                    }
                    return {row,col};
                }
            }
            bool isValid(vector<vector<char>>& board, int row, int col){
                vector<pair<int, int>> directions = {
                {-1, -1}, {-1, 0}, {-1, 1},
                { 0, -1},          { 0, 1},
                { 1, -1}, { 1, 0}, { 1, 1}
                };
                char opponent_color = 'B' + 'W' - color;
                bool valid = false;
                for(auto [dx,dy]: directions){
                    //cout << dx << " " << dy << endl;
                    int x = row + dx;
                    int y = col + dy;
                    bool hasOpponentBetween = false;

                    // Step over one or more opponent pieces
                    while (x >= 0 && x < 8 && y >= 0 && y < 8 && board[x][y] == opponent_color) {
                        x += dx;
                        y += dy;
                        hasOpponentBetween = true;
                    }
                    //cout << hasOpponentBetween << endl;
                    // If at least one opponent piece is flanked and we end on a currentPlayer piece
                    if (hasOpponentBetween &&
                        x >= 0 && x < 8 && y >= 0 && y < 8 &&
                        board[x][y] == color) {
                        // Step over one or more opponent pieces
                        x = row + dx;
                        y = col + dy;
                        //cout << x << " " << y;
                        while (x >= 0 && x < 8 && y >= 0 && y < 8 && board[x][y] == opponent_color) {
                            board[x][y] = color;
                            x += dx;
                            y += dy;
                            valid = true;
                        }
                    }

                }

                return valid;
            }
            char getColor(){
                return color;
            }

            string getUser(){
                return user;
            }
    };


    class Othello{
        private:
        MySQLConnector& db;
        string username;
        vector<vector<char>> board;
        unordered_map<int,Player> players;
        GameState game_state;
        int pieces_on_board;
        int mode = 2;
        public:
        Othello(string uname, MySQLConnector& database):username(uname),db(database){
            setupBoard();
            setupPlayers();
            play();
        }

        void setupBoard(){
            board.resize(8,vector<char>(8,' '));
            board[3][3] = 'W';
            board[4][4] = 'W';
            board[3][4] = 'B';
            board[4][3] = 'B';
            pieces_on_board = 4; 
        }

        void setupPlayers(){
            cout << username << " are you Player 1(Black) or Player 2(White)? (Enter 1 or 2)" << endl;
            int choice;
            cin >> choice;
            int player_color = (choice==1?'B':'W');
            int opponent = 3 - choice;
            int opponent_color = (opponent == 1?'B':'W');
            players[choice].setValues(player_color, username);
            players[opponent].setValues(opponent_color, "opponent");
        }

        void play(){
            displayBoard();
            int currentplayer = 1;
            int loop = 0;
            game_state = GameState::start;
            while(game_state!=GameState::end && loop < 2){
                displayBoard();
                if(!hasValidMove(currentplayer)){
                    switchPlayer(currentplayer);
                    loop++;
                    if(loop == 2) game_state = GameState::end;
                    continue;
                }
                loop = 0;
                auto [row,col] = players[currentplayer].getMove(board);
                board[row][col] = players[currentplayer].getColor();
                pieces_on_board++;
                updateGameState();
                if(game_state == GameState::end){break;}
                switchPlayer(currentplayer);
            }
            cout << "Game Over" << endl;
            findWinner();
        }

        bool hasValidMove(const int& current_player){          
            char opponent = 3-current_player;
            char current_color = players[current_player].getColor();
            char opponent_color = players[opponent].getColor();
            const int N = 8;

            vector<pair<int, int>> directions = {
                {-1, -1}, {-1, 0}, {-1, 1},
                { 0, -1},          { 0, 1},
                { 1, -1}, { 1, 0}, { 1, 1}
            };

            for (int row = 0; row < N; ++row) {
                for (int col = 0; col < N; ++col) {
                    if (board[row][col] != ' ') continue;

                    for (auto [dx, dy] : directions) {
                        int x = row + dx;
                        int y = col + dy;
                        bool hasOpponentBetween = false;

                        // Step over one or more opponent pieces
                        while (x >= 0 && x < N && y >= 0 && y < N && board[x][y] == opponent_color) {
                            x += dx;
                            y += dy;
                            hasOpponentBetween = true;
                        }

                        // If at least one opponent piece is flanked and we end on a currentPlayer piece
                        if (hasOpponentBetween &&
                            x >= 0 && x < N && y >= 0 && y < N &&
                            board[x][y] == current_color) {
                            return true;
                        }
                    }
                }
            }
            cout << "Player " << current_player << " has no moves right now. Switching to Player " << opponent << endl;
            usleep(100000);
            return false;
        }
        void updateGameState(){
            if (pieces_on_board > 4){
                game_state = GameState::current;
            }
            if(pieces_on_board == 64){
                game_state = GameState::end;
            }
        }
        void switchPlayer(int& a){
            a = 3-a;
        }

        void findWinner(){
            int count_black = 0, count_white = 0;
            for(int i = 0; i < 8; i++){
                for(int j = 0; j < 8; j++){
                    if(board[i][j] == 'B') count_black++;
                    else if(board[i][j] == 'W')count_white++;
                }
            }
            cout << "Player 1:" << count_black << endl;
            cout << "Player 2:" << count_white << endl; 
            char query[256];
            if(count_black > count_white){
                cout << "Player 1 wins!!!" << endl;
                if(players[1].getUser() == username){
                    sprintf(query, "UPDATE othello SET matches = matches + 1, wins = wins + 1 WHERE user = '%s' AND mode = '%s';", username.c_str(), (mode==1?"computer":"player"));
                }
                else{
                    sprintf(query, "UPDATE othello SET matches = matches + 1, losses = losses + 1 WHERE user = '%s' AND mode = '%s';", username.c_str(), (mode==1?"computer":"player"));
                }
            }
            else if(count_black == count_white){
                cout << "Game Drawn!" << endl;
                sprintf(query, "UPDATE othello SET matches = matches + 1, draws = draw + 1 WHERE user = '%s' AND mode = '%s';", username.c_str(), (mode==1?"computer":"player"));
            }
            else{
                cout << "Player 2 wins" << endl;
                if(players[1].getUser() == username){
                    cout << players[1].getUser() << " " << username << endl;
                    sprintf(query, "UPDATE othello SET matches = matches + 1, losses = losses + 1 WHERE user = '%s' AND mode = '%s';", username.c_str(), (mode==1?"computer":"player"));
                }
                else{
                    cout << players[1].getUser() << " " << username << endl;
                    sprintf(query, "UPDATE othello SET matches = matches + 1, wins = wins + 1 WHERE user = '%s' AND mode = '%s';", username.c_str(), (mode==1?"computer":"player"));   
                }
            }
            cout << query << endl;
            db.executeQuery(query);
        }
        
        void clearScreen() {
            //system("clear");
            cout << "\033c\033[H";
        }
        
        void displayBoard(){
            clearScreen();
            cout << "       A   B   C   D   E   F   G   H   " << endl;
            cout << "     ----------------------------------" << endl;
            for(int i = 0; i < 8; i++){
                cout << i+1;
                cout << "    | ";
                for(int j = 0; j < 8; j++){
                    cout << board[i][j] << " | ";
                }
                cout << endl;
                cout << "     ----------------------------------" << endl;
            }
        }
    };

    void runOthello(string username, MySQLConnector& db){
        Othello Game(username, db);
    }
}

#endif