#ifndef SNAKE_HPP
#define SNAKE_HPP

// #include <iostream>
// #include <unistd.h>
// #include <fcntl.h>
// #include <sys/select.h>
// #include <cstdlib>
// #include <ctime>
// #include <deque>
// #include <utility>
#include <termios.h>
#include<bits/stdc++.h>
#include<mysql/mysql.h>
#include "mysqlconnector.hpp"

using namespace std;

namespace Snake {

const int width = 20;
const int height = 20;

//Used online help for this terminal control setup
struct termios orig_termios;

void reset_terminal_mode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode() {
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    new_termios = orig_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    new_termios.c_cc[VMIN] = 0;
    new_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    atexit(reset_terminal_mode);
}

int kbhit() {
    struct timeval tv = {0, 0};
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0;
}

int getch() {
    unsigned char ch;
    if (read(STDIN_FILENO, &ch, 1) < 0)
        return 0;
    return ch;
}

void clearScreen() {
    cout << "\033c\033[H";
}

class Snake {
private:
    deque<pair<int, int>> body;
    int length;

public:
    Snake(int startX, int startY) {
        body.push_front({startX, startY});
        length = 1;
    }

    void move(int x, int y, bool grow) {
        body.push_front({x, y});
        if (!grow) {
            body.pop_back();
        } else {
            length++;
        }
    }

    bool checkCollision(int x, int y) const {
        for (size_t i = 1; i < body.size(); ++i) {
            if (body[i].first == x && body[i].second == y)
                return true;
        }
        return false;
    }

    pair<int, int> getHead() const {
        return body.front();
    }

    const deque<pair<int, int>>& getBody() const {
        return body;
    }

    int getLength() const {
        return length;
    }
};

class SnakeGame {
private:
    string username;
    MySQLConnector db;
    Snake snake;
    int foodX, foodY;
    enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
    Direction dir;
    bool gameOver;

public:
    SnakeGame(string uname, MySQLConnector& database) 
    : db(database), username(uname),snake(width / 2, height / 2), dir(RIGHT), gameOver(false) {
        db.connect();
        generateFood();
        set_conio_terminal_mode();
    }

    void generateFood() {
        do {
            foodX = rand() % width;
            foodY = rand() % height;
        } while (snake.checkCollision(foodX, foodY) || (foodX == snake.getHead().first && foodY == snake.getHead().second));
    }

    void drawBoard() const {
        clearScreen();
        for (int i = 0; i < width + 2; i++) cout << "#";
        cout << endl;

        for (int y = 0; y < height; y++) {
            cout << "#";
            for (int x = 0; x < width; x++) {
                auto [hx, hy] = snake.getHead();
                if (x == foodX && y == foodY)
                    cout << "F";
                else if (x == hx && y == hy)
                    cout << "O";
                else {
                    bool printed = false;
                    for (const auto& segment : snake.getBody()) {
                        if (segment.first == x && segment.second == y) {
                            cout << "o";
                            printed = true;
                            break;
                        }
                    }
                    if (!printed)
                        cout << " ";
                }
            }
            cout << "#" << endl;
        }

        for (int i = 0; i < width + 2; i++) cout << "#";
        cout << endl;
        cout << "\n\nScore: " << snake.getLength() << endl;
    }

    void handleInput() {
        if (kbhit()) {
            char ch = getch();
            switch (ch) {
                case 'a': case 'A':
                    if (dir != RIGHT) dir = LEFT;
                    break;
                case 'd': case 'D':
                    if (dir != LEFT) dir = RIGHT;
                    break;
                case 'w': case 'W':
                    if (dir != DOWN) dir = UP;
                    break;
                case 's': case 'S':
                    if (dir != UP) dir = DOWN;
                    break;
                case 'x': case 'X':
                    gameOver = true;
                    break;
            }
        }
    }

    void update() {
        auto [x, y] = snake.getHead();
        switch (dir) {
            case LEFT: x--; break;
            case RIGHT: x++; break;
            case UP: y--; break;
            case DOWN: y++; break;
            default: break;
        }

        if (x < 0 || x >= width || y < 0 || y >= height || snake.checkCollision(x, y)) {
            gameOver = true;
            return;
        }

        bool grow = (x == foodX && y == foodY);
        if (grow) generateFood();
        snake.move(x, y, grow);
    }

    void play() {
        while (!gameOver) {
            drawBoard();
            handleInput();
            update();
            usleep(150000);
        }

        reset_terminal_mode();
        cout << "Game Over! Final Score: " << snake.getLength() << endl;
        char query[256];
        sprintf(query, "SELECT highscore FROM snake WHERE user = '%s';",username.c_str());
        int score = snake.getLength();
        MYSQL_RES* res = db.executeQuery(query);
        MYSQL_ROW row = mysql_fetch_row(res);
        if(score > stoi(row[0])){
            sprintf(query,"UPDATE snake SET matches = matches + 1, highscore = '%s' WHERE user = '%s';",to_string(score).c_str(),username.c_str());
        }
        else{
            sprintf(query,"UPDATE snake SET matches = matches + 1 WHERE user = '%s';", username.c_str());
        }
        mysql_free_result(res);
        db.executeQuery(query);
        
    }
};


int runSnake(string username, MySQLConnector& db){
    clearScreen();
    cout << "Welcome to Snake! Use W A S D to move. Press X to quit.\nPress any key to begin...\n";

    set_conio_terminal_mode();

    while (!kbhit()) {
        usleep(10000);
    }
    getch();

    reset_terminal_mode();

    srand(time(0));

    SnakeGame game(username, db);
    game.play();

    return 0;
}


} 

#endif
