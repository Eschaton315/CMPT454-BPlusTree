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

//#todo: once the search is implemented, add a false bool if the key already exists.

bool BPlusTree::insert(int n, string x){

//creating the first insertion as the root.
    if (root == NULL){
        root = new Node(keysMax);
        root->key[0] = n;
        root->value[0] = x;
        root->isLeaf = true;
        root->size = 1;
        return true;
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
//updating the nodes and assigning pointers
            current->key[i] = n;
            current->value[i] = x;
            current->size++;
            current->ptr[current->size] = current->ptr[current->size-1];
            current->ptr[current->size-1] = NULL;
            return true;
        }else{
//If nodes are full, create a new node for splitting
            Node *newLeaf = new Node(keysMax);
            int tempNode[keysMax+1];
            for (int i = 0; i<keysMax; i++){
                tempNode[i] = current->key[i];
            } 
            int i = 0,j;
            while(n > tempNode[i] && i<keysMax){
                i++;
            }
            for (int j = keysMax + 1; j > i; j--) {
                tempNode[j] = tempNode[j-1];
            }
            tempNode[i] = n;
            newLeaf->isLeaf = true;
            current->size = (keysMax+1)/2;
            newLeaf->size = keysMax + 1 -(keysMax + 1) / 2;
            current->ptr[current->size] = newLeaf;
            newLeaf->ptr[newLeaf->size] = current->ptr[keysMax];
            current->ptr[keysMax] = NULL;
            for(i = 0; i < current->size;i++){
                current->key[i] = tempNode[i];
            }
            for(i = 0, j = current->size; i < newLeaf->size; i++, j++){
                newLeaf->key[i] = tempNode[j];
            }
//Creating new root if we had to split the previous root            
            if(current == root) {
                Node *newRoot = new Node(keysMax);
                newRoot->key[0] = newLeaf->key[0];
                newRoot->ptr[0] = current;
                newRoot->ptr[1] = newLeaf;
                newRoot->isLeaf = false;
                newRoot->size = 1;
                root = newRoot;
                return true;
            }else{
//reorganizing keys to maintain the properties of a B+Tree
                insertInternal(newLeaf->key[0], parent, newLeaf);
            }

        }
    }
};

void BPlusTree::insertInternal(int n, Node *current, Node *child){

}