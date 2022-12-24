#include <ncurses.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <thread>

void makeGrid(void);
void displayGrid(void);

using namespace std;

enum size {height = 22, width = 22};
vector<vector<char>> grid;

class tank{
    private:
        vector<int> position;
        
        vector<int> RIGHT ={0,1};
        vector<int> LEFT = {0,-1};
        vector<int> DOWN = {1,0};
        vector<int> UP = {-1,0};



    public:
        tank(vector<int> startingPosition){
            position = startingPosition;
        }

        vector<int> getPosition(){
            return position;
        }

};

int main(void){
    initscr();
    start_color();
    timeout(-1);
    nodelay(stdscr, true);
    keypad(stdscr, true);
    noecho();
    curs_set(0);

    //Color of walls
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    //Color of open space
    init_pair(2, COLOR_WHITE, COLOR_WHITE);
    //Color of player one
    init_pair(3, COLOR_CYAN, COLOR_CYAN);
    //Color of player two
    init_pair(4, COLOR_GREEN, COLOR_GREEN);

    //Start of main
    makeGrid();
    displayGrid();
    tank playerOne({0,0});
}

void makeGrid(void){
    for(int row = 0; row < height; row++){
        if(row % 2 == 0){
            vector<char> thisRow(width, 'X');
            grid.push_back(thisRow);

            continue; 
        }

        vector<char> thisRow;
        for(int column = 0; column < width; column++){
            if(column % 2 == 0){
                thisRow.push_back('X');
            }
            else thisRow.push_back(' ');
        }

        grid.push_back(thisRow);
    }
}

void displayGrid(void){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            int color;

            switch(grid[i][j]){
                case 'X':
                    color = 1;
                    break;
                case ' ':
                    color = 2;
                    break;
            }
            attron(COLOR_PAIR(color));
            mvprintw(i, j, "   ");
            attroff(COLOR_PAIR(color));
        }
        mvprintw(i, width, "\n");
    }
    refresh();
}