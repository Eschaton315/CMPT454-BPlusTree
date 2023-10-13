#ifndef BPLUSTREE_H_
#define BPLUSTREE_H_
#include<string>
using namespace std;

class Node {
    bool isLeaf;
    int *key, size;
    string *value;
    Node **ptr;
    friend class BPlusTree;

    public:
    Node(int);
    ~Node();
};

class BPlusTree{
    int keysMax;
    Node *root;
    
    public:
        BPlusTree(int);
        BPlusTree(BPlusTree &t);
        //~BPlusTree();
        bool insert(int, string);
        void insertInternal(int, string, Node*,Node*);
        Node *getParent(Node*, Node*);
        bool remove(int);
        string find(int);
        void printKeys();
        void printKeysRecurse(Node*);
        void printValues();
        void printValuesRecurse(Node*);

};

#endif /* BPLUSTREE_H_ */