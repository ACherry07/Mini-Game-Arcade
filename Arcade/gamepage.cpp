#include "gamepage.hpp"
#include "snake.hpp"
#include "sudoku.hpp"
#include "othello1.hpp"
#include "tictactoe.hpp"
#include "stats.hpp"
Gamepage::Gamepage(const std::string uname, MySQLConnector& database):username(uname), db(database){}

bool Gamepage::show(){
    std::cout << "Welcome " << username << std::endl;
    while(true){
        std::cout << "What would you like to do?" << std::endl;
        std::cout << "1. Snake" << std::endl;
        std::cout << "2. Sudoku Solver" << std::endl;
        std::cout << "3. Othello" << std::endl;
        std::cout << "4. Tic Tac Toe" << std::endl;
        std::cout << "5. Get Stats" << std::endl;
        std::cout << "6. Logout" << std::endl;
        int choice = 0;
        std::cin >> choice;
        if(choice == 1){
            Snake::runSnake(username, db);
        }
        else if(choice == 2){
            Sudoku::runSudoku();
        }
        else if(choice == 3){
            Othello::runOthello(username,db);
        }
        else if(choice == 4){
            TicTacToe::runTicTacToe(username, db);
        }
        else if(choice == 5){
            Stats::runStats(username,db);
        }
        else{
            break;
        }
    }
    return true;
}