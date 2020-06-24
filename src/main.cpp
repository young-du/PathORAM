#include <iostream>
#include <cmath>
#include "Bucket.h"
#include "Block.h"
#include "RandomForOram.h"
#include "OramReadPathEviction.h"
#include "OramDeterministic.h"
#include "OramInterface.h"
#include "RandForOramInterface.h"
#include "UntrustedStorageInterface.h"
#include "ServerStorage.h"
#include "StackTester.h"
#include "StashSizeSimulator.h"

using namespace std;

int main() {
    cout << "Stack Test:" << endl;
    StackTester * tester = new StackTester();
    tester->runStackTest();
}
