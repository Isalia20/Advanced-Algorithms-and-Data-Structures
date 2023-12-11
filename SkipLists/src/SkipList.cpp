#include "SkipList.h"
#include <iostream>
#include <vector>
#include <random>
#include <iostream>
#include <fstream> // For file I/O

using namespace std;

// for random int generation 0 and 1
int SkipList::rand_bool(){
    return distrib(gen);
}

SkipList::SkipList() : gen(rd()), distrib(0, 1) {}; // constructor including random number generator
SkipList::~SkipList() { 
    Node* currentNode = head;
    unordered_set<Node*> nodesToDelete; // stores unique nodes

    // Iterate over the SkipList to collect all unique nodes
    while(currentNode) {
        Node* nextNode = currentNode->next;
        Node* upNode = currentNode->up;

        // Traverse down nodes and add them to the deletion set
        while(currentNode) {
            Node* downNode = currentNode->down;
            nodesToDelete.insert(currentNode);
            currentNode = downNode;
        }

        // If there is an up node, go to it; otherwise, go to the next node
        if(upNode) {
            currentNode = upNode;
        }
        else {
            currentNode = nextNode;
        }
    }

    // Delete all unique nodes
    for(auto node : nodesToDelete) {
        delete node;
    }
};

void SkipList::build_up_rand(Node* startNode, vector<vector<Node*>> &previousNodes, bool &hasTower){
    Node* traverseNode = startNode;
    vector<Node*> upperNodes;
    Node* previousNode; // used for connecting nodes horizontally
    int levelCounter = 0;

    // continue building up while random boolean is true and levelCounter is less than MAX_LVL
    while (rand_bool() && levelCounter < this->MAX_LVL){
        levelCounter++;
        Node* nodeUp = new Node;
        nodeUp->val = traverseNode->val;
        nodeUp->down = traverseNode;
        traverseNode->up = nodeUp;
        traverseNode = nodeUp;
        
        // if we have nodes before in the upper section
        if (previousNodes.size() > 0){
            int i = previousNodes.size() - 1;
            // Before we find the index where level matches the current one, keep iterating backwards
            while (previousNodes[i].size() < levelCounter && i >= 0){ 
                i--;
            }
            // Once we have found it, we just need to get it and set the pointers to each other
            if (i >= 0 && previousNodes[i].size() >= levelCounter){
                previousNode = previousNodes[i][levelCounter - 1];
                previousNode->next = traverseNode;
                traverseNode->prev = previousNode;
            }
        }
        upperNodes.push_back(traverseNode);
        hasTower = true;
    }
    previousNodes.push_back(upperNodes);
}

void SkipList::build_up(Node* startNode, int level, vector<vector<Node*>> &previousNodes){
    Node* traverseNode = startNode;
    vector<Node*> upperNodes;
    Node* previousNode; // used for connecting nodes horizontally
    int levelCounter = 0;

    // continue building up until level reaches 0
    while (level > 0){
        levelCounter++;
        Node* nodeUp = new Node;
        nodeUp->val = traverseNode->val;
        nodeUp->down = traverseNode;
        traverseNode->up = nodeUp;
        traverseNode = nodeUp;
        
        // if we have nodes before in the upper section
        if (previousNodes.size() > 0){
            int i = previousNodes.size() - 1;
            // Before we find the index where level matches the current one, keep iterating backwards
            while (previousNodes[i].size() < levelCounter && i >= 0){ 
                i--;
            }
            // Once we have found it, we just need to get it and set the pointers to each other
            if (i >= 0 && previousNodes[i].size() >= levelCounter){
                previousNode = previousNodes[i][levelCounter - 1];
                previousNode->next = traverseNode;
                traverseNode->prev = previousNode;
            }
        }
        upperNodes.push_back(traverseNode);
        level--;
    }
    previousNodes.push_back(upperNodes);
}

void SkipList::build_skip_list_random(vector<int>& inputList){
    sort(inputList.begin(), inputList.end());
    int currentIndex = 0;
    int maxLevel = std::log2(inputList.size());
    this->MAX_LVL = maxLevel;
    this->ELEMENT_COUNT = inputList.size();

    // Start building the linked list
    Node* currentNode;
    Node* previousNode = nullptr;
    vector<vector<Node*>> previousNodes;

    while (currentIndex < inputList.size()){
        currentNode = new Node;
        currentNode->val = inputList[currentIndex];
        bool hasTower = false;
        // Build node up to the max level it can go
        // based on randomness if not the start or end level
        if (currentIndex == 0 || currentIndex == inputList.size() - 1){
            build_up(currentNode, this->MAX_LVL, previousNodes);
            hasTower = true;
        } else {
            // Randomly grow if we are in the middle(not at the start/end)
            build_up_rand(currentNode, previousNodes, hasTower);
        }
        currentNode->prev = previousNode;
        // make it doubly linked list
        if (previousNode != NULL){
            previousNode->next = currentNode;
        }
        previousNode = currentNode;
        // Set the head of the skip list 
        if (currentIndex == 0) this->head = currentNode;
        // Set the end of the skip list 
        if (currentIndex == inputList.size() - 1) this->tail = currentNode;
        if (hasTower){
            currentNode->height = previousNodes[previousNodes.size() - 1].size(); // get the height of the tower(i.e. last tower inserted)
        } else {
            currentNode->height = 0;
        }
        currentIndex++;
    }
}

void SkipList::build_skip_list_deterministic(vector<int>& inputList){
    sort(inputList.begin(), inputList.end());
    int currentIndex = 0;
    int maxLevel = std::log2(inputList.size());
    this->MAX_LVL = maxLevel;
    this->ELEMENT_COUNT = inputList.size();
    vector<int> levels;

    // for example for max level 2 it will create vector of 
    // {4, 2} which will later be used for build up
    // {2, 1} which will be later used for determining how  
    // high we should go
    for (int i = maxLevel; i > 0; i--){
        levels.push_back(pow(2, i));
    }

    // Start building the linked list
    Node* currentNode;
    Node* previousNode = nullptr;
    vector<vector<Node*>> previousNodes;

    while (currentIndex < inputList.size()){
        currentNode = new Node;
        currentNode->val = inputList[currentIndex];
        bool hasTower = false;
        // Build node up to the max level it can go
        // based on the exponents of 2
        // 2, 4, 8, 16...
        for (int j = 0; j < levels.size(); j++){
            if (currentIndex % levels[j] == 0){
                build_up(currentNode, std::log2(levels[j]), previousNodes);
                hasTower = true;
                break;
            }
            if (currentIndex == inputList.size() - 1){
                build_up(currentNode, std::log2(levels[0]), previousNodes); // build the tail to the maximum
                hasTower = true;
                break;
            }
        }
        currentNode->prev = previousNode;
        // make it doubly linked list
        if (previousNode != NULL){
            previousNode->next = currentNode;
        }
        previousNode = currentNode;
        // Set the head of the skip list
        if (currentIndex == 0){
            this->head = currentNode;
        }
        // Set the end of the skip list 
        if (currentIndex == inputList.size() - 1){
            this->tail = currentNode;
        }

        if (hasTower){
            currentNode->height = previousNodes[previousNodes.size() - 1].size(); // get the height of the tower(i.e. last tower inserted)
        } else {
            currentNode->height = 0; // tower has a height of 1
        }
        currentIndex++;
    }
};

SkipList::Node* SkipList::find_element_rec(int searchValue, Node* traversalNode){
    // Traverse through the list until we find a value greater than searchValue
    while (traversalNode->next != NULL && traversalNode->next->val <= searchValue){
        traversalNode = traversalNode->next;
        // If we find the searchValue, return the node
        if (traversalNode->val == searchValue){
            return traversalNode;
        }
    }
    // If we have traversed to the end of the list and didn't find the element
    if (traversalNode->down == NULL){
        return nullptr;
    }
    // Recursively call the function on the lower level of the skip list
    Node* desiredNode = find_element_rec(searchValue, traversalNode->down);
    return desiredNode;
}

vector<SkipList::Node*> SkipList::find_closest_element_rec(int searchValue, SkipList::Node* traversalNode){
    // Traverse through the list until we find a value greater than searchValue
    while (traversalNode->next != NULL && traversalNode->next->val <= searchValue){
        traversalNode = traversalNode->next;
        // If we find the searchValue, return the node
        if (traversalNode->val == searchValue){
            return {traversalNode}; // if already exists return the node itself and will be dealt in the function find_closest_elements
        }
    }
    // If we have traversed to the end of the list and didn't find the element
    if (traversalNode->down == NULL){
        return {traversalNode, traversalNode->next};
    }
    // Recursively call the function on the lower level of the skip list
    vector<Node*> closestElements = find_closest_element_rec(searchValue, traversalNode->down);
    return closestElements;
}

SkipList::Node* SkipList::get_bottom_node(SkipList::Node* traversalNode){
    /* 
    This function is to get the most bottom node 
    in the skip list. Used in find_element function 
    so the return of the function is consistent
    (i.e. doesn't return middle/top/bottom randomly)
    */
    // If we already get a nullptr, return it
    if (traversalNode == NULL){
        return traversalNode;
    } else {
        // Traverse down the skip list to reach the bottom node
        while (traversalNode->down != NULL){
            traversalNode = traversalNode->down;
        }
    }
    return traversalNode;
}

SkipList::Node* SkipList::find_element(int searchValue){
    /* 
    Returns a node in the skip list 
    containing the desired value
    */
    Node* traversalNode = this -> head;
    // If the head of the list is already the value we are searching for
    if (traversalNode -> val == searchValue){
        return traversalNode;
    }

    // Go to the top of the skip list(upper left corner)
    while (traversalNode -> up != NULL){
        traversalNode = traversalNode -> up;
    }
    
    // Call the recursive find_element_rec function
    Node* foundNode = find_element_rec(searchValue, traversalNode);

    if (foundNode == NULL){
        return foundNode;
    } else {
        // Return the bottom node of the found node
        return get_bottom_node(foundNode);
    }
}

SkipList::Node* SkipList::find_element_ops(int searchValue, std::ofstream& file){
    /* 
    Returns a node in the skip list 
    containing the desired value
    */
    file << "ELEMENT - " << searchValue << std::endl;
    Node* traversalNode = this -> head;
    // If the head of the list is already the value we are searching for
    if (traversalNode -> val == searchValue){
        file << "1" << std::endl;
        file << "------------------------------------" << std::endl;
        return traversalNode;
    }

    // Go to the top of the skip list(upper left corner)
    while (traversalNode -> up != NULL){
        traversalNode = traversalNode -> up;
    }
    
    // Call the recursive find_element_rec function
    Node* foundNode = find_element_rec_ops(searchValue, traversalNode, file);

    file << "------------------------------------" << std::endl;

    if (foundNode == NULL){
        return foundNode;
    } else {
        // Return the bottom node of the found node
        return get_bottom_node(foundNode);
    }
}

SkipList::Node* SkipList::find_element_rec_ops(int searchValue, Node* traversalNode, std::ofstream& file){
    // Traverse through the list until we find a value greater than searchValue
    while (traversalNode->next != NULL && traversalNode->next->val <= searchValue){
        file << "1" << std::endl;
        traversalNode = traversalNode->next;
        // If we find the searchValue, return the node
        if (traversalNode->val == searchValue){
            return traversalNode;
        }
    }
    file << "1" << std::endl;
    // If we have traversed to the end of the list and didn't find the element
    if (traversalNode->down == NULL){
        return nullptr;
    }
    // Recursively call the function on the lower level of the skip list
    Node* desiredNode = find_element_rec_ops(searchValue, traversalNode->down, file);
    return desiredNode;
}


vector<SkipList::Node*> SkipList::find_closest_elements(int searchValue){
    /* 
    Returns a pair of nodes in the skip list 
    that are closest to the desired value
    */
    
    Node* traversalNode = this->head;
    
    // If the head of the list is already the value we are searching for
    if (traversalNode->val == searchValue){
        return {traversalNode->prev, traversalNode->next};
    }
    
    // Go to the top of the skip list (upper left corner)
    while (traversalNode->up != NULL){
        traversalNode = traversalNode->up;
    }
    
    // Call the recursive find_closest_element_rec function
    vector<Node*> closestNodes = find_closest_element_rec(searchValue, traversalNode);
    
    if (closestNodes.size() == 1){
        closestNodes[0] = get_bottom_node(closestNodes[0]);
        Node* leftNode = nullptr;
        Node* rightNode = nullptr;

        // Left pointer
        if (closestNodes[0]->prev != NULL){
            leftNode = closestNodes[0]->prev;
        }
        // Right pointer
        if (closestNodes[0]->next != NULL){
            rightNode = closestNodes[0]->next;
        }
        return {leftNode, rightNode};
    } else if (closestNodes[1] == NULL){ // If we have maximum case
        closestNodes[0] = get_bottom_node(closestNodes[0]);
        closestNodes[1] = closestNodes[0];
        closestNodes[0] = nullptr;
        return closestNodes;
    } else {
        closestNodes[0] = get_bottom_node(closestNodes[0]);
        closestNodes[1] = get_bottom_node(closestNodes[1]);
        if (closestNodes[0] && closestNodes[0]->val > searchValue){ // If we put in a value less than the minimum
            closestNodes[1] = nullptr;
        }
        return closestNodes;
    }
}

SkipList::Node* SkipList::go_up_till_end(SkipList::Node* traversalNode){
    /* 
    A helper function to go up the skip list from 
    a given node until it reaches the top node
    */
    
    Node* traversalCopy = traversalNode;
    // Traverse up the list until the top node is reached
    while (traversalCopy->up != NULL){
        traversalCopy = traversalCopy->up;
    }
    return traversalCopy;
}

SkipList::Node* SkipList::go_up_n_times(SkipList::Node* traversalNode, int steps){
    /* 
    A helper function to go up the skip list from 
    a given node for a certain number of steps
    */

    // Traverse up the list for the given number of steps
    while (steps > 0){
        if (traversalNode->up != NULL){
            traversalNode = traversalNode->up;
        }
        steps--;
    }
    return traversalNode;
}

void SkipList::insert_element_min(int value, Node* node, int maxLevelNew){

    Node* newStart = new Node; // Maintain pointer to the start of the new linked list
    Node* traverseNode = newStart; // For traversing and adding nodes to the new linked list
    Node* nodeCopy = node; // Copy of the head or tail of skip list
    newStart->val = value; // Assign value to the node
    int height = 0;
    int insertHeight = 0;

    // If the new node is the minimum (inserted on the left)
    newStart->next = nodeCopy;
    nodeCopy->prev = newStart;
    this->head = newStart;
    Node* insertNodeUp;

    // For start of the list, we have to grow it to the top because searching relies on the first element having the highest level
    while (height < maxLevelNew){ 
        insertHeight++;
        insertNodeUp = new Node;

        // Two-way connection
        insertNodeUp->down = traverseNode;
        traverseNode->up = insertNodeUp;
        traverseNode = insertNodeUp;

        // Search for the node with appropriate height
        while (nodeCopy->next != NULL && nodeCopy->height < insertHeight){
            nodeCopy = nodeCopy->next;
        }

        if (nodeCopy->height >= insertHeight){
            Node* nodeForConnecting = nodeCopy;
            nodeForConnecting = go_up_n_times(nodeForConnecting, insertHeight);
            traverseNode->next = nodeForConnecting;
            nodeForConnecting->prev = traverseNode;
        }
        
        insertNodeUp->val = value;
        height++;
    }

    newStart->height = height; // Assign height to the bottom most node

    if (maxLevelNew > this->MAX_LVL){
        Node* tailEnd = go_up_till_end(this->tail);
        Node* tailEndUp = new Node;
        tailEndUp->val = tailEnd->val;
        tailEndUp->down = tailEnd;
        tailEnd->up = tailEndUp;

        insertNodeUp->next = tailEndUp;
        tailEndUp->prev = insertNodeUp;
    }
    this->MAX_LVL = maxLevelNew;
}

void SkipList::insert_element_max(int value, Node* node, int maxLevelNew){

    Node* newStart = new Node; // Maintain pointer to the start of the new linked list
    Node* traverseNode = newStart; // For traversing and adding nodes to the new linked list
    Node* nodeCopy = node; // Copy of the head or tail of skip list
    newStart->val = value; // Assign value to the node
    int height = 0;
    int insertHeight = 0;

    // If the new node is the maximum (inserted on the right)
    newStart->prev = nodeCopy;
    nodeCopy->next = newStart;
    this->tail = newStart;
    Node* insertNodeUp;

    while (height < maxLevelNew){
        insertHeight++;
        insertNodeUp = new Node;

        // Two-way connection
        insertNodeUp->down = traverseNode;
        traverseNode->up = insertNodeUp;
        traverseNode = insertNodeUp;

        // Search for the node with appropriate height to the left before we reach the start or find the appropriate node to insert the next level element in
        while (nodeCopy->prev != NULL && nodeCopy->height < insertHeight){
            nodeCopy = nodeCopy->prev;
        }
        
        if (nodeCopy->height >= insertHeight){
            Node* nodeForConnecting = nodeCopy;
            nodeForConnecting = go_up_n_times(nodeForConnecting, insertHeight);
            traverseNode->prev = nodeForConnecting;
            nodeForConnecting->next = traverseNode;
        }
        
        insertNodeUp->val = value;
        height++;
    }

    newStart->height = height; // Assign height to the bottom most node

    if (maxLevelNew > this->MAX_LVL){
        Node* headEnd = go_up_till_end(this->head);
        Node* headEndUp = new Node;
        headEndUp->val = headEnd->val;
        headEndUp->down = headEnd;
        headEnd->up = headEndUp;

        insertNodeUp->prev = headEndUp;
        headEndUp->next = insertNodeUp;
    }
    this->MAX_LVL = maxLevelNew;
}


SkipList::Node* SkipList::create_node_up_if_not_exist(SkipList::Node* node){
    if (node->up == NULL){
        // Create new node which will be connected with up pointer
        Node* nodeUp = new Node;
        nodeUp->val = node->val;
        
        // Connect it two ways
        nodeUp->down = node;
        node->up = nodeUp;
        
        // Assign the new node to the original node pointer
        node = nodeUp;
    } else {
        // If the up node already exists, just move the pointer to it
        node = node->up;
    }
    return node;
}

void SkipList::remove_nodes_without_applying_connections(SkipList::Node* node){
    /* This function is used when we have only 2 elements left 
    in skip list and someone deletes an element. In this case, 
    to prevent a cyclic linked list, we only delete the nodes and that's all
    (i.e., pointers from the node that remains become nullptrs)
    */
    Node* nodeCopy = node;
    while (nodeCopy != NULL){
        nodeCopy = nodeCopy->up; // Move up the skip list

        // If there is a previous node, disconnect it
        if (node->prev != NULL){
            node->prev->next = nullptr;
        }
        
        // If there is a next node, disconnect it
        if (node->next != NULL){
            node->next->prev = nullptr;
        }
        
        // Delete the current node
        delete node;
        
        // Move to the next node
        node = nodeCopy;
    }
}

void SkipList::remove_element_min_max(Node* node_to_remove, bool is_max){
    /* removing maximum or minimum element 
    is dealt through this function */
    Node* node_to_remove_up = node_to_remove->up;
    Node* node_adjacent = is_max ? node_to_remove->prev : node_to_remove->next;
   
    if ((is_max && node_adjacent->prev == NULL) || (!is_max && node_adjacent->next == NULL)){
        if (is_max) {
            this->tail = node_adjacent;
        } else {
            this->head = node_adjacent;
        }
        remove_nodes_without_applying_connections(node_to_remove); // then remove without rewiring the pointers
        return;
    }

    if (is_max) {
        node_to_remove->prev = nullptr; 
        node_adjacent->next = nullptr;
        this->tail = node_adjacent; 
    } else {
        node_adjacent->prev = nullptr; 
        this->head = node_adjacent;
    }

    delete(node_to_remove); // remove the level 0 node

    Node* node_to_remove_copy_traverse;
    // Create up node if it doesn't exist on the next node(initial head->next)
    if (node_to_remove_up != NULL){
        node_adjacent = create_node_up_if_not_exist(node_adjacent);
        node_to_remove_copy_traverse = node_to_remove_up; // copy for traversal
    }

    while (node_to_remove_up != NULL){
        if (is_max && node_adjacent != node_to_remove_copy_traverse->prev){ 
            node_adjacent->prev = node_to_remove_copy_traverse->prev;
        } else if (!is_max && node_adjacent != node_to_remove_copy_traverse->next){
            node_adjacent->next = node_to_remove_copy_traverse->next;
        }
        
        if ((is_max && node_adjacent->prev != NULL) || (!is_max && node_adjacent->next != NULL)){
            if (is_max) {
                node_adjacent->prev->next = node_adjacent;
            } else {
                node_adjacent->next->prev = node_adjacent;
            }
        }
        
        node_to_remove_copy_traverse = node_to_remove_copy_traverse->up; // go up for the original node
        
        // For the next iteration
        if (node_to_remove_copy_traverse != NULL){
            node_adjacent = create_node_up_if_not_exist(node_adjacent);
        }
        delete(node_to_remove_up); // delete the node we have just rewired pointers for 
        node_to_remove_up = node_to_remove_copy_traverse; // save it so we can delete further nodes as well
    }
}

void SkipList::remove_element(int value){
    // find the node with the given value
    Node* nodeToRemove = find_element(value);

    // If the node doesn't exist, throw an error
    if (nodeToRemove == NULL){
        throw std::runtime_error("Element you want to remove doesn't exist");
    }

    // Decrement the count of elements
    this->ELEMENT_COUNT--;

    // If we're trying to remove the last element, throw an error
    if (this->tail->val == this->head->val){
        throw std::runtime_error("You wanted to remove the last element!");
    }

    // If we're removing the minimum element
    if (nodeToRemove->prev == NULL){
        remove_element_min_max(nodeToRemove, 0);
        return;
    }

    // If we're removing the maximum element
    if (nodeToRemove->next == NULL){
        remove_element_min_max(nodeToRemove, 1);
        return;
    }

    // If we're removing an element that is not the min or max
    while (nodeToRemove != NULL){
        // Create a copy for traversal
        Node* nodeToRemoveCopy = nodeToRemove;

        // Connect the previous node to the next node
        nodeToRemoveCopy->prev->next = nodeToRemoveCopy->next;

        // If the next node exists, connect it to the previous node
        if (nodeToRemoveCopy->next != NULL){
            nodeToRemoveCopy->next->prev = nodeToRemoveCopy->prev;
        }

        // Move up in the list
        nodeToRemove = nodeToRemove->up;

        // Delete the current node
        delete nodeToRemoveCopy;
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
    vector<SkipList::Node*> nodes = find_closest_elements(value);
    
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
    while (SkipList::rand_bool() && insert_height < MAX_LVL_NEW){
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

std::vector<int> SkipList::get_level_node_count(){
    std::vector<int> result;
    SkipList::Node* node_copy = this -> head;
    node_copy = go_up_till_end(node_copy); 

    while (node_copy != this -> head){
        SkipList::Node* node_copy_traverse_right = node_copy;
        int num_elements_per_level = 0;
        while (node_copy_traverse_right != nullptr){
            num_elements_per_level++;
            node_copy_traverse_right = node_copy_traverse_right -> next;
        }
        node_copy = node_copy -> down;
        result.push_back(num_elements_per_level);
    }
    return result;
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

void SkipList::print_range(int X, int Y){
    /* 
    NOTE:
    This function is just a very basic implementation
    and lacks considering many edge cases and might give 
    you errors. USE WITH CAUTION!!!
    */
    Node* head = this -> head;
    if (X > Y){ // If the given range is invalid, X should be left and Y should be right.
        throw std::runtime_error("Invalid range!");
    }

    if (this -> head -> val > Y || this -> tail -> val < X){ // If range is invalid for the elements we have
        std::cout << "NO ELEMENTS IN THIS RANGE" << std::endl;
    }
    
    std::vector<SkipList::Node*> closest_elements = find_closest_elements(X); // find closest left and right nodes to element X
    Node* start;

    if (closest_elements[0] -> next -> val == X){ // if equal then get the vaue itself, look at how find_closest_elements work if value exists
        start = closest_elements[0] -> next;
    } else { // if not equal then we should start with the value greater than the min of X
        start = closest_elements[1];
    }

    Node* start_traverse = start;

    while (start_traverse && start_traverse -> val <= Y){
        std::cout << start_traverse -> val << std::endl;
        start_traverse = start_traverse -> next;
    }
}