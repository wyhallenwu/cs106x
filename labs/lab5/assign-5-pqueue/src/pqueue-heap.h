#pragma once
#include "pqueue.h"
#include <string>

class HeapPQueue : public PQueue {
public:
	HeapPQueue();
	~HeapPQueue();
	
	static HeapPQueue *merge(HeapPQueue *one, HeapPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
    
private:
    // provide data methods and helper methods to
    // help realize the binary heap-backed PQueue
    std::string* elems;
    // logSize inheritance
    int capacity;
    void balance_topdown();
    void balance_downtop();
    void construct_heap_recursive(int index);
};
