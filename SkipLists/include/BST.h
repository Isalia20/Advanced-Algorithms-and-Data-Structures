// Minor implementation of BST for comparison with SkipLists
#ifndef BST_H
#define BST_H
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <unordered_set>
#include <random>

using namespace std;


class BST {
private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> distrib;

public:
    struct Node {
        int val;
        Node* left;
        Node* right;
    };

    Node* root;

    BST(); // constructor
    ~BST(); // destructor

    void build_bst(vector<int> list);
    BST::Node* sortedArrayToBST(vector<int>& arr, int start, int end);
    BST::Node* newNode(int value);
    BST::Node* find_element(BST::Node* root, int val);
    BST::Node* find_element_ops(BST::Node* root, int val, std::ofstream& file);
    void deleteTree(BST::Node* node);
    void print_bst();
    void printRange(BST::Node* root, int X, int Y);
};

#endif
