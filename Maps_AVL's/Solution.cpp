#include "WordFreq.h"   //given header
#include <vector>
#include <unordered_map>    //needed headers
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;

vector<WordFreq> GetMostFrequentKWords(string filename, int k) {
    vector<WordFreq> result;
    unordered_map<string, int> w_Count;

    struct Node {    //node structure for presenting AVL tree
        int height;
        string word;
        Node* left;
        Node* right;

        Node(string w) : word(w), left(nullptr), right(nullptr), height(1) {}
    };

  
    auto height_avl = [&](Node* node) -> int {  //determination part of height of node
        if (!node)
            return 0;
        return node->height;
        };

   
    auto balance_cal = [&](Node* node) -> int { //determination part of balance calculation of a node
        if (!node)
            return 0;
        return height_avl(node->left) - height_avl(node->right);
        };

    
    auto right_rotation = [&](Node* t) -> Node* {   //process of turning to right
        Node* a = t->left;
        Node* t_2 = a->right;

        a->right = t;
        t->left = t_2;

        t->height = max(height_avl(t->left), height_avl(t->right)) + 1;
        a->height = max(height_avl(a->left), height_avl(a->right)) + 1;

        return a;
        };

  
    auto left_rotation = [&](Node* a) -> Node* {    //process of turning to left
        Node* t = a->right;
        Node* t_2 = t->left;

        t->left = a;
        a->right = t_2;

        a->height = max(height_avl(a->left), height_avl(a->right)) + 1;
        t->height = max(height_avl(t->left), height_avl(t->right)) + 1;

        return t;
        };

    
    auto balance = [&](Node* node) -> Node* {   //balance process that is main idea of AVL's
        int balance_fact = balance_cal(node);

        if (balance_fact > 1) {
            if (balance_cal(node->left) >= 0) {
                return right_rotation(node);
            }
            else {
                node->left = left_rotation(node->left);
                return right_rotation(node);
            }
        }

        if (balance_fact < -1) {
            if (balance_cal(node->right) <= 0) {
                return left_rotation(node);
            }
            else {
                node->right = right_rotation(node->right);
                return left_rotation(node);
            }
        }

        return node;
        };

   
    auto insert = [&](Node* node, const string& w) -> Node* {   //addition new word operation to AVL tree
        if (!node)
            return new Node(w);

        if (w < node->word) {
            node->left = insert(node->left, w);
        }
        else if (w > node->word) {
            node->right = insert(node->right, w);
        }
        else {
            return node;  //in equal position,return same node
        }

        node->height = 1 + max(height_avl(node->left), height_avl(node->right));

        return balance(node);
        };

    
    auto inorder_traverse = [&](Node* node, vector<WordFreq>& result, const unordered_map<string, int>& w_Count) {  //an arbitrary journey
        if (node != nullptr) {
            inorder_traverse(node->left, result, w_Count);
            result.emplace_back(node->word, w_Count.at(node->word));  // addition safely
            inorder_traverse(node->right, result, w_Count);
        }
        };

   
    ifstream inputFile(filename);   //procedure of file-opening
    if (!inputFile.is_open()) {
        cout << "*WARNING* IT'S AN ERROR MESSAGE: " << filename << endl;
        return result;
    }

    string word;
    while (inputFile >> word) {
        string cleaning = "";    //determine and clean the non-alphabetical character
        for (char& c : word) {
            if (isalnum(c)) {
                cleaning += tolower(c);
            }
        }
        if (!cleaning.empty()) {
            ++w_Count[cleaning];    //count the alphanumerics
        }
    }
    inputFile.close();

    
    Node* root = nullptr;   //creation of avl tree
    for (const auto& wc : w_Count) {
        root = insert(root, wc.first);
    }

    inorder_traverse(root, result, w_Count);    //transfer the freqs to vectors

    sort(result.begin(), result.end(), [](const WordFreq& i, const WordFreq& j) {   //sorting by frequency
        return j.freq < i.freq;
        });

    
    if (result.size() > k) {    //get most freq. k returning
        result.resize(k);
    }

    return result;
}



