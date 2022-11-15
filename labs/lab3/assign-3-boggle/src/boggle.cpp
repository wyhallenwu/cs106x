/**
 * File: boggle.cpp
 * ----------------
 * Implements the game of Boggle.
 */

#include <cctype>
#include <iostream>
#include <ctype.h>
#include "console.h"
#include "gboggle.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "random.h"
#include "grid.h"
#include "map.h"
#include "set.h"
#include "lexicon.h"

#include "gwindow.h"
using namespace std;

static const string kStandardCubes[16] = {
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

static const string kBigBoggleCubes[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

static const int kMinLength = 4;
static const double kDelayBetweenHighlights = 100;
static const double kDelayAfterAllHighlights = 500;

static const string kEnglishLanguageDatafile = "C:\\Users\\allenwu\\Desktop\\cs106x\\labs\\lab2\\assign-2-adts\\word-ladder\\res\\dictionary.txt";
static const Lexicon english(kEnglishLanguageDatafile);

//my adding
struct State{
    Set<GridLocation> used_locs;
    GridLocation current_loc;
    string word = "";
    bool operator<(const State& rhs) const{
        return word < rhs.word;
    }
};

static void shuffle_init(int dimension, Grid<char>& grid);
bool is_valid_state(const State& state);
bool has_prefix(const State& state);
void search(State state, Set<State>& solutions, const Grid<char>& grid);
Set<GridLocation> get_candidates(const State& state, const Grid<char>& grid);

/**
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */
static void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl;
    cout << endl;
}

/**
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */
static void giveInstructions() {
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl;
    cout << endl;
    cout << "Hit return when you're ready...";
    getLine(); // ignore return value
}

/**
 * Function: getPreferredBoardSize
 * -------------------------------
 * Repeatedly prompts the user until he or she responds with one
 * of the two supported Boggle board dimensions.
 */

static int getPreferredBoardSize() {
    cout << "You can choose standard Boggle (4x4 grid) or Big Boggle (5x5 grid)." << endl;
    return getIntegerBetween("Which dimension would you prefer: 4 or 5?", 4, 5);
}

/**
 * Function: playBoggle
 * --------------------
 * Manages all details needed for the user to play one
 * or more games of Boggle.
 */
static void playBoggle() {
    int dimension = getPreferredBoardSize();
    Grid<char> grid(dimension, dimension); // store the letters
    drawBoard(dimension, dimension);
    shuffle_init(dimension, grid);
    // precompute all results
    Set<State> solutions;
    State init_state;
    search(init_state, solutions, grid);
    // begin games

    cout << endl;
    cout << "This is where you'd play the game of Boggle" << endl;
}

static void shuffle_init(int dimension, Grid<char>& grid){
    //random dice
    Vector<char> vec;
    if(dimension == 4){
        for(int i=0; i < 16; i++){
            vec.add(kStandardCubes[i][randomInteger(0, 5)]);
        }
    } else if(dimension == 5){
        for(int i=0; i<25;i++){
            vec.add(kBigBoggleCubes[i][randomInteger(0, 5)]);
        }
    }
    // shuffle cubes
    for(int i = 0; i<vec.size(); i++){
        int index = randomInteger(i, vec.size() - 1);
        char temp = vec[index];
        vec[index] = vec[i];
        vec[i] = temp;
    }
    // label cubes
    for(int i = 0; i<vec.size();i++){
        labelCube(i / dimension, i % dimension, vec[i]);
        grid.set(i / dimension, i % dimension, vec[i]);
    }
}

bool is_valid_state(const State& state){
    if(state.word.size() >= 4 && english.contains(state.word)){
        return true;
    }
    return false;
}

bool has_prefix(const State& state){
    if (english.containsPrefix(state.word))
        return true;
    return false;
}

Set<GridLocation> get_candidates(const State& state, const Grid<char>& grid){
    Set<GridLocation> candidates;
    for(auto& loc: state.current_loc.neighbors()){
        if(grid.inBounds(loc) && loc != state.current_loc){
            candidates.add(loc);
        }
    }
    candidates.difference(state.used_locs);
    return candidates;
}

void update_state(State& state, const GridLocation& candidate, const Grid<char>& grid){
    state.word += toLowerCase(grid.get(candidate));
    state.used_locs.add(candidate);
    state.current_loc = candidate;
}

void back_state(State& state, const GridLocation& candidate, const GridLocation& current_loc){
    state.word.pop_back();
    state.used_locs.remove(candidate);
    state.current_loc = current_loc;
}

void search(State state, Set<State>& solutions, const Grid<char>& grid){
    if(is_valid_state(state)){
        solutions.add(state);
    }
    if(!has_prefix(state)){
        return;
    }
    for(auto& candidate: get_candidates(state, grid)){
        GridLocation current_loc = state.current_loc;
        update_state(state, candidate, grid);
        search(state, solutions, grid);
        back_state(state, candidate, current_loc);
    }
}



/**
 * Function: main
 * --------------
 * Serves as the entry point to the entire program.
 */
int main() {
    GWindow gw(kBoggleWindowWidth, kBoggleWindowHeight);
    initGBoggle(gw);
    welcome();
    if (getYesOrNo("Do you need instructions?")) giveInstructions();
    do {
        playBoggle();
    } while (getYesOrNo("Would you like to play again?"));
    cout << "Thank you for playing!" << endl;
    shutdownGBoggle();
    return 0;
}
