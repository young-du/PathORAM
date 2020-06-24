#include "Stack.h"
#include "Bucket.h"
#include "ServerStorage.h"
#include "OramInterface.h"
#include "RandForOramInterface.h"
#include "RandomForOram.h"
#include "UntrustedStorageInterface.h"
#include "OramReadPathEviction.h"
#include <iostream>
#include <string>
#include <sstream>
#include "StackTester.h"



void StackTester::runStackTest() {

	int bucketSize = 4;
	int numBlocks = pow(2, 5);
    Stack myStack(bucketSize, numBlocks);
    cout << "Built a stack\n";

	for (int i = 0; i < 20; i++){
        cout << "pushed " << i << " and the cache size is " << myStack.getCacheSize() << ".\n";
        myStack.push(i);
	}

    for (int i = 0; i < 13; i++){
        int popped = myStack.pop();
        cout << "popped " << popped << " and the cache size is " << myStack.getCacheSize() << ".\n";
	}
    for (int i = 0; i < 8; i++){
        cout << "pushed " << i << " and the cache size is " << myStack.getCacheSize() << ".\n";
        myStack.push(i);
	}

    for (int i = 0; i < 15; i++){
        int popped = myStack.pop();
        cout << "popped " << popped << " and the cache size is " << myStack.getCacheSize() << ".\n";
	}
}