#include "BST.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <queue>

using namespace std;

// for random int generation between 0 and 1
BST::BST() : gen(rd()), distrib(0, 1) {}; // constructor including random number generator
BST::~BST() {
    // call a function to delete all nodes in the tree
    BST::deleteTree(root);
};  // destructor

void BST::build_bst(vector<int> list){
    sort(list.begin(), list.end()); // sort the list

    root = BST::sortedArrayToBST(list, 0, list.size() - 1);
}

// utility function to delete all nodes in a tree
void BST::deleteTree(BST::Node* node){
    if (node == nullptr) return;

    BST::deleteTree(node->left);
    BST::deleteTree(node->right);

    delete(node);
}

// utility function to create a new node
BST::Node* BST::newNode(int value) {
    Node* node = new Node;
    node->val = value;
    node->left = node->right = nullptr;

    return node;
}

// function to create a BST from a sorted array
BST::Node* BST::sortedArrayToBST(vector<int>& arr, int start, int end) {
    if (start > end) {
        return nullptr;
    }

    // get the middle element and make it root
    int mid = (start + end) / 2;
    Node* node = newNode(arr[mid]);

    // recursively construct the left and right subtrees
    node->left = sortedArrayToBST(arr, start, mid - 1);
    node->right = sortedArrayToBST(arr, mid + 1, end);

    this -> root = node; // set root of the tree
    return node;
}

void BST::print_bst() {
    // check if tree is empty
    if(root == nullptr) {
        cout << "Tree is empty.\n";
        return;
    }

    // create an empty queue for level order traversal
    queue<Node *> q;

    // enqueue root and initialize height
    q.push(root);

    while(q.empty() == false) {
        // nodeCount (queue size) indicates number of nodes at current level.
        int nodeCount = q.size();

        // dequeue all nodes of current level and enqueue all nodes of next level
        while(nodeCount > 0) {
            Node *node = q.front();
            cout << node->val << " ";

            q.pop();

            if(node->left != nullptr)
                q.push(node->left);
            if(node->right != nullptr)
                q.push(node->right);

            nodeCount--;
        }

        // print newline to move to the next level
        cout << endl;
    }
}

BST::Node* BST::find_element(BST::Node* root, int val){
    if(root == nullptr) {
        // Element is not found in the tree
        return nullptr;
    } else if(root -> val == val) {
        // We have found the element
        return root;
    } else if(root -> val < val) {
        return find_element(root -> right, val);
    } else {
        return find_element(root -> left, val);
    }
}
