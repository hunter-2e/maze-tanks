#include <ncurses.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <thread>
#include <deque>
#include <algorithm>

using namespace std;

void makeGrid(void);
void displayGrid(void);
void generateMaze(void);
vector<int> randomDirection(vector<vector<int>> possibleDirections);

vector<int> RIGHT ={0,1};
vector<int> LEFT = {0,-1};
vector<int> DOWN = {1,0};
vector<int> UP = {-1,0}; 

enum size {height = 23, width = 23};
vector<vector<char>> grid;


class tank{
    public:
        vector<int> position;
        char letter;

        tank(vector<int> startingPosition, char letter){
            this->letter = letter;
            setStart(startingPosition);
        }

        vector<int> getPosition(){
            return position;
        }

        void setStart(vector<int> startingPosition){
            position = {(startingPosition[0] * 2) + 1, (startingPosition[1] * 2) + 1};

            grid[(startingPosition[0] * 2) + 1][(startingPosition[1] * 2) + 1] = letter;
        }

};

class graphTracer: public tank {
    using tank::tank;

    public:
        void drawTo(vector<int> direction){

            for(int draw = 0; draw < 2; draw++){
                position[0] += direction[0];
                position[1] += direction[1];

                grid[position[0]][position[1]] = letter;

                displayGrid();
                usleep(10000);
            }
            
        }

        vector<int> toVisit(vector<int> direction){
            return {position[0] + (direction[0]*2), position[1] + (direction[1]*2)};
        }

        void setTracer(vector<int> node){
            position = node;
        }
};

int main(void){
    srand((unsigned)time(NULL));
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
                    color = 5;
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
            mvprintw(i, j*2, "  ");
            attroff(COLOR_PAIR(color));
        }
        mvprintw(i, width*3, "\n");
    }
    refresh();
}

void generateMaze(void){
    //Creating queue for depth first search
    deque<vector<int>> nodes;

    //List of visited nodes
    vector<vector<int>> visited;

    //Stating position of DFS
    vector<int> start = {(height-1)/2 - 1, (width-1)/2 - 1};
    graphTracer tracer(start, ' ');

    //Push starting position onto queue and mark as visited
    nodes.push_front(start);
    visited.push_back(start);

    //Main loop of generation
    while(visited.size() < ((width - 1)/2 * (height - 1)/2) + 1){

        vector<vector<int>> possibleDirections = {RIGHT, LEFT, UP, DOWN};
        bool foundUnvisited = false;
        while(possibleDirections.size() > 0){
            mvprintw(50,50, "%d", possibleDirections.size());
            //Get random direction
            vector<int> randDirection = randomDirection(possibleDirections);
            //Check remove random direction for next choice
            possibleDirections.erase(remove(possibleDirections.begin(), possibleDirections.end(), randDirection), possibleDirections.end());

            //Check if already been visited
            vector<int> inVisited = tracer.toVisit(randDirection);
            if(inVisited[0] > height - 2 || inVisited[1] > width - 2 || inVisited[0] < 1 || inVisited[1] < 1){
                continue;
            }
            if(find(visited.begin(), visited.end(), inVisited) == visited.end()){
                //If not visited go to it and push new position into visted and queue
                tracer.drawTo(randDirection);

                visited.push_back(inVisited);
                nodes.push_front(inVisited);

                foundUnvisited = true;
                break;
        }

        }

        if(possibleDirections.size() == 0 && foundUnvisited == false){
            nodes.pop_front();

            tracer.setTracer(nodes.front());
        }
        


    }
    
}

vector<int> randomDirection(vector<vector<int>> possibleDirections){
    int random = rand() % possibleDirections.size();

    return possibleDirections[random];
}