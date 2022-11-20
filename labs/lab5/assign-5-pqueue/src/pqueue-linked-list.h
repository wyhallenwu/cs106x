#pragma once
#include "pqueue.h"
#include <string>

struct node{
    node* next;
    node* prev;
    std::string elem;
    node():next(nullptr), prev(nullptr){}
    explicit node(std::string elem_):next(this), prev(this), elem(elem_){}
};

class LinkedListPQueue : public PQueue {
public:
	LinkedListPQueue();
	~LinkedListPQueue();
	
	static LinkedListPQueue *merge(LinkedListPQueue *one, LinkedListPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
	
private:
    node* head = nullptr;
    //logSize inheritance
};
