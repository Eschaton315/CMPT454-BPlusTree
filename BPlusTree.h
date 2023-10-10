#ifndef BPLUSTREE_H_
#define BPLUSTREE_H_

class Node {
    bool isLeaf;
    int *key, size;
    string *value;
    Node **ptr;

    public:
    Node();
    ~Node();
};

class BPlusTree{
    int keys;
    Node *root;
    
    public:
        BPlusTree(int);
        BPlusTree(BPlusTree &t);
        ~BPlusTree();
        bool insert(int, string);
        bool remove(int);
        string find(int);
        void printKeys();
        void printValues();

};

#endif /* BPLUSTREE_H_ */