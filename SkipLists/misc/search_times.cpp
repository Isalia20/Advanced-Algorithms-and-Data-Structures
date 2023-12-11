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

    int list_size = std::stoi(argv[1]); // used later for functions
    std::string str_list_size = std::to_string(list_size); // used for naming file(txts)

    for (int i = 0; i <= list_size; i++){
        list.push_back(i);
    }
    sl -> build_skip_list_random(list);
    ll -> build_ll(list);
    bst -> build_bst(list);

    // Open files to write results
    std::ofstream myfile[21];
    for (int i = 1; i < 21; ++i) {
        myfile[i - 1].open("time_results_search_" + std::to_string(i * 5) + "th_percentile_random_" + str_list_size + ".txt");
    }

    for (int percentile = 5; percentile <= 100; percentile += 5) {
        int index = list_size / 100 * percentile;
        std::cout << index << std::endl;
        int fileIndex = (percentile / 5) - 1;
        for (int i =0; i < 200; i++){
            measure_and_log([&](){ ll->find_element(index); }, "LinkedList", myfile[fileIndex]);
            measure_and_log([&](){ sl->find_element(index); }, "SkipList", myfile[fileIndex]);
            measure_and_log([&](){ bst->find_element(bst->root, index); }, "BST", myfile[fileIndex]);
        }
    }

    delete(sl);
    // delete(ll);
    delete(bst);

    // Close the files
    for (auto& file : myfile) {
        file.close();
    }
    return 0;
}
