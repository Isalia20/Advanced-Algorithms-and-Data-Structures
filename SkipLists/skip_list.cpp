#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "SkipList.h"

using namespace std;

int main(){
    SkipList sl;
    for (int i = 0; i < 100000; i++){
        srand(static_cast<unsigned>(std::time(0)));
        std::vector<int> some_vector = {10, 20, 30, 40, 50, 60, 70, 80, 90};
        sl.build_skip_list_random(some_vector);
        sl.print();
    }
    
    // vector<int> some_vector = {10};
    // for (int i = 0; i < 65; i++){
    // some_vector.push_back(i);
    // }
    // sl.build_skip_list_deterministic(some_vector);
    // sl.build_skip_list_random(some_vector);
    // sl.remove_element(30);
    // sl.remove_element(40);
    // sl.remove_element(50);
    // sl.remove_element(60);
    // sl.remove_element(70);
    // sl.remove_element(80);
    // sl.remove_element(90);
    // sl.remove_element(20);
    // sl.insert_element(20);
    // sl.insert_element(11);
    // sl.insert_element(12);
    // sl.insert_element(13);
    // sl.insert_element(14);
    // sl.insert_element(15);
    // sl.insert_element(16);
    // sl.insert_element(17);
    // sl.insert_element(18);
    // sl.insert_element(19);
    // sl.insert_element(300);
    // sl.insert_element(1);
    
    // cout << sl.find_element(18) << endl;
    // cout << sl.tail -> val << endl;
    // cout << sl.head -> val << endl;
    // cout << sl.head -> val << endl;
    sl.print();

    return 0;
}