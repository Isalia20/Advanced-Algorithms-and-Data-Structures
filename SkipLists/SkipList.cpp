#include "SkipList.h"
#include <iostream>
#include <vector>
#include <random>

using namespace std;

// for random int generation 0 and 1
int SkipList::next(){
    return distrib(gen);
}

SkipList::SkipList() : gen(rd()), distrib(0, 1) {}; // constructor including random number generator
SkipList::~SkipList() { 
    SkipList::Node* curNode = head;
    std::unordered_set<SkipList::Node*> nodesToDelete; // stores unique nodes

    // Iterate over the SkipList to collect all unique nodes
    while(curNode) {
        SkipList::Node* nextNode = curNode->next;
        SkipList::Node* upNode = curNode->up;

        while(curNode) {
            SkipList::Node* downNode = curNode->down;
            nodesToDelete.insert(curNode);
            curNode = downNode;
        }

        if(upNode) {
            curNode = upNode;
        }
        else {
            curNode = nextNode;
        }
    }

    // Delete all unique nodes
    for(auto node : nodesToDelete) {
        delete node;
    }
};

void SkipList::build_up_rand(SkipList::Node* node, std::vector<std::vector<SkipList::Node*>> &last_nodes, bool &has_tower){
    SkipList::Node* node_traverse = node;
    std::vector<SkipList::Node*> upper_nodes;
    SkipList::Node* node_prev; // used for connecting nodes horizontally
    int counter = 0;

    while (SkipList::next() && counter < this -> MAX_LVL){
        counter++;
        SkipList::Node* node_up = new SkipList::Node;
        node_up -> val = node_traverse -> val;
        node_up -> down = node_traverse;
        node_traverse -> up = node_up;
        node_traverse = node_up;
        // if we have nodes before in the upper section
        if (last_nodes.size() > 0){
            int i = last_nodes.size() - 1;
            // Before we find the index where level matches the current one, keep iterating backwards
            while (last_nodes[i].size() < counter && i >= 0){ 
                i--;
            }
            if (i >= 0 && last_nodes[i].size() >= counter){
                // Once we have found it, we just need to get it and set the pointers to each other
                node_prev = last_nodes[i][counter - 1];
                node_prev -> next = node_traverse;
                node_traverse -> prev = node_prev;
            }
        }
        upper_nodes.push_back(node_traverse);
        has_tower = 1;
    }
    last_nodes.push_back(upper_nodes);
}

void SkipList::build_up(SkipList::Node* node, int level, std::vector<std::vector<SkipList::Node*>> &last_nodes){
    SkipList::Node* node_traverse = node;
    std::vector<SkipList::Node*> upper_nodes;
    SkipList::Node* node_prev; // used for connecting nodes horizontally
    int counter = 0;

    while (level > 0){
        counter++;
        SkipList::Node* node_up = new SkipList::Node;
        node_up -> val = node_traverse -> val;
        node_up -> down = node_traverse;
        node_traverse -> up = node_up;
        node_traverse = node_up;
        // if we have nodes before in the upper section
        if (last_nodes.size() > 0){
            int i = last_nodes.size() - 1;
            // Before we find the index where level matches the current one, keep iterating backwards
            while (last_nodes[i].size() < counter && i >= 0){ 
                i--;
            }
            if (i >= 0 && last_nodes[i].size() >= counter){
                // Once we have found it, we just need to get it and set the pointers to each other
                node_prev = last_nodes[i][counter - 1];
                node_prev -> next = node_traverse;
                node_traverse -> prev = node_prev;
            }
        }
        upper_nodes.push_back(node_traverse);
        level--;
    }
    last_nodes.push_back(upper_nodes);
}

void SkipList::build_skip_list_random(std::vector<int>& list){
    sort(list.begin(), list.end());
    int i = 0;
    int max_level = std::log2(list.size());
    this -> MAX_LVL = max_level;
    this -> ELEMENT_COUNT = list.size();

    // Start building the linked list
    SkipList::Node* p;
    SkipList::Node* p_prev = nullptr;
    std::vector<std::vector<SkipList::Node*>> last_nodes;

    while (i < list.size()){
        p = new SkipList::Node;
        p -> val = list[i];
        bool has_tower = 0;
        // Build node up to the max level it can go
        // based on randomness if not the start or end level
        if (i == 0 || i == list.size() - 1){
            SkipList::build_up(p, this -> MAX_LVL, last_nodes);
            has_tower = 1;
        } else {
            // Randomly grow if we are in the middle(not at the start/end)
            SkipList::build_up_rand(p, last_nodes, has_tower);
        }
        p -> prev = p_prev;
        // make it doubly linked list
        if (p_prev != NULL){
            p_prev -> next = p;
        }
        p_prev = p;
        // Set the head of the skip list 
        if (i == 0) this -> head = p;
        // Set the end of the skip list 
        if (i == list.size() - 1) this -> tail = p;
        if (has_tower){
            p -> height = last_nodes[last_nodes.size() - 1].size(); // get the height of the tower(i.e. last tower inserted)
        } else {
            p -> height = 0;
        }
        i++;
    }
    // this -> ALL_NODES = last_nodes;
}

void SkipList::build_skip_list_deterministic(std::vector<int>& list){
    sort(list.begin(), list.end());
    int i = 0;
    int max_level = std::log2(list.size());
    this -> MAX_LVL = max_level;
    this -> ELEMENT_COUNT = list.size();
    std::vector<int> levels;

    // for example for max level 2 it will create std::vector of 
    // {4, 2} which will later be used for build up
    // {2, 1} which will be later used for determining how  
    // high we should go
    for (int i = max_level; i > 0; i--){
        levels.push_back(pow(2, i));
    }

    // Start building the linked list
    SkipList::Node* p;
    SkipList::Node* p_prev = nullptr;
    std::vector<std::vector<SkipList::Node*>> last_nodes;

    while (i < list.size()){
        p = new SkipList::Node;
        p -> val = list[i];
        bool has_tower = 0;
        // Build node up to the max level it can go
        // based on the exponents of 2
        // 2, 4, 8, 16...
        for (int j = 0; j < levels.size(); j++){
            if (i % levels[j] == 0){
                SkipList::build_up(p, std::log2(levels[j]), last_nodes);
                has_tower = 1;
                break;
                }
            if (i == list.size() - 1){
                SkipList::build_up(p, std::log2(levels[0]), last_nodes); // build the tail to the maximum
                has_tower = 1;
                break;
                }
            }
        p -> prev = p_prev;
        // make it doubly linked list
        if (p_prev != NULL){
            p_prev -> next = p;
        }
        p_prev = p;
        // Set the head of the skip list
        if (i == 0){
            this -> head = p;
        }
        // Set the end of the skip list 
        if (i == list.size() - 1){
            this -> tail = p;
        }

        if (has_tower){
            p -> height = last_nodes[last_nodes.size() - 1].size(); // get the height of the tower(i.e. last tower inserted)
        } else {
            p -> height = 0; // tower has a height of 1
        }
        i++;
    }
    // this -> ALL_NODES = last_nodes; // Save it for later use in inserting
};

SkipList::Node* SkipList::find_element_rec(int value, SkipList::Node* head_copy){
    while (head_copy -> next != NULL && head_copy -> next -> val <= value){
        head_copy = head_copy -> next;
        if (head_copy -> val == value){
            return head_copy;
        }
    }
    // If we traversed to the end of the list and didn't find the element
    if (head_copy -> down == NULL){
        return nullptr;
    }
    SkipList::Node* des_node = find_element_rec(value, head_copy -> down);
    return des_node;
}

std::vector<SkipList::Node*> SkipList::find_closest_element_rec(int value, SkipList::Node* head_copy){
    while (head_copy -> next != NULL && head_copy -> next -> val <= value){
        head_copy = head_copy -> next;
        if (head_copy -> val == value){
            return {head_copy}; // if already exists return the node itself and will be dealt in the function find_closest_elements
        }
    }
    // If we traversed to the end of the list and didn't find the element
    if (head_copy -> down == NULL){
        return {head_copy, head_copy -> next};
    }
    std::vector<SkipList::Node*> closest_elements  = find_closest_element_rec(value, head_copy -> down);
    return closest_elements;
}

SkipList::Node* SkipList::get_bottom_node(SkipList::Node* node){
    /* 
    This function is to get the most bottom node 
    in the skip list. Used in find_element function 
    so the return of the function is consistent
    (i.e. doesn't return middle/top/bottom randomly)
    */
    if (node == NULL){ // if we already get a nullptr
        return node;
    } else {
        while (node -> down != NULL){
            node = node -> down;
        }
    }
    return node;
}

SkipList::Node* SkipList::find_element(int value){
    /* 
    Returns a node in the skip list 
    containing the desired value
    */
    
    SkipList::Node* head_copy = this -> head;
    // If it's already on the value we are searching
    if (head_copy -> val == value){
        return head_copy;
    }

    // go to the top of the skip list(upper left corner)
    while (head_copy -> up != NULL){
        head_copy = head_copy -> up;
    }
    
    SkipList::Node* node = SkipList::find_element_rec(value, head_copy);
    if (node == NULL){
        return node;
    } else {
        return get_bottom_node(node);
    }
}

std::vector<SkipList::Node*> SkipList::find_closest_elements(int value){
    /* 
    Returns a node in the skip list 
    containing the desired value
    */
    
    SkipList::Node* head_copy = this -> head;
    
    // If it's already on the value we are searching
    if (head_copy -> val == value){
        return {head_copy -> prev, head_copy -> next};
    }
    // go to the top of the skip list(upper left corner)
    while (head_copy -> up != NULL){
        head_copy = head_copy -> up;
    }
    std::vector<SkipList::Node*> nodes = SkipList::find_closest_element_rec(value, head_copy);
    
    if (nodes.size() == 1){
        nodes[0] = get_bottom_node(nodes[0]);
        SkipList::Node* left_node = nullptr;
        SkipList::Node* right_node = nullptr;

        // left pointer
        if (nodes[0] -> prev != NULL){
            left_node = nodes[0] -> prev;
        }
        // right pointer
        if (nodes[0] -> next != NULL){
            right_node = nodes[0] -> next;
        }
        return {left_node, right_node};
    } else if (nodes[1] == NULL){ // if we have maximum case
        nodes[0] = get_bottom_node(nodes[0]);
        nodes[1] = nodes[0];
        nodes[0] = nullptr;
        return nodes;
    } else {
        nodes[0] = get_bottom_node(nodes[0]);
        nodes[1] = get_bottom_node(nodes[1]);
        if (nodes[0] && nodes[0] -> val > value){ // if we put in a value less than the minimum
            nodes[1] = nullptr;
        }
        return nodes;
    }
}

SkipList::Node* SkipList::go_up_till_end(SkipList::Node* node){
    SkipList::Node* node_copy = node;
    while (node_copy -> up != NULL){
        node_copy = node_copy -> up;
    }
    return node_copy;
}

SkipList::Node* SkipList::go_up_n_times(SkipList::Node* node, int n){
    while (n > 0){
        if (node -> up != NULL){
            node = node -> up;
        }
        n--;
    }
    return node;
}

void SkipList::insert_element_min(int value, SkipList::Node* node, int max_lvl_new){

    SkipList::Node* start = new SkipList::Node; // maintain pointer to the start of the new ll
    SkipList::Node* traverse_node = start; // for traversing and adding nodes to the new ll
    SkipList::Node* node_copy = node; // copy of the head or tail of skip list
    start -> val = value; // assign value to the node
    int height = 0;
    int insert_height = 0; // TODO MIGHT NOT NEED THIS VARIABLE AND CAN JUST USE HEIGHT

    start -> next = node_copy; // if minimum(inserted on the left)
    node_copy -> prev = start; // doubly linked
    this -> head = start;
    SkipList::Node* insert_node_up;

    while (height < max_lvl_new){ // for start of the list we have to grow it to the top due to the reason that searching relies on the first element having the highest level
        insert_height++;
        insert_node_up = new SkipList::Node;

        // two way connection
        insert_node_up -> down = traverse_node;
        traverse_node -> up = insert_node_up;
        traverse_node = insert_node_up;

        // search for the node with appropriate height
        while (node_copy -> next != NULL && node_copy -> height < insert_height){
            node_copy = node_copy -> next;
        }

        if (node_copy -> height >= insert_height){
            SkipList::Node* node_for_connecting = node_copy;
            node_for_connecting = go_up_n_times(node_for_connecting, insert_height);
            traverse_node -> next = node_for_connecting;
            node_for_connecting -> prev = traverse_node;
        }
        //  else {
        //     traverse_node -> next = NULL;
        // }
        insert_node_up -> val = value;
        height++;
    }
    // SkipList::Node* head_end = go_up_till_end(this -> head);
    // insert_node_up -> next = head_end;
    // head_end -> prev = insert_node_up;

    start -> height = height; // assign height to the bottom most node TODO 
    if (max_lvl_new > this -> MAX_LVL){
        SkipList::Node* tail_end = go_up_till_end(this -> tail);
        SkipList::Node* tail_end_up = new SkipList::Node;
        tail_end_up -> val = tail_end -> val;
        tail_end_up -> down = tail_end;
        tail_end -> up = tail_end_up;

        insert_node_up -> next = tail_end_up;
        tail_end_up -> prev = insert_node_up;
    }
    this -> MAX_LVL = max_lvl_new;
}

void SkipList::insert_element_max(int value, SkipList::Node* node, int max_lvl_new){
    SkipList::Node* start = new SkipList::Node; // maintain pointer to the start of the new ll
    SkipList::Node* traverse_node = start; // for traversing and adding nodes to the new ll
    SkipList::Node* node_copy = node; // copy of the head or tail of skip list
    start -> val = value; // assign value to the node
    int height = 0;
    int insert_height = 0; // TODO MIGHT NOT NEED THIS VARIABLE AND CAN JUST USE HEIGHT
    
    start -> prev = node_copy; // if maximum(inserted on the right)
    node_copy -> next = start; // doubly linked
    this -> tail = start;
    SkipList::Node* insert_node_up;

    while (height < max_lvl_new){
        insert_height++;
        insert_node_up = new SkipList::Node;
        // two way connection
        insert_node_up -> down = traverse_node;
        traverse_node -> up = insert_node_up;
        traverse_node = insert_node_up;
        
        // search for the node with appropriate height
        while (node_copy -> prev != NULL && node_copy -> height < insert_height){ // search to the left before we reach the start or find the appropriate node to insert the next level element in(for it to be then connected)
            node_copy = node_copy -> prev;
        }
        if (node_copy -> height >= insert_height){
            SkipList::Node* node_for_connecting = node_copy;
            node_for_connecting = go_up_n_times(node_for_connecting, insert_height);
            traverse_node -> prev = node_for_connecting;
            node_for_connecting -> next = traverse_node;
        }
        insert_node_up -> val = value;
        height++;
    }
    start -> height = height; // assign height to the bottom most node
    if (max_lvl_new > this -> MAX_LVL){
        SkipList::Node* head_end = go_up_till_end(this -> head);
        SkipList::Node* head_end_up = new SkipList::Node;
        head_end_up -> val = head_end -> val;
        head_end_up -> down = head_end;
        head_end -> up = head_end_up;

        insert_node_up -> prev = head_end_up;
        head_end_up -> next = insert_node_up;
    }
    this -> MAX_LVL = max_lvl_new;
}


SkipList::Node* SkipList::create_node_up_if_not_exist(SkipList::Node* node){
    if (node -> up == NULL){
        // Create new node which then will be connected with up pointer
        SkipList::Node* node_up = new SkipList::Node;
        node_up -> val = node -> val;
        // Connect it two ways
        node_up -> down = node;
        node -> up = node_up;
        node = node_up;
    } else {
        node = node -> up;
    }
    return node;
}

void SkipList::remove_nodes_without_applying_connections(SkipList::Node* node){
    /* This function is used when we have only 2 elements left 
    in skip list and someone deletes the element, in this case 
    to not get a cycled linked list we only delete the nodes and that's all
    (i.e. pointers from the node that remains become nullptrs)
    */
    SkipList::Node* node_copy = node;
    while (node_copy != NULL){
        node_copy = node_copy -> up;
        if (node -> prev != NULL){
            node -> prev -> next = nullptr;
        }
        if (node -> next != NULL){
            node -> next -> prev = nullptr;
        }
        delete (node);
        node = node_copy;
    }
}

void SkipList::remove_element_min_max(SkipList::Node* node_to_remove, bool is_max){
    /* removing maximum or minimum element 
    is dealt through this function */
    if (is_max){
        // connect level 0
        SkipList::Node* node_to_remove_up = node_to_remove -> up;
        SkipList::Node* node_prev = node_to_remove -> prev;            
        
        if (node_prev -> prev == NULL){ // Meaning if node_prev will be the only element left in skip list after removing this one
            this -> tail = node_prev;
            remove_nodes_without_applying_connections(node_to_remove); // then remove without rewiring the pointers
            return;
        }

        node_to_remove -> prev = nullptr; // remove previous pointer of the node we are deleting
        node_prev -> next = nullptr; // level 0 cancel previous node's pointer for the next node(i.e. node_to_remove)
        this -> tail = node_prev; // save the tail(end of the skip list)

        delete(node_to_remove); // remove the level 0 node

        SkipList::Node* node_to_remove_copy_traverse;
        // Create up node if it doesnt exist on the next node(initial head -> next)
        if (node_to_remove_up != NULL){
            node_prev = create_node_up_if_not_exist(node_prev);
             node_to_remove_copy_traverse = node_to_remove_up; // copy for traversal
        }
    
        while (node_to_remove_up != NULL){
            if (node_prev != node_to_remove_copy_traverse -> prev){ // to avoid cyclical pointers
                node_prev -> prev = node_to_remove_copy_traverse -> prev;
            }
            if (node_prev -> prev != NULL){ // connect the previous node both ways
                node_prev -> prev -> next = node_prev;
            }
            node_to_remove_copy_traverse = node_to_remove_copy_traverse -> up; // go up for the original node
            
            // For the next iteration
            if (node_to_remove_copy_traverse != NULL){
                node_prev = create_node_up_if_not_exist(node_prev);
            }
            delete(node_to_remove_up); // delete the node we have just rewired pointers for 
            node_to_remove_up = node_to_remove_copy_traverse; // save it so we can delete further nodes as well
        }
    } else {
        // connect level 0
        SkipList::Node* node_to_remove_up = node_to_remove -> up;
        SkipList::Node* node_next = node_to_remove -> next;

        if (node_next -> next == NULL){
            this -> head = node_next;
            remove_nodes_without_applying_connections(node_to_remove);
            return;
        }

        node_next -> prev = nullptr; // level 0 cancel previous pointer for the next node
        this -> head = node_next; // save the head(start of the skip list)

        delete(node_to_remove); // remove the level 0 node

        // Create up node if it doesnt exist on the next node(initial head -> next)
        node_next = create_node_up_if_not_exist(node_next);
        SkipList::Node* node_to_remove_copy_traverse = node_to_remove_up; // copy for traversal
        
        while (node_to_remove_up != NULL){
            if (node_next != node_to_remove_copy_traverse -> next){ // to avoid cyclical pointers
                node_next -> next = node_to_remove_copy_traverse -> next;
            }
            if (node_next -> next != NULL){
                node_next -> next -> prev = node_next; 
            }
            node_to_remove_copy_traverse = node_to_remove_copy_traverse -> up; // go up for the original node
            
            // For the next iteration
            if (node_to_remove_copy_traverse != NULL){
                node_next = create_node_up_if_not_exist(node_next);
            }
            delete(node_to_remove_up); // delete the node we have just rewired pointers for 
            node_to_remove_up = node_to_remove_copy_traverse; // save it so we can delete further nodes as well
        }
    }
}

void SkipList::remove_element(int value){
    SkipList::Node* node_to_be_removed = find_element(value);
    if (node_to_be_removed == NULL){
        throw std::runtime_error("Element you want to remove doesn't exist");
    }
    this -> ELEMENT_COUNT--; // decrement element count once we know that the value exists.

    if (this -> tail -> val == this -> head -> val){
        throw std::runtime_error("You wanted to remove the last element!");
    }

    if (node_to_be_removed -> prev == NULL){ // if we are removing the minimum
        remove_element_min_max(node_to_be_removed, 0);
        return;
    }
    if (node_to_be_removed -> next == NULL){ // if we are removing the maximum
        remove_element_min_max(node_to_be_removed, 1);
        return;
    }

    while (node_to_be_removed != NULL){
        SkipList::Node* node_to_be_removed_traverse = node_to_be_removed; // make a copy for traversal
        
        node_to_be_removed_traverse -> prev -> next = node_to_be_removed_traverse -> next; // connect the left to right
        if (node_to_be_removed_traverse -> next != NULL){ // if next element isn't null
            node_to_be_removed_traverse -> next -> prev = node_to_be_removed_traverse -> prev; // connect the right to left
        }

        node_to_be_removed = node_to_be_removed -> up; // go up with the original pointer so we can later free the removed node
        delete(node_to_be_removed_traverse); // delete the node and free up pointer
    }
    return;
}

void SkipList::insert_element(int value){
    if (find_element(value) != NULL){
        throw std::runtime_error("Element with this value is already in the skip list!");
    }
    int MAX_LVL_NEW = std::log2(this -> ELEMENT_COUNT + 1); // calculate MAX_LVL_NEW

    if (this -> ELEMENT_COUNT == 1){
        if (this -> head -> val > value){
            insert_element_min(value, this -> head, MAX_LVL_NEW); // TODO do we really need to pass this->head here? just use it inside the function
        } else {
            insert_element_max(value, this -> tail, MAX_LVL_NEW);
        }
        this -> ELEMENT_COUNT++; // increment the amount of elements
        return;
    }
    this -> ELEMENT_COUNT++; // increment the amount of elements
    std::vector<SkipList::Node*> nodes = find_closest_elements(value);
    
    if (nodes[1] == NULL){ // if we have inserted the minimum
        insert_element_min(value, this-> head, MAX_LVL_NEW);
        return;
    }
    if (nodes[0] == NULL){
        insert_element_max(value, this -> tail, MAX_LVL_NEW);
        return;
    }

    SkipList::Node* start = new SkipList::Node; // maintain pointer to the start of the new ll
    SkipList::Node* traverse_node = start; // for traversing and adding nodes to the new ll
    start -> val = value;
    int insert_height = 0;

    // Put the level 0 node inside the list
    start -> prev = nodes[0];
    start -> next = nodes[1];
    nodes[0] -> next = start;
    nodes[1] -> prev = start;

    // Now we grow it till we randomly reach the end while also connecting the nodes to the closest
    // ones on the same level
    while (SkipList::next() && insert_height < MAX_LVL_NEW){
        insert_height++; // height variable indicates on which height we currently are
        SkipList::Node* start_copy_left = start;
        SkipList::Node* start_copy_right = start;
        SkipList::Node* insert_node_up = new SkipList::Node;
        insert_node_up -> val = traverse_node -> val;
        
        // two way connection
        insert_node_up -> down = traverse_node;
        traverse_node -> up = insert_node_up;
        traverse_node = insert_node_up;

        if (this -> MAX_LVL < MAX_LVL_NEW && insert_height > this -> MAX_LVL){
            // If we can breakthrough max level and we are on that height to be breaking through
            SkipList::Node* head_tip = SkipList::go_up_till_end(this -> head); // tip of the head node
            SkipList::Node* head_up = new SkipList::Node;
            // both ways connection
            head_up -> val = head_tip -> val;
            head_tip -> up = head_up;
            head_up -> down = head_tip;
            head_up -> next = insert_node_up;
            insert_node_up -> prev = head_up;

            // Now do the same for the tail
            SkipList::Node* tail_tip = SkipList::go_up_till_end(this -> tail);
            SkipList::Node* tail_up = new SkipList::Node;
            // both ways connection
            tail_up -> val = tail_tip -> val;
            tail_tip -> up = tail_up;
            tail_up -> down = tail_tip;
            tail_up -> prev = insert_node_up;
            insert_node_up -> next = tail_up;

            // Update MAX_LVL
            this -> MAX_LVL = MAX_LVL_NEW;
        } else {
            // This is for when we have normal case(no breakthrough the MAX_LVL)
            // search for the node with appropriate height
            while (start_copy_left -> prev != NULL && start_copy_left -> height < insert_height){ // search to the left before we reach the start
                start_copy_left = start_copy_left -> prev;
            }

            if (start_copy_left -> height >= insert_height){
                start_copy_left = go_up_n_times(start_copy_left, insert_height); // goes up to the height on which we want to connect
                
                if (start_copy_left -> next != NULL){
                    // copy the pointers of the node it was originally pointing to before changing it's pointers
                    insert_node_up -> next = start_copy_left -> next;
                    insert_node_up -> next -> prev = insert_node_up;
                }
                start_copy_left -> next = insert_node_up; // connect the left node at that height to the one we are inserting
                insert_node_up -> prev = start_copy_left; // connect the node we are inserting to that node by prev pointer
            }
            
            // connect to right on appropriate level
            while (start_copy_right -> next != NULL && start_copy_right -> height < insert_height){
                start_copy_right = start_copy_right -> next;
            }
            if (start_copy_right -> height >= insert_height){
                start_copy_right = go_up_n_times(start_copy_right, insert_height);

                if (start_copy_right -> prev != NULL){
                    insert_node_up -> prev = start_copy_right -> prev;
                    insert_node_up -> prev -> next = insert_node_up;
                } 

                start_copy_right -> prev = insert_node_up;
                insert_node_up -> next = start_copy_right;
            }
        }
    }
    start -> height = insert_height;
}

// Print the skip list
void SkipList::print(){
    SkipList::Node* top_left = this -> head;
    // Find the top level head
    while (top_left->up) {
        top_left = top_left->up;
    }

    // Iterate from top level to bottom
    while (top_left) {
        SkipList::Node* node = top_left;
        // Iterate from head to tail at this level
        while (node) {
            cout << node->val << " ";
            node = node->next;
        }
        cout << "\n";
        // Move to the next level down
        top_left = top_left->down;
    }
}