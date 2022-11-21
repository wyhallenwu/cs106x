#pragma once
#include "Vector.h"
#include "Map.h"
#include "pqueue.h"
#include <string>

struct node_bp{
    std::string elem;
    Vector<node_bp*> children;
    node_bp(std::string elem_):elem(elem_){}
};

class BinomialHeapPQueue : public PQueue {
public:
	BinomialHeapPQueue();
	~BinomialHeapPQueue();
	
	static BinomialHeapPQueue *merge(BinomialHeapPQueue *one, BinomialHeapPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
	
private:
    Map<int, node_bp*> roots;
    void merge_single_tree(int index, node_bp* head);
    void destructor_helper(node_bp* node);
    // logSize inheritance
    // logSize is the num of the roots in this binomial heap

};

