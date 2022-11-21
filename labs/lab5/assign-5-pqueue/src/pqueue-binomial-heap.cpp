#include "pqueue-binomial-heap.h"
using namespace std;

BinomialHeapPQueue::BinomialHeapPQueue() {}
BinomialHeapPQueue::~BinomialHeapPQueue() {
    for(auto& val: roots.values()){
        destructor_helper(val);
    }
}

void BinomialHeapPQueue::destructor_helper(node_bp* node){
    if(node->children.isEmpty()){
        delete node;
        return;
    }
    for(auto& child: node->children){
        destructor_helper(child);
    }
    delete node;
}

const string& BinomialHeapPQueue::peek() const {
    int k = roots.front();
    for(auto& key: roots.keys()){
        if(roots.get(key)->elem < roots.get(k)->elem){
            k = key;
        }
    }
    return roots.get(k)->elem;
}

void BinomialHeapPQueue::merge_single_tree(int index, node_bp* node){
    while(true){
        if(roots.containsKey(index)){
            node_bp* add = roots.get(index);
            if(node->elem > add->elem){
                add->children.push_back(node);
                node = add;
            } else{
                node->children.push_back(add);
            }
            roots.remove(index);
            index++;
        }else{
            roots.add(index, node);
            break;
        }
    }
}

string BinomialHeapPQueue::extractMin() {
    int k = roots.front();
    for(auto& key: roots.keys()){
        if(roots.get(key)->elem < roots.get(k)->elem){
            k = key;
        }
    }
    node_bp* node_min = roots.get(k);
    string min = node_min->elem;
    logSize--;
    roots.remove(k);
    // merge each sub binomial tree
    for(int i = 0; i < node_min->children.size(); i++){
        merge_single_tree(i, node_min->children[i]);
    }
    delete node_min;
    return min;
}

void printout(Map<int, node_bp*>& roots){
    for(auto& val: roots.values()){
        cout << val->elem << endl;
    }
}
void BinomialHeapPQueue::enqueue(const string& elem) {
    node_bp* insert = new node_bp(elem);
    logSize++;
    int key = 0;
    while(true){
        if(roots.containsKey(key)){
            node_bp* add = roots.get(key);
            if(insert->elem > add->elem){
                add->children.push_back(insert);
                insert = add;
            } else{
                insert->children.push_back(add);
            }
            roots.remove(key);
            key++;
        } else{
            roots.add(key, insert);
            break;
        }
    }
}

BinomialHeapPQueue *BinomialHeapPQueue::merge(BinomialHeapPQueue *one, BinomialHeapPQueue *two) {
    BinomialHeapPQueue* merged_heap = new BinomialHeapPQueue();
    // set heap one as basis
    merged_heap->roots = one->roots;
    merged_heap->logSize = one->logSize + two->logSize;
    for(auto& index: two->roots.keys()){
        merged_heap->merge_single_tree(index, two->roots.get(index));
    }
    return merged_heap;
}
