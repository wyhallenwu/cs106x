/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <iostream>  // for cout
#include <fstream> // for read file
using namespace std;

#include "console.h" // required of all files that contain the main function
#include "simpio.h"  // for getLine
#include "gevents.h" // for mouse event detection
#include "strlib.h"
#include "gwindow.h"

#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay

/**
 * Function: welcome
 * -----------------
 * Introduces the user to the Game of Life and its rules.
 */
static void welcome() {
    cout << "Welcome to the game of Life, a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells live and die by the following rules:" << endl << endl;
    cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
    cout << "\tLocations with 2 neighbors remain stable" << endl;
    cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
    cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl << endl;
    cout << "In the animation, new cells are dark and fade to gray as they age." << endl << endl;
    getLine("Hit [enter] to continue....   ");
}

void updateDisplay(LifeDisplay& display, Grid<int>& ages){
    for(auto& loc: ages.locations()){
        display.drawCellAt(loc.row, loc.col, ages.get(loc));
    }
}

void initilize(Grid<int>& ages, string& file, LifeDisplay& display) {
   if (file == "random"){
       ages.resize(randomInteger(40, 60), randomInteger(40, 60));
       for (const GridLocation& loc: ages.locations()) {
           ages.set(loc.row, loc.col, randomInteger(0, 1) == 0 ? randomInteger(1, kMaxAge):0);
       }
   } else {
       ifstream ifs{file};
       if(!ifs.is_open()){
           cout << "read file wrong. " << file << endl;
           ifs.close();
           exit(1);
       }
       string line;
       int rows, cols, count = -2;
       while(getline(ifs, line)){
           if (startsWith(line, '#')){
               continue;
           } else if (count == -2){
               rows = stringToInteger(line);
           } else if (count == -1) {
               cols = stringToInteger(line);
               ages.resize(rows, cols);
           } else {
               for(int i = 0; i < line.size(); i++){
                   int v = line[i] == 'X' ? 1 : 0;
                   ages.set(count, i, v);
               }
           }
           count++;
       }
       ifs.close();
   }
   display.setDimensions(ages.numRows(), ages.numCols());
   updateDisplay(display, ages);
   display.printBoard();
   cout << "initilize done." << endl;
}



int count_neighbors(const GridLocation& loc, Grid<int>& ages){
    auto neighbors_locs = loc.neighbors();
    int neighbors = 0;
    for (auto& current_loc: neighbors_locs){
        if (ages.inBounds(current_loc) && current_loc != loc && ages.get(current_loc) != 0)
            neighbors++;
    }
    return neighbors;
}

void rules(Grid<int>& temp_ages, Grid<int>& ages, const GridLocation& loc, int neighbors){
    switch (neighbors){
    case 2:
        if (ages.get(loc) != 0)
            temp_ages.set(loc, ages.get(loc) + 1);
        break;
    case 3:
        temp_ages.set(loc, ages.get(loc) + 1);
        break;
    default:
        temp_ages.set(loc, 0);
    }
}

bool checkStable(Grid<int>& ages){
    for(auto& loc: ages.locations()){
        int age = ages.get(loc);
        if (age != 0 && age < kMaxAge){
            return false;
        }
    }
    return true;
}

void checkRulesAndUpdateDisplay(LifeDisplay& display, Grid<int>& ages){
    Grid<int> temp_ages = ages;
    // count neighbors for each every location
    for(const GridLocation& ages_loc: ages.locations()){
        int neighbors = count_neighbors(ages_loc, ages);
        rules(temp_ages, ages, ages_loc, neighbors);
    }
    updateDisplay(display, temp_ages);
    display.printBoard();
    // advance to next generation
    ages = temp_ages;
}

int mapMs(string& render_mode){
    if (render_mode == "slow")
        return 2000;
    if (render_mode == "medium")
        return 1000;
    if (render_mode == "fast")
        return 500;
    int ms = getInteger("please input ms time");
    return ms;
}

void render(LifeDisplay& display, int ms){
    display.repaint();
    pause(ms);
}

void config(string& file, string& render_mode){
    cout << "please select file and display mode: " << endl;
    file = getLine("please input file:");
    render_mode = getLine("input render mode:");
}


/**
 * Function: main
 * --------------
 * Provides the entry point of the entire program.
 */
int main() {
    LifeDisplay display;
    display.setTitle("Game of Life");
//    welcome();
    // step 1: parse user input
    string file;  // "filename", "random"
    string render_mode; // "manual", "slow", "medium", "fast"
    int ms;
    config(file, render_mode);
    ms = mapMs(render_mode);
    // step 2: initilize
    Grid<int> ages;
    initilize(ages, file, display);
    while(true){
        checkRulesAndUpdateDisplay(display, ages);
        // render
        render(display, ms);
        // stable check
        if(checkStable(ages)){
            cout << "stable and quit" << endl;
            break;
        }
    }
    return 0;
}
