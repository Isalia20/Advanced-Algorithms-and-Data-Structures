#ifndef SKIP_LIST_H
#define SKIP_LIST_H
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <unordered_set>
#include <random>


class SkipList {
private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> distrib;

public:
    struct Node {
        int val;
        int height;
        Node* next;
        Node* prev;
        Node* up;
        Node* down;
    };

    int MAX_LVL; // max level of the skip list.
    Node* head;
    Node* tail;
    int ELEMENT_COUNT;

    SkipList(); // constructor
    ~SkipList(); // destructor

    int rand_bool(); // Generate random number 0/1
    void build_up_rand(Node* node, std::vector<std::vector<Node*>> &last_nodes, bool &has_tower); // Once we have some node build up randomly
    void build_up(Node* node, int level, std::vector<std::vector<Node*>> &last_nodes); // Once we have some node build up while indicating the level to build up to
    void build_skip_list_random(std::vector<int>& list); // Function for building skip list from a vector randomly(meaning each node height is determined randomly)
    void build_skip_list_deterministic(std::vector<int>& list); // Function for building skip list from a vector deterministically(meaning each node height is determined by its index)
    Node* find_element_rec(int searchValue, Node* traversalNode); // Function for finding element recursively
    Node* find_element(int searchValue); // Function for finding the element in skip list(returns the bottom most node of that element)
    std::vector<Node*> find_closest_element_rec(int searchValue, Node* traversalNode); // Function for finding closest elements recursively(left and right)
    std::vector<Node*> find_closest_elements(int searchValue); // Function for finding the closest elements(left and right)
    Node* get_bottom_node(Node* node); // Function for getting the bottom most node from any node(go down till the end is reached)
    Node* go_up_n_times(Node* traversalNode, int steps);
    void insert_element_max(int value, Node* node, int maxLevelNew);
    void insert_element_min(int value, Node* node, int maxLevelNew);
    Node* create_node_up_if_not_exist(Node* node);
    void remove_nodes_without_applying_connections(Node* node);
    void remove_element_min_max(Node* node_to_remove, bool is_max);
    void remove_element(int value);
    void insert_element(int value);
    Node* go_up_till_end(Node* traversalNode);
    std::vector<int> get_level_node_count();
    void print();
};

#endif
