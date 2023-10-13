#include"BPlusTree.h"
#include<string>
#include<iostream>
#include<stdlib.h>
using namespace std;


BPlusTree::BPlusTree(int n){

    keysMax = n;
    root = NULL;

}

BPlusTree::BPlusTree(BPlusTree &t){

    keysMax = t.keysMax;
    root = t.root;

}

Node::Node(int n){

    key = new int[n];
    value = new string[n];
    ptr = new Node *[n+1];

}



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
//check if there is a value in the key.
        if(!find(n).empty()){
        cout<<"Key already being used"<<endl;
        return false;
        }
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
                current->value[j] = current->value[j-1];
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
            int tempKeys[keysMax+1];
            string tempValues[keysMax+1];
            for (int i = 0; i<keysMax; i++){
                tempKeys[i] = current->key[i];
                tempValues[i] = current->value[i];
            } 
            int i = 0,j;
            while(n > tempKeys[i] && i<keysMax){
                i++;
            }
            for (int j = keysMax /*+ 1*/; j > i; j--) {
                tempKeys[j] = tempKeys[j-1];
                tempValues[j] = tempValues[j-1];
            }
            tempKeys[i] = n;
            tempValues[i] = x;
            newLeaf->isLeaf = true;
            current->size = (keysMax+1)/2;
            newLeaf->size = keysMax + 1 - current->size;
            current->ptr[current->size] = newLeaf;
            newLeaf->ptr[newLeaf->size] = current->ptr[keysMax];
            current->ptr[keysMax] = NULL;
            for(i = 0; i < current->size;i++){
                current->key[i] = tempKeys[i];
                current->value[i] = tempValues[i];
            }
            for(i = 0, j = current->size; i < newLeaf->size; i++, j++){
                newLeaf->key[i] = tempKeys[j];
                newLeaf->value[i] = tempValues[j];
            }
//Creating new root if we had to split the previous root            
            if(current == root) {
                Node *newRoot = new Node(keysMax);
                newRoot->key[0] = newLeaf->key[0];
                newRoot->value[0] = newLeaf->value[0];
                newRoot->ptr[0] = current;
                newRoot->ptr[1] = newLeaf;
                newRoot->isLeaf = false;
                newRoot->size = 1;
                root = newRoot;
                
            }else{
//reorganizing keys to maintain the properties of a B+Tree
                insertInternal(newLeaf->key[0], newLeaf->value[0], parent, newLeaf);
            }
            return true;
        }
    }
}


void BPlusTree::insertInternal(int n, string x, Node *current, Node *child){
//if parent node still has space, add the new key in the parent node
    if(current->size < keysMax){
        int i = 0;
        while (n > current->key[i] && i < current->size){
            i++;
        }
        for (int j = current->size; j > i; j--) {
            current->key[j] = current->key[j-1];
            current->value[j] = current->value[j-1];
        }
        for (int j = current->size + 1; j > i; j--){
            current->ptr[j] = current->ptr[j-1];
        }
        current->key[i] = n;
        current->value[i] = x;
        current->size++;;
        current->ptr[i+1] = child;
    }else{
//create new internal node and split the current one
        Node *newInternal = new Node(keysMax);
        int tempKeys[keysMax + 1];
        string tempValues[keysMax + 1];
        Node *tempPtr[keysMax + 2];
        for (int i = 0; i < keysMax; i++){
            tempKeys[i] = current->key[i];
            tempValues[i] = current->value[i];
        }
        for(int i = 0; i < keysMax + 1; i++){
            tempPtr[i] = current->ptr[i];
        }
        int i =0,j;
        while (n > tempKeys[i] && i < keysMax){
            i++;
        }
        for(int j = keysMax /*+ 1*/; j > i; j--){
            tempKeys[j] = tempKeys[j-1];
            tempValues[j] = tempValues[j-1];
        }
        for(int j = keysMax + 2; j > i + 1; j--){
            tempPtr[j] = tempPtr[j-1];
        }
        tempKeys[i] = n;
        tempValues[i] = x;
        tempPtr[j]  = child;
        newInternal->isLeaf = false;
        current->size = (keysMax + 1) / 2;
        newInternal->size = keysMax - (keysMax + 1) / 2;
        for(i = 0, j = current->size + 1; i < newInternal->size; i++, j++){
            newInternal->key[i] = tempKeys[j];
            newInternal->value[i] = tempValues[j];
        }
        for(i = 0, j = current->size + 1; i< newInternal->size + 1; i++, j++){
            newInternal->ptr[i] = tempPtr[j];
        }
//Creating new root if we had to split the previous root
        if(current == root){
            Node *newRoot = new Node(keysMax);
                newRoot->key[0] = current->key[current->size];
                newRoot->value[0] = current->value[current->size];
                newRoot->ptr[0] = current;
                newRoot->ptr[1] = newInternal;
                newRoot->isLeaf = false;
                newRoot->size = 1;
                root = newRoot;
        }else{
            insertInternal(current->key[current->size], current->value[current->size],getParent(root,current),newInternal);
        }
    }
}

Node *BPlusTree::getParent(Node *current, Node *child){
    Node *parent;
    if(current->isLeaf || (current->ptr[0])->isLeaf){
        return NULL;
    }
    for (int i = 0; i < current->size + 1; i++){
        if(current->ptr[i] == child){
            parent = current;
            return parent;
        }else{
            parent = getParent(current->ptr[i], child);
            if (parent != NULL){
                return parent;
            }
        }
    }
    return parent;
}


bool BPlusTree::remove(int n){
    if(root == NULL){
        cout << "empty tree" << endl;
        return false;
    }else{
//Search if the key exists
        Node *current = root;
        Node *parent;
        int leftSib, rightSib;
        while (!current->isLeaf){
            
        }
    }
}


void BPlusTree::printKeys(){
    Node *current = root;
    printKeysRecurse(current);   
}

void BPlusTree::printKeysRecurse(Node *current){
    if(current != NULL){
        cout <<"[";
        for(int i = 0; i < current->size; i++){
            cout << current->key[i];
            if(i < current->size - 1){
                 cout << ",";
            }
        }
        cout << "]";
        if(!current->isLeaf){
            cout << endl;
            for(int i=0; i<current->size + 1; i++){
                printKeysRecurse(current->ptr[i]);
                if(i == current->size){
                    cout<< endl;
                }
            }
        }
    }
}

void BPlusTree::printValues(){
    Node *current  = root;
    printValuesRecurse(current);
}

void BPlusTree::printValuesRecurse(Node* current){
    if(current != NULL){
        for(int i = 0; i<current->size; i++){
            if(current->isLeaf){
                cout << current->value[i];
                cout << endl;
            }
        }
        if(!current->isLeaf){
            for(int i = 0; i<current->size + 1; i++){
                printValuesRecurse(current->ptr[i]);
            }
        }
    }
}


string BPlusTree::find(int n){
    if(root == NULL){
        cout<<"empty tree"<<endl;
        return {};
    }
    else{
        Node *current = root;
        while (!current->isLeaf){
            for(int i = 0; i < current->size; i++){
                if(n < current->key[i]){
                    current = current->ptr[i];
                    break;
                }
                if(i == current->size - 1){
                    current = current->ptr[i+1];
                    break;
                }
            }
        }
        for(int i = 0; i < current->size; i++){
            if(current->key[i] == n){
                return current->value[i];
            }
        }
        return {};
    }
}