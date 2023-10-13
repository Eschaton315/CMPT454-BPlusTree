#include"BPlusTree.h"
#include<string>
#include<iostream>
#include<stdlib.h>
using namespace std;

//constrctor
BPlusTree::BPlusTree(int n){
    keysMax = n;
    root = NULL;
}

//copy constructor
BPlusTree::BPlusTree(BPlusTree &t){
    keysMax = t.keysMax;
    root = copyNode(t.root);
}

//overloaded assigned operator
BPlusTree& BPlusTree::operator=(BPlusTree &t){
    if (this == &t){
        return *this;
    }
    this->~BPlusTree();
    keysMax = t.keysMax;
    root = copyNode(t.root);
    return *this;
}

Node *BPlusTree::copyNode(Node* original){
    if(original == NULL){
        return NULL;
    }
    Node* newNode = new Node(keysMax);
    newNode->size = original->size;
    newNode->isLeaf = original->isLeaf;
    
    for (int i = 0; i < original->size; i++) {
        newNode->key[i] = original->key[i];
        newNode->value[i] = original->value[i];
    }
    
    if (!original->isLeaf) {
        for (int i = 0; i <= original->size; i++) {
            newNode->ptr[i] = copyNode(original->ptr[i]);
        }
    }
    return newNode; 
}

//destructor
BPlusTree::~BPlusTree() {
    destroyTree(root);
}

void BPlusTree::destroyTree(Node* node) {
    if (node == NULL) {
        return;
    }

    if (!node->isLeaf) {
        for (int i = 0; i <= node->size; i++) {
            destroyTree(node->ptr[i]);
        }
    }else{
        for (int i = 0; i <= node->size; i++) {
            remove(node->key[i]);
        }
    }

    
}


Node::Node(int n){

    key = new int[n];
    value = new string[n];
    ptr = new Node *[n+1];

}

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
            for (int j = current->size; j > i ; j--){
                current->key[j] = current->key[j-1];
                current->value[j] = current->value[j-1];
            }
//updating the nodes and assigning pointers
            current->key[i] = n;
            current->value[i] = x;
            current->size++;
            current->ptr[current->size] = current->ptr[current->size - 1];
            current->ptr[current->size-1] = NULL;
            return true;
        }else{
//If nodes are full, create a new node for splitting
            Node *newLeaf = new Node(keysMax);
            int tempKeys[keysMax+1];
            string *tempValues = new string[keysMax+1];
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
        current->size++;
        current->ptr[i+1] = child;
    }else{
//create new internal node and split the current one
        Node *newInternal = new Node(keysMax);
        int tempKeys[keysMax + 1];
        string *tempValues = new string[keysMax + 1];
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
            for (int i = 0; i< current->size; i++){
                parent = current;
                leftSib = i - 1;
                rightSib = i + 1;
                if(n < current->key[i]){
                    current = current->ptr[i];
                    break;
                }
                if(i == current->size - 1){
                    leftSib = i;
                    rightSib = + 2;
                    current = current->ptr[i+1];
                    break;
                }
            }
        }
        bool exists = false;
        int nDelete;
        for (nDelete = 0; nDelete < current->size; nDelete++){
            if(current->key[nDelete] == n){
                exists = true;
                break;
            }
        }
        if(!exists){
//if the key doesn't exist, return false, otherwise, remove key and its value
            cout<<"key not found"<<endl;
            return false;
        }
        for(int i = nDelete; i < current->size; i++){
            current->key[i] = current->key[i+1];
            current->value[i] = current->value[i+1];
        }
        current->size--;
//Dealing with tree if the removal happenes at the root node
        if(current == root){
            for(int i = 0; i < keysMax + 1; i++){
                current->ptr[i] = NULL;
            }
            if(current->size == 0){
                cout<<"tree is now empty"<<endl;
                delete[] current->key;
                delete[] current->value;
                delete[] current->ptr;
                delete[] current;
                root = NULL;
            }
            return true;
        }
//If the amount of children still meets the B+Tree's properties
        current->ptr[current->size] = current->ptr[current->size + 1];
        current->ptr[current->size + 1] = NULL;
        if(current->size >= (keysMax + 1) / 2){
            return true;
        }
//Redistribute to the left side
        if(leftSib >= 0){
            Node *leftNode = parent->ptr[leftSib];
            if (leftNode->size >= (keysMax + 1) / 2 + 1){
                for(int i = current->size; i > 0; i--){
                    current->key[i] = current->key[i-1];
                    current->value[i] = current->value[i-1];
                }
                current->size++;
                current->ptr[current->size] = current->ptr[current->size - 1];
                current->ptr[current->size - 1] = NULL;
                current->key[0] = leftNode->key[leftNode->size - 1];
                current->value[0] = leftNode->key[leftNode->size - 1];
                leftNode->size--;
                leftNode->ptr[leftNode->size] = current;
                leftNode->ptr[leftNode->size + 1] = NULL;
                parent->key[leftSib] = current->key[0];
                parent->value[leftSib] = current->value[0];
                return true;
            }
        }
//Redistribute to the right side
        if(rightSib <= parent->size){
            Node *rightNode = parent->ptr[rightSib];
            if(rightNode->size >= (keysMax + 1) / 2 + 1){
                current->size++;
                current->ptr[current->size] = current->ptr[current->size - 1];
                current->ptr[current->size - 1] = NULL;
                current->key[current->size - 1] = rightNode->key[0];
                current->value[current->size - 1] = rightNode->value[0];
                rightNode->size--;
                rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size + 1];
                rightNode->ptr[rightNode->size + 1 ] = NULL;
                for(int i = 0; i < rightNode->size; i++){
                    rightNode->key[i] = rightNode->key[i+1];
                    rightNode->value[i] = rightNode->value[i+1];
                }
                parent->key[rightSib - 1] = rightNode->key[0];
                parent->value[rightSib - 1] = rightNode->value[0];
                return true;
            }
        }
//coalesce to the left side
        if(leftSib >= 0){
            Node *leftNode = parent->ptr[leftSib];
            for (int i = leftNode->size, j = 0; j < current->size; i++,j++){
               leftNode->key[i] = current->key[j];
               leftNode->value[i] = current->value[j]; 
            }
            leftNode->ptr[leftNode->size] = NULL;
            leftNode->size += current->size;
            leftNode->ptr[leftNode->size] = current->ptr[current->size];
            removeInternal(parent->key[leftSib],parent->value[leftSib], parent, current);
            
        }
//coalesce to the right side
        if(rightSib <= parent->size){
            Node *rightNode = parent->ptr[rightSib];
            for(int i = current->size, j = 0; j < rightNode->size; i++, j++){
                current->key[i] = rightNode->key[j];
                current->value[i] = rightNode->value[j];
            }
            current->ptr[current->size] = NULL;
            current->size += rightNode->size;
            current->ptr[current->size] = rightNode->ptr[rightNode->size];
            removeInternal(parent->key[rightSib - 1],parent->value[rightSib - 1], parent, rightNode);
            
        }
        return true;
    }
}


void BPlusTree::removeInternal(int n, string x, Node *current, Node *child){
//removing necessary internal nodes if the parent is a root
    if(current == root && current->size == 1){
        if(current->ptr[1] == child){
            delete[] child->key;
            delete[] child->value;
            delete[] child->ptr;
            delete[] child;
            root = current->ptr[0];
            delete[] current->key;
            delete[] current->value;
            delete[] current->ptr;
            delete[] current;
            return;
        }else if(current->ptr[0] == child){
            delete[] child->key;
            delete[] child->value;
            delete[] child->ptr;
            delete[] child;
            root = current->ptr[1];
            delete[] current->key;
            delete[] current->value;
            delete[] current->ptr;
            delete[] current;
            return;
        }  
    }
    //removing necessary internal nodes if the parent is not a root
    int nDelete;
    for(nDelete = 0; nDelete < current->size; nDelete++){
        if(current->key[nDelete] == n){
            break;
        }
    }
    for(int i = nDelete; i< current->size; i++){
        current->key[i] = current->key[i+1];
        current->value[i] = current->value[i+1];
    }
    for(nDelete = 0; nDelete < current->size + 1; nDelete++){
        if(current->ptr[nDelete] == child){
            break;
        }
    }
    for(int i = nDelete; i < current->size + 1; i++){
        current->ptr[i]  = current->ptr[i+1];
    }
    current->size--;
    if(current->size >= (keysMax + 1) / 2 - 1){
        return;
    }
    if(current == root){
        return;
    }
    Node *parent = getParent(root, current);
    int leftSib, rightSib;
    for(nDelete = 0; nDelete < parent->size + 1; nDelete++){
        if(parent->ptr[nDelete] == current){
            leftSib = nDelete - 1;
            rightSib = nDelete + 1;
            break;
        }
    }
    //redistribute inner nodes to the left
    if(leftSib >= 0) {
        Node *leftNode = parent->ptr[leftSib];
        if (leftNode->size >= (keysMax + 1) / 2){
            for(int i = current->size; i > 0; i--){
                current->key[i] = current->key[i - 1];
                current->value[i] = current->value[i - 1];
            }
            current->key[0] = parent->key[leftSib];
            current->value[0] = parent->key[leftSib];
            parent->key[leftSib] = leftNode->key[leftNode->size - 1];
            parent->value[leftSib] = leftNode->value[leftNode-> size - 1];
            for(int i = current->size + 1; i > 0; i--){
                current->ptr[i] = current->ptr[i-1];
            }
            current->ptr[0] = leftNode->ptr[leftNode->size];
            current->size++;
            leftNode->size--;
            return;
        }
    }
    //redistribute inner nodes to the right
    if(rightSib <= parent->size){
        Node *rightNode = parent->ptr[rightSib];
        if(rightNode->size >= (keysMax + 1) / 2){
            current->key[current->size] = parent->key[nDelete];
            current->value[current->size] = parent->value[nDelete];
            parent->key[nDelete] = rightNode->key[0];
            parent->value[nDelete] = rightNode->value[0];
            for(int i = 0; i < rightNode->size - 1; i++){
                rightNode->key[i] = rightNode->key[i+1];
                rightNode->value[i] = rightNode->value[i+1];
            }
            current->ptr[current->size + 1] = rightNode->ptr[0];
            for (int i = 0; i < rightNode->size; i++){
                rightNode->ptr[i] = rightNode->ptr[i + 1];   
            }
            current->size++;
            rightNode->size--;
            return;
        }
    }
    //coalesce inner node to the left
    if(leftSib >= 0){
        Node *leftNode = parent->ptr[leftSib];
        leftNode->key[leftNode->size] = parent->key[leftSib];
        leftNode->value[leftNode->size] = parent->key[leftSib];
        for(int i = leftNode->size + 1, j = 0; j < current->size; j++){
            leftNode->key[i] = current->key[j];
            leftNode->value[i] = current->value[j];
        }
        for(int i = leftNode->size + 1, j = 0; j < current->size + 1; j++){
            leftNode->ptr[i] = current->ptr[j];
            current->ptr[j] = NULL;
        } 
        leftNode->size += current->size + 1;
        current->size = 0;
        removeInternal(parent->key[leftSib], parent->value[leftSib], parent, current);
    }
    //coalesce inner node to the right
    if(rightSib <= parent->size){
        Node *rightNode = parent->ptr[rightSib];
        current->key[current->size] = parent->key[rightSib - 1];
        current->value[current->size] = parent->value[rightSib - 1];
        for(int i = current->size + 1, j = 0; j < rightNode->size; j++){
            current->key[i] = rightNode->key[j];
            current->value[i] = rightNode->value[j];
        }
        for(int i = current->size + 1, j = 0; j < rightNode->size + 1; j++){
            current->ptr[i] = rightNode->ptr[j];
            rightNode->ptr[j] = NULL;
        }
        current->size += rightNode->size + 1;
        rightNode->size = 0;
        removeInternal(parent->key[rightSib - 1], parent->value[rightSib - 1], parent, rightNode);
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


string BPlusTree::find(int n){
//traverse down to find if the key exists. Return empty string if not found
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

