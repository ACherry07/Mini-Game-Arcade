#ifndef SUDOKU_HPP
#define SUDOKU_HPP
#include<bits/stdc++.h>
#include<unistd.h>
using namespace std;

namespace Sudoku{
class Sudoku {
    private:
        vector<vector<int>> grid;
        vector<vector<int>> grid_original;

        bool checkInRow(int n, int r) {
            for(int i = 0; i < 9; i++) {
                if(grid[r][i] == n) {
                    return true;
                }
            }
            return false;
        }

        bool checkInColumn(int n, int c) {
            for(int i = 0; i < 9; i++) {
                if(grid[i][c] == n) {
                    return true;
                }
            }
            return false;
        }

        bool checkInBox(int n, int r, int c) {
            int row = r/3 * 3;
            int col = c/3 * 3;
            cout << n << " " << row << " " << col << endl;
            for(int i = row; i < row + 3; i++) {
                for(int j = col; j < col + 3; j++) {
                    if(grid[i][j] == n) {
                        return true;
                    }
                }
            }
            return false;
        }

        bool checkValid(int n, int r, int c) {
            if(checkInRow(n, r)) return false;
            if(checkInColumn(n, c)) return false;
            if(checkInBox(n, r, c)) return false;
            return true;
        }

        void displayGridLive() {
            cout << "\033c\033[H";
            for(int i = 0; i < 9; i++) {
                for(int j = 0; j < 9; j++) {
                    cout << grid[i][j] << " ";
                }
                cout << endl;
            }
            usleep(100000);
        }

    public:
        Sudoku() {
            grid.assign(9, vector<int>(9));
            grid_original.assign(9, vector<int>(9));
        }

        void takeGrid() {
            cout << "Enter the grid row by row." << endl;
            cout << "Leave a space between every entry and enter a zero to indicate an empty cell.\n";
            for(int i = 0; i < 9; i++) {
                for(int j = 0; j < 9; j++) {
                    int val;
                    cin >> val;
                    grid[i][j] = val;
                    grid_original[i][j] = val;
                }
            }
        }

        bool solveSudoku(int r = 0, int c = 0) {
            if (r == 9) return true;
            if (c == 9) return solveSudoku(r + 1, 0);
            if (grid_original[r][c] != 0)
                return solveSudoku(r, c + 1);
            for (int n = 1; n <= 9; ++n) {
                if (checkValid(n, r, c)) {
                    grid[r][c] = n;
                    displayGridLive();
                    if (solveSudoku(r, c + 1))
                        return true;
                    grid[r][c] = 0;
                    displayGridLive();
                }
            }
            return false;
        }

        void displayGrid() {
            for(int i = 0; i < 9; i++) {
                for(int j = 0; j < 9; j++) {
                    cout << grid[i][j] << " ";
                }
                cout << endl;
            }
        }
};

void runSudoku() {
    Sudoku grid;
    grid.takeGrid();
    cout << "Grid inputted successfully!" << endl;
    if(grid.solveSudoku()) cout << "Grid solved!" << endl;
    else cout << "Unsolvable grid" << endl;
    grid.displayGrid();
    cout << "Thank you!" << endl;
}
}

#endif