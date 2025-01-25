#include <iostream> //needed headers
#include "ThreadedBST.h"

void ThreadedBST::eraseTreeNodes(BSTNode* node) {
    if (node == nullptr) return;

    if (node->leftLinkType == CHILD) {
        eraseTreeNodes(node->left);
    }
    if (node->rightLinkType == CHILD) {
        eraseTreeNodes(node->right);
    }

    delete node;
}

void ThreadedBST::add(int key) {    //adding operation
    if (root == nullptr) {
        root = new BSTNode(key);
        return;
    }

    BSTNode* NOW = root;
    BSTNode* parental = nullptr;
    while (NOW != nullptr) {
        parental = NOW;
        if (key < NOW->key) {
            if (NOW->leftLinkType == CHILD) {   //if the left part is real
                NOW = NOW->left;
            }
            else {
                break;
            }
        }
        else {
            if (NOW->rightLinkType == CHILD) {  //if the right part is real
                NOW = NOW->right;
            }
            else {
                break;
            }
        }
    }

    BSTNode* newest = new BSTNode(key);
    if (key < parental->key) {  //add the child right or left
        newest->left = parental->left;
        newest->right = parental;
        parental->left = newest;
        parental->leftLinkType = CHILD;
    }
    else {
        newest->right = parental->right;
        newest->left = parental;
        parental->right = newest;
        parental->rightLinkType = CHILD;
    }
}

void ThreadedBST::remove(int key) { //remove operation
    BSTNode* NOW = root;
    BSTNode* parent_2 = nullptr;
    bool leftChild = false;

    while (NOW != nullptr && NOW->key != key) {
        parent_2 = NOW;
        if (key < NOW->key) {
            if (NOW->leftLinkType == CHILD) {
                NOW = NOW->left;
                leftChild = true;
            }
            else {
                return; //cannot found
            }
        }
        else {
            if (NOW->rightLinkType == CHILD) {
                NOW = NOW->right;
                leftChild = false;
            }
            else {
                return; //cannot found
            }
        }
    }

    if (NOW == nullptr) 
        return; //cannot found

    if (NOW->leftLinkType == THREAD && NOW->rightLinkType == THREAD) {  //node that's don't have child
        if (NOW == root) {
            root = nullptr;
        }
        else if (leftChild) {
            parent_2->left = NOW->left;
            parent_2->leftLinkType = THREAD;
        }
        else {
            parent_2->right = NOW->right;
            parent_2->rightLinkType = THREAD;
        }
    }
    else if (NOW->leftLinkType == CHILD && NOW->rightLinkType == THREAD) {  //only have left child
        if (NOW == root) {
            root = NOW->left;
        }
        else if (leftChild) {
            parent_2->left = NOW->left;
        }
        else {
            parent_2->right = NOW->left;
        }
    }
    else if (NOW->rightLinkType == CHILD && NOW->leftLinkType == THREAD) {  //only have right child
        if (NOW == root) {
            root = NOW->right;
        }
        else if (leftChild) {
            parent_2->left = NOW->right;
        }
        else {
            parent_2->right = NOW->right;
        }
    }
    else {
        BSTNode* succ = NOW->right; //having to directional (left and right) child
        BSTNode* succ_Parent = NOW;

        while (succ->leftLinkType == CHILD) {
            succ_Parent = succ;
            succ = succ->left;
        }

        NOW->key = succ->key;   //changing the keys
        if (succ->rightLinkType == THREAD) {
            if (succ_Parent->left == succ) {
                succ_Parent->left = succ->left;
                succ_Parent->leftLinkType = THREAD;
            }
            else {
                succ_Parent->right = succ->right;
                succ_Parent->rightLinkType = THREAD;
            }
        }
        else {
            if (succ_Parent->left == succ) {
                succ_Parent->left = succ->right;
            }
            else {
                succ_Parent->right = succ->right;
            }
        }
    }

    delete NOW;
}

BSTNode* ThreadedBST::find(int key) {   //find operations
    BSTNode* NOW = root;
    while (NOW != nullptr) {
        if (key == NOW->key) {
            return NOW;
        }
        else if (key < NOW->key) {
            if (NOW->leftLinkType == CHILD) {
                NOW = NOW->left;
            }
            else {
                return nullptr;
            }
        }
        else {
            if (NOW->rightLinkType == CHILD) {
                NOW = NOW->right;
            }
            else {
                return nullptr;
            }
        }
    }
    return nullptr;
}

BSTNode* ThreadedBST::min() {   //detecting operation for minimum
    BSTNode* NOW = root;
    while (NOW != nullptr && NOW->leftLinkType == CHILD) {
        NOW = NOW->left;
    }
    return NOW;
}

BSTNode* ThreadedBST::max() {   //detecting operation for maximum
    BSTNode* NOW = root;
    while (NOW != nullptr && NOW->rightLinkType == CHILD) {
        NOW = NOW->right;
    }
    return NOW;
}

BSTNode* ThreadedBST::previous(BSTNode* node) { //detecting operatiom for previous part
    if (node == nullptr) 
        return nullptr;

    if (node->leftLinkType == THREAD) {
        return node->left;
    }

    BSTNode* pred = node->left;
    while (pred != nullptr && pred->rightLinkType == CHILD) {
        pred = pred->right;
    }
    return pred;
}

BSTNode* ThreadedBST::next(BSTNode* node) { //detecting operatiom for next part
    if (node == nullptr)
        return nullptr;

    if (node->rightLinkType == THREAD) {
        return node->right;
    }

    BSTNode* succ_2 = node->right;
    while (succ_2 != nullptr && succ_2->leftLinkType == CHILD) {
        succ_2 = succ_2->left;
    }

    return succ_2;
}