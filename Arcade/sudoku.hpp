#include<bits/stdc++.h>
using namespace std;

class Sudoku{
    private:
        vector<vector<int>> grid;
        vector<vector<int>> grid_original;


    public:
        Sudoku(){
            grid.assign(9,vector<int>(9));
            grid_original.assign(9,vector<int>(9));
        }
        void takeGrid(){
            cout << "Enter the grid row by row." << endl;
            cout << "Leave a space between every entry and enter a zero to indicate an empty cell.\n";
            for(int i = 0; i < 9; i++){
                for(int j = 0; j < 9; j++){
                    int val;
                    cin >> val;
                    grid[i][j] = val;
                    grid_original[i][j] = val;
                }
            }
        }

        bool checkInRow(int n, int r){
            for(int i = 0; i < 9; i++){
                if(grid[r][i] == n){
                    return true;
                }
            }
            return false;
        }

        bool checkInColumn(int n, int c){
            for(int  i = 0; i < 9; i++){
                if(grid[i][c] == n){
                    return true;
                }
            }
            return false;
        }
        bool checkInBox(int n, int r, int c){
            int row = r/3 * 3;
            int col = c/3 * 3;
            cout << n << " " << row << " " << col << endl;
            for(int i = row; i < row + 3; i++){
                for(int j = col; j < col + 3; j++){
                    if(grid[i][j] == n){
                        return true;
                    }
                }
            }
            return false;
        }

        bool checkValid(int n, int r, int c){
            if(checkInRow(n,r)) return false;
            if(checkInColumn(n,c)) return false;
            if(checkInBox(n,r,c)) return false;
            return true;
        }

        bool solveSudoku(int r = 0, int c = 0) {
            if (r == 9) return true; // Finished all rows

            // Move to next row if at end of current row
            if (c == 9) return solveSudoku(r + 1, 0);

            // Skip cells that are pre-filled in the original grid
            if (grid_original[r][c] != 0)
                return solveSudoku(r, c + 1);

            // Try digits from 1 to 9
            for (int n = 1; n <= 9; ++n) {
                if (checkValid(n, r, c)) {
                    grid[r][c] = n;
                    displayGridLive(); // Live update
                    if (solveSudoku(r, c + 1))
                        return true;
                    grid[r][c] = 0; // Backtrack
                    displayGridLive(); // Show backtrack step
                }
            }

            return false; // Trigger backtracking
        }

        void displayGridLive(){
            //cout << "\033[2J\033[H";
            cout << "\033c\033[H"; // Clear screen and move cursor to top-left
            for(int i = 0; i < 9; i++){
                for(int j = 0; j < 9; j++){
                    cout << grid[i][j] << " ";
                }
                cout << endl;
            }
            usleep(100000); // Sleep for 30ms to visualize updates (needs <unistd.h>)
        }

        void displayGrid(){
            for(int i = 0; i < 9; i++){
                for(int j = 0; j < 9; j++){
                    cout << grid[i][j] << " ";
                }
                cout << endl;
            }
        }

};

void runSudoku(){
    Sudoku grid;
    grid.takeGrid();
    cout << "Grid inputted successfully!" << endl;
    if(grid.solveSudoku()) cout << "Grid solved!" << endl;
    else cout << "Unsolvable grid" << endl;
    grid.displayGrid();
    cout << "Thank you!" << endl;
}

#endif