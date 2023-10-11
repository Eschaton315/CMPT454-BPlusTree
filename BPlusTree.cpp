#include"BPlusTree.h"
#include<string>
#include<stdlib.h>
using namespace std;


BPlusTree::BPlusTree(int n){

    keysMax = n;
    root = NULL;

};

BPlusTree::BPlusTree(BPlusTree &t){

    keysMax = t.keysMax;
    root = t.root;

};

Node::Node(int n){

    key = new int[n];
    value = new string[n];
    ptr = new Node *[n+1];

};

bool BPlusTree::insert(int n, string x){

//creating the first insertion as the root.
    if (root == NULL){
        root = new Node(n);
        root->key[0] = n;
        root->value[0] = x;
        root->isLeaf = true;
        root->size = 1;
    }else{
//If first node exists, traverse down to the leaf nodes.
        Node *current =  root;
        Node *parent;
        while(current->isLeaf == false){
            parent = current;
            for (int i=0; i < current->size; i++){
                if (n < current->key[i]){
                    current = current->ptr[i];
                    break;
                }
                if (i == current->size - 1){
                    current = current->ptr[i + 1];
                    break;
                }
            }
        }
//check if the leaf node is full. If not, insert key and value
        if(current->size < keysMax){
            int i = 0;
            while (n > current->key[i] && i < current->size){
                i++;
            }
            for (int j = current->size; j>i ; j--){
                current->key[j] = current->key[j-1];
            }
            current->key[i] = n;
            current->value[i] = x;
            current

        } 
    }


};