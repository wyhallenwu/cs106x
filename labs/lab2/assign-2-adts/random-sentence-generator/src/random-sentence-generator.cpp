/**
 * File: random-sentence-generator.cpp
 * -----------------------------------
 * Presents a short program capable of reading in
 * context-free grammar files and generating arbitrary
 * sentences from them.
 */

#include <iostream>
#include <fstream>
using namespace std;

#include "console.h"
#include "simpio.h"   // for getLine
#include "strlib.h"   // for toLowerCase, trim
#include <map>
#include <vector>

static const string kGrammarsDirectory = "C:\\Users\\allenwu\\Desktop\\cs106x\\labs\\lab2\\assign-2-adts\\random-sentence-generator\\res\\grammars\\";
static const string kGrammarFileExtension = ".g";
static string getNormalizedFilename(string filename) {
    string normalizedFileName = kGrammarsDirectory + filename;
    if (!endsWith(normalizedFileName, kGrammarFileExtension))
        normalizedFileName += kGrammarFileExtension;
    return normalizedFileName;
}

static bool isValidGrammarFilename(string filename) {
    string normalizedFileName = getNormalizedFilename(filename);
    ifstream infile(normalizedFileName.c_str());
    return !infile.fail();
}

static string getFileName() {
    while (true) {
        string filename = trim(getLine("Name of grammar file? [<return> to quit]: "));
        if (filename.empty() || isValidGrammarFilename(filename)) return getNormalizedFilename(filename);
        cout << "Failed to open the grammar file named \"" << filename << "\". Please try again...." << endl;
    }
}

void read_file(const string& filename, map<string, vector<string>>& m){
    ifstream ifs(filename);
    string line;
    if(!ifs.is_open()){
        cout << "open file wrong" << endl;
        return;
    }
    while(getline(ifs, line)){
        string slength;
        vector<string> v;
        getline(ifs, slength);
        int length = std::atoi(slength.c_str());
        int i = 0;
        while(i < length){
            string item;
            getline(ifs, item);
            v.push_back(item);
            ++i;
        }
        m[line] = v;
        // skip the blank line
        getline(ifs, line);
    }
    ifs.close();
}

string randomChoose(vector<string>& v){
    int length = v.size();
    return v.at(randomInteger(0, length-1));
}

bool isDone(const string& result){
    for(auto& ch : result){
        if(ch == '<'){
            return false;
        }
    }
    return true;
}

string search(const string& origin, map<string, vector<string>>& m){
    string result;
    for(int i=0; i<origin.size();i++){
        if(origin[i] == '<'){
            string token;
            while(origin[i] != '>'){
                token += origin[i++];
            }
            token += '>';
            // randomly substitute
            result += randomChoose(m.at(token));
        } else {
            result += origin[i];
        }
    }
    return result;
}

string multi_search(string start, map<string, vector<string>>& m){
    while(true){
        string result = search(start, m);
        start = result;
        if (isDone(result)){
            return result;
        }
    }
    return "";
}

int main() {
    while (true) {
        string filename = getFileName();
        if (filename.empty()) break;
        cout << "Here's where you read in the \"" << filename << "\" grammar "
             << "and generate three random sentences." << endl;
        map<string, vector<string>> m;
        read_file(filename, m);
        string start = m["<start>"].front();
        string result = multi_search(start, m);
        cout << result << endl;
    }
    
    cout << "Thanks for playing!" << endl;
    return 0;
}
