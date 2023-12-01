#include <iostream>
#include <fstream>
#include "SkipList.h"


int main(){
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> distrib;

    SkipList* sl = new SkipList;
    std::vector<int> result;

    std::vector<int> list;
    for (int i = 0; i < 10000; i++){
        list.push_back(i);
    }

    std::ofstream myfile("height_results_random.txt");
    for (int i = 0; i < 1000; i++){
        sl -> build_skip_list_random(list);
        result = sl -> get_level_node_count();
        for (int j = 0; j < result.size(); j++){
            myfile << result[j] << std::endl;
        }
        myfile << "---------------------------------" << std::endl;
    }
}
