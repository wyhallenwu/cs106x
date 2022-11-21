#include "pqueue-vector.h"
using namespace std;

VectorPQueue::VectorPQueue() {}
VectorPQueue::~VectorPQueue() {}

const string& VectorPQueue::peek() const{
    int index = this->find_min_index();
    return elems[index];
}

string VectorPQueue::extractMin() {
    int index = find_min_index();
    string find_result = elems[index];
    elems.remove(index);
    logSize -= 1;
    return find_result;
}

void VectorPQueue::enqueue(const string& elem) {
    elems.push_back(elem);
    logSize += 1;
}

VectorPQueue *VectorPQueue::merge(VectorPQueue *one, VectorPQueue * two) {
    VectorPQueue *new_pq = new VectorPQueue();
    for(auto& item: one->elems){
        new_pq->enqueue(item);
    }
    for(auto& item: two->elems){
        new_pq->enqueue(item);
    }
    return new_pq;
}

int VectorPQueue::find_min_index()const{
    if(elems.isEmpty()){
        return -1;
    }
    int index = 0;
    string compare = elems[0];
    for(int i = 0; i < this->elems.size(); i++){
        if(elems[i] < compare){
            index = i;
            compare = elems[i];
        }
    }
    return index;
}
