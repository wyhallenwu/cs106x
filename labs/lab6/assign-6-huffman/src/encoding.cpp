// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include "bitstream.h"
#include "pqueue.h"
#include "filelib.h"
using namespace std;
// TODO: include any other headers you need

Map<int, int> buildFrequencyTable(istream& input) {
    // TODO: implement this function
    Map<int, int> freqTable; // this is just a placeholder so it will compile
    while(!input.fail()){
        char ch = input.get();
        if(freqTable.containsKey(int(ch))){
            freqTable[int(ch)] += 1;
        } else{
            freqTable.add(ch == EOF ? PSEUDO_EOF:int(ch), 1);
            if(ch == EOF){
                break;
            }
        }
    }
    return freqTable;          // this is just a placeholder so it will compile
}

HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    // TODO: implement this function
    // priority queue
    PriorityQueue<HuffmanNode*> pq;
    for(auto& key: freqTable.keys()){
        HuffmanNode* node = new HuffmanNode(key, freqTable.get(key));
        pq.add(node, double(node->count));
    }
    // build encoding tree
    if(pq.size() == 1){
        HuffmanNode* node1 = pq.dequeue();
        HuffmanNode* node = new HuffmanNode(NOT_A_CHAR, node1->count, node1, NULL);
        return node;
    }
    while(pq.size() > 1){
        HuffmanNode* node1 = pq.dequeue();
        HuffmanNode* node2 = pq.dequeue();
        HuffmanNode* node = new HuffmanNode(NOT_A_CHAR, node1->count + node2->count, node1, node2);
        pq.enqueue(node, node->count);
    }
    return pq.dequeue();   // this is just a placeholder so it will compile
}

void traverse(string& result, HuffmanNode* node, Map<int, string>& map){
    if(node->isLeaf()){
        map[node->character] = result;
        return;
    }
    // backtrack
    if(node->zero != NULL){
        result += '0';
        traverse(result, node->zero, map);
        result.pop_back();
    }
    if(node->one != NULL){
        result += '1';
        traverse(result, node->one, map);
        result.pop_back();
    }
    return;
}

Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    // TODO: implement this function
    Map<int, string> encodingMap;
    string result;
    traverse(result, encodingTree, encodingMap);
    return encodingMap;
}

void write_binary(const string& binary, obitstream& output){
    for(auto iter = binary.begin(); iter != binary.end(); ++iter){
        output.writeBit(int(*iter - '0'));
    }
}

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    // TODO: implement this function
    rewindStream(input);
    while(!input.fail()){
        char ch = input.get();
        string binary = encodingMap.get(ch == EOF ? PSEUDO_EOF: int(ch));
        write_binary(binary, output);
        if(ch == EOF){
            break;
        }
    }
}

int pattern_match_huffman_node(HuffmanNode*& start_node, int bit){
    start_node = bit == 0 ? start_node->zero : start_node->one;
    return start_node->isLeaf() ? start_node->character : NOT_A_CHAR;
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    // TODO: implement this function
    HuffmanNode* node = encodingTree;
    while(!input.fail()){
        int bit = input.readBit();
        int match_result = pattern_match_huffman_node(node, bit);
        if(match_result != NOT_A_CHAR){
            if(match_result == PSEUDO_EOF){
                break;
            }
            output << char(match_result);
            node = encodingTree;
        }
    }
}

void compress(istream& input, obitstream& output) {
    // TODO: implement this function
    Map<int, int> freqTable = buildFrequencyTable(input);
    output << freqTable;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    encodeData(input, encodingMap, output);
}

void decompress(ibitstream& input, ostream& output) {
    // TODO: implement this function
    Map<int, int> freqTable;
    input >> freqTable;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    decodeData(input, encodingTree, output);
}

void freeTree(HuffmanNode* node) {
    // TODO: implement this function
    if(node == NULL){
        return;
    }
    if(node->isLeaf()){
        delete node;
        return;
    }
    freeTree(node->zero);
    freeTree(node->one);
 }
