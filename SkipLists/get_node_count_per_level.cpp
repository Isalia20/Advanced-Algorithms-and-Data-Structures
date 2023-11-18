#include <iostream>
#include <fstream>
#include "SkipList.h"


int main(){
    SkipList* sl = new SkipList;
    std::vector<int> result;

    std::vector<int> list;
    for (int i = 0; i < 10000; i++){
        list.push_back(i);
    }

    sl -> build_skip_list_random(list);
    result = sl -> get_level_node_count();
    std::ofstream myfile("height results.txt");
    for (int i = 0; i < 1000; i++){
        for (int j = 0; j < result.size(); j++){
            myfile << result[j] << std::endl;
        }
        myfile << "---------------------------------" << std::endl;
    }
}
