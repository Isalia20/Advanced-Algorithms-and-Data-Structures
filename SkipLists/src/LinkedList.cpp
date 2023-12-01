#include <iostream>
#include <cstdlib>
#include "LinkedList.h"

using namespace std;

LinkedList::LinkedList() : head(NULL) {}; // constructor
LinkedList::~LinkedList() { // destructor
    Node* current = head;
    while (current != NULL) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head = NULL;
}

void LinkedList::build_ll(vector<int>& list){
        int i = list.size() - 1;
        LinkedList::Node* p_next = nullptr;
        LinkedList::Node* p;

        while (i >= 0){
            // initialize a node
            p = new LinkedList::Node;
            p -> val = list[i];
            // if the last node isn't set then set it.
            if (last == NULL){
                last = p;
            }
            p -> next = p_next;
            p_next = p;
            i--;
        }
        head = p;
    };

void LinkedList::print_ll(){
    LinkedList::Node* head_copy = head;
    while (head_copy != NULL){
        cout << head_copy -> val << " ";
        head_copy = head_copy -> next;
    }
    cout << endl;
};

void LinkedList::push_back(int val){
    // create a new node
    LinkedList::Node* new_node = new LinkedList::Node;
    new_node -> val = val;
    // If we don't have a linked list at all
    if (head == NULL){
        head = new_node;
    }
    // attach it to the end of the list
    last -> next = new_node;
    last = new_node;
}

void LinkedList::push_front(int val){
    LinkedList::Node* new_node = new LinkedList::Node;
    new_node -> val = val;
    
    if (head == NULL){
        head = new_node;
    } else {
        Node* head_old = head;
        head = new_node;
        head -> next = head_old;
    }
}

void LinkedList::pop_back(){
    // copy of the head so we dont change the head during traversal
    LinkedList::Node* head_copy = head;
    // traverse to the end of the linked list
    while (head_copy -> next != last){
        head_copy = head_copy -> next;
    }
    LinkedList::Node* last_copy = last;
    last = head_copy;
    last -> next = NULL;
    // free up memory
    delete(last_copy);
}

void LinkedList::pop_front(){
    LinkedList::Node* head_copy = head;
    head = head -> next;
    // free memory
    delete(head_copy);
}

int LinkedList::front(){
    return head -> val;
}

int LinkedList::back(){
    return last -> val;
}

bool LinkedList::empty(){
    return (head == NULL);
}

int LinkedList::get_size(){
    LinkedList::Node* head_copy = head;
    int counter = 0;
    while (head_copy != NULL){
        head_copy = head_copy -> next;
        counter++;
    }
    return counter;
}

LinkedList::Node* LinkedList::find_element(int val){
    LinkedList::Node* head_copy = head;
    while (head_copy != NULL){
        if (head_copy -> val == val){
            return head_copy;
        }
        head_copy = head_copy -> next;
    }
    throw std::runtime_error("Element you want to find doesn't exist");
};
