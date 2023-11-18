#include <fstream>  // For file I/O
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>  // For timing
#include <limits>
#include "SkipList.h"
#include "LinkedList.h"
#include "BST.h"
#include <string>

void measure_and_log(std::function<void()> function, const std::string& name, std::ofstream& myfile) {
    auto start = std::chrono::high_resolution_clock::now();
    function();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    myfile << name << " - " << duration << std::endl;
}

int main(int argc, char* argv[]){
    SkipList* sl = new SkipList;
    LinkedList* ll = new LinkedList;
    BST* bst = new BST;
    vector<int> list;

    int list_size = std::stoi(argv[1]);
    std::string str_list_size = std::to_string(list_size);

    for (int i = 0; i <= list_size; i++){
        list.push_back(i);
    }
    sl -> build_skip_list_deterministic(list);
    ll -> build_ll(list);
    bst -> build_bst(list);

    // Open files to write results
    std::ofstream myfile1("time_results_serch_last_deterministic_" + str_list_size + ".txt");
    std::ofstream myfile2("time_results_serch_mid_deterministic_" + str_list_size + ".txt");
    std::ofstream myfile3("time_results_serch_quarter_deterministic_" + str_list_size + ".txt");
    std::ofstream myfile4("time_results_serch_third_quarter_deterministic_" + str_list_size + ".txt");

    for(int i = 0; i < 200; ++i) {
        measure_and_log([&](){ ll->find_element(list_size); }, "LinkedList", myfile1); // End of the list
        measure_and_log([&](){ sl->find_element(list_size); }, "SkipList", myfile1);
        measure_and_log([&](){ bst->find_element(bst -> root, list_size); }, "BST", myfile1);
        measure_and_log([&](){ ll->find_element(list_size/2); }, "LinkedList", myfile2); // Middle of the list 
        measure_and_log([&](){ sl->find_element(list_size/2); }, "SkipList", myfile2);
        measure_and_log([&](){ bst->find_element(bst -> root, list_size/2); }, "BST", myfile2);
        measure_and_log([&](){ ll->find_element(list_size/4); }, "LinkedList", myfile3); // Quarter of the list
        measure_and_log([&](){ sl->find_element(list_size/4); }, "SkipList", myfile3);
        measure_and_log([&](){ bst->find_element(bst -> root, list_size/4); }, "BST", myfile3);
        measure_and_log([&](){ ll->find_element(list_size * 3/4); }, "LinkedList", myfile4); // 3/4th of the list
        measure_and_log([&](){ sl->find_element(list_size * 3/4); }, "SkipList", myfile4);
        measure_and_log([&](){ bst->find_element(bst -> root, list_size * 3/4); }, "BST", myfile4);
    }

    delete(sl);
    delete(ll);
    delete(bst);

    // Close the files
    myfile1.close(); 
    myfile2.close();
    myfile3.close();
    return 0;
}