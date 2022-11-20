#include "pqueue-linked-list.h"
using namespace std;

LinkedListPQueue::LinkedListPQueue() {}
LinkedListPQueue::~LinkedListPQueue() {
    while(logSize > 0){
        node* to_delete = head;
        head->next->prev = head->prev;
        head->prev->next = head->next;
        head = head->next;
        delete to_delete;
        logSize--;
    }
}

const string& LinkedListPQueue::peek() const {
    return head->elem;
}

string LinkedListPQueue::extractMin() {
    string min = head->elem;
    node* to_delete = head;
    head->next->prev = head->prev;
    head->prev->next = head->next;
    head = head->next;
    delete to_delete;
    logSize--;
    return min;
}

void LinkedListPQueue::enqueue(const string& elem) {
    if(head == nullptr){
        head = new node(elem);
        logSize++;
        return;
    }
    node* to_insert = head;
    node* insert = new node(elem);
    // indicate whether insert is prior than head or not
    bool flag = false;
    for(int i = 0; i < logSize; i++){
        if(elem < to_insert->elem){
            flag = i==0?true:false;
            break;
        }
        to_insert = to_insert->next;
    }
    // head should be the newly inserted elem is it is prior than head
    head = flag?insert:head;
    insert->next = to_insert;
    insert->prev = to_insert->prev;
    to_insert->prev->next = insert;
    to_insert->prev = insert;
    logSize++;
}

LinkedListPQueue *LinkedListPQueue::merge(LinkedListPQueue * lp1, LinkedListPQueue * lp2) {
    LinkedListPQueue* merge_lp = new LinkedListPQueue();
    node* lp1_index = lp1->head;
    node* lp2_index = lp2->head;
    for(int i = 0; i < lp1->logSize; i++){
        merge_lp->enqueue(lp1_index->elem);
        lp1_index = lp1_index->next;
    }
    for(int i = 0; i < lp2->logSize; i++){
        merge_lp->enqueue(lp2_index->elem);
        lp2_index = lp2_index->next;
    }
    return merge_lp;
}

