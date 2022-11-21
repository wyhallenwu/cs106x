#include "pqueue-heap.h"
#include <iostream>
#include <algorithm>
using namespace std;

HeapPQueue::HeapPQueue():elems(new string[4]), capacity(4){logSize=0;}
HeapPQueue::~HeapPQueue() {delete []elems;}

const string& HeapPQueue::peek() const {
    return elems[0];
}

void HeapPQueue::balance_topdown(){
    int index = 0;
    while(index < logSize){
        int left = (index+1) * 2 - 1;
        int right = (index+1) * 2;
        if(left >= logSize){
            return;
        }
        string l = elems[left];
        string r = right<logSize?elems[right]:string(elems[left].size(), 'z');
        int exchange_index = l <= r ? left:right;
        if(elems[index] > elems[exchange_index]){
            elems[index].swap(elems[exchange_index]);
            index = exchange_index;
        } else{
            return;
        }
    }

}

string HeapPQueue::extractMin() {
    string min = elems[0];
    elems[0] = elems[logSize - 1];
    logSize--;
    balance_topdown();
    return min;
}

void HeapPQueue::balance_downtop(){
    int index = logSize - 1;
    int exchange_index = (index + 1) / 2 - 1;
    while(exchange_index >= 0){
        if(elems[index] < elems[exchange_index]){
            elems[index].swap(elems[exchange_index]);
        } else{
            return;
        }
        index = exchange_index;
        exchange_index = (exchange_index + 1) / 2 - 1;
    }
}

void HeapPQueue::enqueue(const string& elem) {
    if(logSize == capacity){
        string* new_elems = new string[2 * capacity];
        for(int i = 0; i < capacity; i++){
            new_elems[i] = elems[i];
        }
        delete []elems;
        elems = new_elems;
        capacity = 2 * capacity;
    }
    elems[logSize] = elem;
    logSize++;
    balance_downtop();
}

// a simple version which is different from the lab instructions;
// In original requirement, it concatenates the two hp, and construct the heap
// down-top recursively.
//HeapPQueue *HeapPQueue::merge(HeapPQueue * hp1, HeapPQueue * hp2) {
//    HeapPQueue* hp = new HeapPQueue();
//    hp->capacity = hp1->capacity + hp2->capacity;
//    delete[] hp->elems;
//    hp->elems = new string[hp->capacity];
//    for(int i = 0; i < hp1->logSize; i++){
//        hp->enqueue(hp1->elems[i]);
//    }
//    for(int i = 0; i < hp2->logSize; i++){
//        hp->enqueue(hp2->elems[i]);
//    }
//    return hp;
//}

void HeapPQueue::construct_heap_recursive(int index){
    int left = (index + 1) * 2 - 1;
    int right = (index + 1) * 2;
    if(left >= logSize){
        return;
    }
    string l = elems[left];
    string r = right < logSize? elems[right]:string(elems[left].size(), 'z');
    int compare_index = l <= r? left: right;
    if(elems[index] > elems[compare_index]){
        elems[index].swap(elems[compare_index]);
    } else{
        return;
    }
    construct_heap_recursive(compare_index);
}

HeapPQueue* HeapPQueue::merge(HeapPQueue *one, HeapPQueue *two){
    // initialize hp
    HeapPQueue* hp = new HeapPQueue();
    hp->capacity = one->capacity + two->capacity;
    delete[] hp->elems;
    hp->elems = new string[hp->capacity];
    // concatenated
    for(int i = 0; i < one->logSize; i++){
        hp->elems[i] = one->elems[i];
        hp->logSize++;
    }
    for(int i = 0; i< two->logSize; i++){
        hp->elems[one->logSize + i] = two->elems[i];
        hp->logSize++;
    }
    // construct
    for(int i = hp->logSize - 1; i>=0;i--){
        hp->construct_heap_recursive(i);
    }
    return hp;
}
