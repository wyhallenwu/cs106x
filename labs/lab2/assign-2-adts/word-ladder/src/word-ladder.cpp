/**
 * File: word-ladder.cpp
 * ---------------------
 * Implements a program to find word ladders connecting pairs of words.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "lexicon.h"
#include "strlib.h"
#include "simpio.h"
#include "queue.h"

static string getWord(const Lexicon& english, const string& prompt) {
    while (true) {
        string response = trim(toLowerCase(getLine(prompt)));
        if (response.empty() || english.contains(response)) return response;
        cout << "Your response needs to be an English word, so please try again." << endl;
    }
}

// avoid reuse those words which are already in use
bool isUsed(string& word, Vector<string>& used){
    foreach (string w, used) {
        if(w == word){
            return true;
        }
    }
    used.add(word);
    return false;
}

string find_substitute_at_index(const Lexicon& english, Vector<string>& used, const Vector<string>& ladder, Queue<Vector<string>>& ladders, const string& end){
    for (int i = 0; i < end.length(); i++){
        for (char c='a'; c <= 'z'; c++){
            string new_word = ladder.back();
            if (c == new_word[i])
                continue;
            new_word[i] = c;
            if (new_word == end)
                return new_word;
            if(english.contains(new_word) && !isUsed(new_word, used)){
                Vector<string> new_ladder = ladder;
                new_ladder.add(new_word);
                ladders.enqueue(new_ladder);
            }
        }
    }
    return "";
}

Vector<string> bfs(const Lexicon& english, const string& end, Vector<string>& used, Queue<Vector<string>>& ladders){
    while(!ladders.isEmpty()){
        Vector<string> ladder = ladders.dequeue();
        string result = find_substitute_at_index(english, used, ladder, ladders, end);
        if(result != ""){
            ladder.add(result);
            return ladder;
        }
    }
    return Vector<string>();
}

static void generateLadder(const Lexicon& english, const string& start, const string& end) {
    cout << "Here's where you'll search for a word ladder connecting \"" << start << "\" to \"" << end << "\"." << endl;
    // create two ladders
    Vector<string> ladder;
    Vector<string> used;
    Queue<Vector<string>> ladders;
    //initilize
    ladder.add(start);
    used.add(start);
    ladders.enqueue(ladder);
    // breadth-first search
    Vector<string> result = bfs(english, end, used, ladders);
    if (!result.isEmpty()){
        foreach (string v, result) {
            cout << v << "->";
        }
        cout << endl;
    }else {
        cout << "no ladders" << endl;
    }
}

static const string kEnglishLanguageDatafile = "C:\\Users\\allenwu\\Desktop\\cs106x\\labs\\lab2\\assign-2-adts\\word-ladder\\res\\dictionary.txt";
static void playWordLadder() {
    Lexicon english(kEnglishLanguageDatafile);
    while (true) {
        string start = getWord(english, "Please enter the source word [return to quit]: ");
        if (start.empty()) break;
        string end = getWord(english, "Please enter the destination word [return to quit]: ");
        if (end.empty()) break;
        generateLadder(english, start, end);
    }
}

int main() {
    cout << "Welcome to the CS106 word ladder application!" << endl << endl;
    playWordLadder();
    cout << "Thanks for playing!" << endl;
    return 0;
}
