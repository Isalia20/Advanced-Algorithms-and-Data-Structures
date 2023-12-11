#include <iostream>
#include <fstream> // For file I/O
#include "SkipList.h"
#include "BST.h"

int main(){
    SkipList sl;
    BST tree;
    
    std::vector<int> some_list;
    for (int i = 0; i <= 100000; i++){
        some_list.push_back(i);
    }
    
    sl.build_skip_list_deterministic(some_list);
    tree.build_bst(some_list);

    std::ofstream file_skiplist("count_ops_skiplist.txt");
    std::ofstream file_bst("count_ops_bst.txt");

    for (int i = 0; i < some_list.size(); i++){ // for each element append to the list the amount of operations needed to be done
        sl.find_element_ops(i, file_skiplist);
        file_bst << "ELEMENT - " << i << std::endl;
        tree.find_element_ops(tree.root, i, file_bst);
        file_bst << "------------------------------------" << std::endl;
    }

    return 0;
}
