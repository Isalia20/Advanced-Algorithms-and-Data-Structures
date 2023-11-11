#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "SkipList.h"

using namespace std;

int main(){
    SkipList sl;
    vector<int> foo;
    for (int i = 0; i < 6; i ++){
        foo.push_back(i);
    }
    sl.build_skip_list_deterministic(foo);
    // 0, 1, 2, 3
    sl.print();
    // sl.remove_element(2);
    // sl.remove_element(3);
    // sl.remove_element(1);
    sl.remove_element(5);
    sl.remove_element(4);
    // sl.remove_element(3);
    // sl.remove_element(2);
    // sl.remove_element(1);
    sl.print();
    // sl.print();
    // sl.remove_element(0);
    // sl.print();
    return 0;
}