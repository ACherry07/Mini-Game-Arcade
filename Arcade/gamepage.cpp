#include "gamepage.hpp"
#include "snake.hpp"
#include "sudoku.hpp"
#include "othello.hpp"
Gamepage::Gamepage(const std::string uname):username(uname){}

bool Gamepage::show(){
    std::cout << "Welcome " << username << std::endl;
    while(true){
        std::cout << "What would you like to do?" << std::endl;
        std::cout << "1. Snake" << std::endl;
        std::cout << "2. Sudoku Solver" << std::endl;
        std::cout << "3. Logout" << std::endl;
        int choice = 0;
        std::cin >> choice;
        if(choice == 1){
            runSnake();
        }
        else if(choice == 2){
            runSudoku();
        }
        else if(choice == 3){
            runOthello();
        }
        else{
            break;
        }
    }
    return true;
}