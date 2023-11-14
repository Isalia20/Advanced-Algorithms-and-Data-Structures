#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <iostream>
#include <cstdlib>

using namespace std;


class LinkedList {
public:
    // Node structure
    struct Node {
        int val;
        Node* next;
    };

    LinkedList(); // constructor
    ~LinkedList(); // destructor
    
    Node* head; // head pointer of the linked list
    Node* last; // last node of the linked list
    
    void build_ll(vector<int> list);
    void print_ll();
    void push_back(int val);
    void push_front(int val);
    void pop_back();
    void pop_front();
    int front();
    int back();
    bool empty();
    int get_size();
    Node* find_element(int val);
};

#endif