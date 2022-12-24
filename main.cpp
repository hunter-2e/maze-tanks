#include <ncurses.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <thread>
#include <queue>

void makeGrid(void);
void displayGrid(void);
void generateMaze(void);

using namespace std;

enum size {height = 23, width = 23};
vector<vector<char>> grid;

class tank{
    private:
        vector<int> position;
        char tankLetter;

        vector<int> RIGHT ={0,1};
        vector<int> LEFT = {0,-1};
        vector<int> DOWN = {1,0};
        vector<int> UP = {-1,0};

        void setStart(vector<int> startingPosition){
            position = {(startingPosition[0] * 2) + 1, (startingPosition[1] * 2) + 1};

            grid[(startingPosition[0] * 2) + 1][(startingPosition[1] * 2) + 1] = tankLetter;
        }


    public:
        tank(vector<int> startingPosition, char tankLetter){
            this->tankLetter = tankLetter;
            setStart(startingPosition);
        }

        vector<int> getPosition(){
            return position;
        }

};

class graphTracer: private tank {

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
    //Graph tracer color
    init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);

    //Start of main
    makeGrid();
    generateMaze();

    //tank playerOne({0,0}, 'H');

    displayGrid();
    
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
                case 'H':
                    color = 3;
                    break;
                case 'M':
                    color = 4;
                    break;
                case 'O':
                    color = 5;
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

void generateMaze(void){
    //Creating queue for depth first search
    queue<vector<int>> nodes;

    //List of visited nodes
    vector<int> visited;

    //Stating position of DFS
    vector<int> start = {height - 1, width - 1};

    graphTracer(start, 'O');

    //Push starting position onto queue and mark as visited
    nodes.push(start);
    visited.push(start);


    
}