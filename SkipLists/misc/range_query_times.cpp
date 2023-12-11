#include "BST.h"
#include "SkipList.h"
#include "LinkedList.h"
#include <iostream>
#include <fstream>


int main(){
    SkipList sl;
    BST bst;
    LinkedList ll;
    vector<int> list;


    for (int i = 0; i < 10000000; i++){
        list.push_back(i);
    }

    bst.build_bst(list);
    sl.build_skip_list_deterministic(list);

    std::ofstream file_bst("range_query_bst.txt");
    std::ofstream file_sl("range_query_sl.txt");

    for (int i = 0; i < 200; i++){
        auto start = std::chrono::high_resolution_clock::now();
        sl.print_range(500, 100000);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        file_sl << duration << std::endl;

        start = std::chrono::high_resolution_clock::now();
        bst.printRange(bst.root, 500, 100000);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        file_bst << duration << std::endl;
    }

    return 0;
}
