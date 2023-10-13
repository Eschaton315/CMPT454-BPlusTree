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
   // ~Node();
};

class BPlusTree{
    int keysMax;
    Node *root;
    Node *copyNode(Node*);
    void destroyTree(Node*);
    public:
        BPlusTree(int);
        BPlusTree(BPlusTree&);
        BPlusTree& operator=(BPlusTree&);
        ~BPlusTree();
        bool insert(int, string);
        void insertInternal(int, string, Node*,Node*);
        bool remove(int);
        void removeInternal(int, string, Node*, Node*);
        Node *getParent(Node*, Node*);
        string find(int);
        void printKeys();
        void printKeysRecurse(Node*);
        void printValues();
        void printValuesRecurse(Node*);

};

#endif /* BPLUSTREE_H_ */