#include <deque>
#include <cmath>
#include <queue>
#include "Block.h"
#include "Bucket.h"
#include "OramInterface.h"
#include "RandForOramInterface.h"
#include "RandomForOram.h"
#include "UntrustedStorageInterface.h"
#include "OramReadPathEviction.h"
#include "ServerStorage.h"
#include <iostream>
using namespace std;

struct StackEntry {
    int data;
    int prev_id;
};

class Stack {
    private:
    int bucketSize;
	int numBlocks;
    int blockLength;
	UntrustedStorageInterface* storage;
	RandForOramInterface* random;
	OramInterface* oram;
    deque<StackEntry> localCache;
    int opcount = 0;
    void accessORAM();
    int nextid();
    queue<int> availblocks;

    public:
    Stack(int bs=5, int nb=32);
    ~Stack();
    void push(int x);
    int pop();
    int getCacheSize();
};


Stack::Stack(int bs, int nb) {
    this->bucketSize = bs;
	this->numBlocks = nb;
	Bucket::setMaxSize(bucketSize);	
	this->storage = new ServerStorage();
	this->random = new RandomForOram();
	this->oram = new OramReadPathEviction(storage, random, bucketSize, numBlocks);
    this->blockLength = floor(log2(numBlocks));
    cout << blockLength << endl;
    for (int i = 0; i < numBlocks; ++i) {
        availblocks.push(i);
    }
}


Stack::~Stack() {
    delete this->storage;
    delete this->random;
    delete this->oram;
}

void Stack::push(int x) {
    opcount ++;
    StackEntry to_push;
    to_push.data = x;
    to_push.prev_id = -1;
    localCache.push_front(to_push);
    if (opcount == blockLength) {
        accessORAM();
        opcount = 0;
    }
}

int Stack::pop() {
    opcount ++;
    auto to_pop = localCache.front();
    localCache.pop_front();
    if (opcount == blockLength) {
        accessORAM();
        opcount = 0;
    }
    return to_pop.data;
}

int Stack::nextid() {
    int i = availblocks.front();
    availblocks.pop();
    return i;
}

void Stack::accessORAM() {
    cout << "accessed !!"<< endl;
    if (localCache.size() < blockLength) {
        while (localCache.size() < blockLength) {
            StackEntry a;
            int id = localCache[localCache.size()-1].prev_id;
            if (id == -1) break;
            int* accessed = oram->access(OramInterface::Operation::READ, id , NULL);
            a.data = accessed[0];
            a.prev_id = accessed[1];
            delete accessed;
            availblocks.push(id);
            localCache.push_back(a);
        }
    } else if (localCache.size() > 2 * blockLength) {
        while (localCache.size() > 2 * blockLength) {
            int id = nextid();
            localCache[localCache.size()-2].prev_id = id;
            int* newArray = new int[Block::BLOCK_SIZE];
            newArray[0] = localCache[localCache.size()-1].data;
            newArray[1] = localCache[localCache.size()-1].prev_id;
            oram->access(OramInterface::Operation::WRITE, id, newArray);
            localCache.pop_back();
        }
    } else {
        // perform a dummy read
        oram->access(OramInterface::Operation::READ, 0, NULL);
    }
}

int Stack::getCacheSize() {
    return localCache.size();
}