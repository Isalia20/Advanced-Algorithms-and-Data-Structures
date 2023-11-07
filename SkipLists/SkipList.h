#ifndef SKIP_LIST_H
#define SKIP_LIST_H
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <unordered_set>

using namespace std;


class SkipList {
public:
    struct Node {
        int val;
        int height;
        Node* next;
        Node* prev;
        Node* up;
        Node* down;
    };

    int MAX_LVL; // max level of the skip list
    Node* head;
    Node* tail;
    vector<vector<Node*>> ALL_NODES;

    SkipList(); // default constructor
    ~SkipList(); // destructor

    void build_up_rand(Node* node, vector<vector<Node*>> &last_nodes);
    void build_up(Node* node, int level, vector<vector<Node*>> &last_nodes);
    void build_skip_list_random(vector<int> list);
    void build_skip_list_deterministic(vector<int> list);
    Node* find_element_rec(int value, Node* head_copy);
    vector<Node*> find_closest_element_rec(int value, Node* head_copy);
    Node* get_bottom_node(Node* node);
    Node* find_element(int value);
    vector<Node*> find_closest_elements(int value);
    Node* go_up_n_times(Node* node, int n);
    void insert_element_min_max(int value, bool is_max, Node* node);
    Node* create_node_up_if_not_exist(Node* node);
    void remove_nodes_without_applying_connections(Node* node);
    void remove_element_min_max(Node* node_to_remove, bool is_max);
    void remove_element(int value);
    void insert_element(int value);
    void print();
};

#endif
