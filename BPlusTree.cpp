#include"BPlusTree.h"
#include<string>
#include<stdlib.h>
using namespace std;


BPlusTree::BPlusTree(int n){

    keys = n;
    root = NULL;

};

BPlusTree::BPlusTree(BPlusTree &t){

    keys = t.keys;
    root = t.root;

};
