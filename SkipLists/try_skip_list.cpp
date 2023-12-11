#include "SkipList.h"


int main(){
    SkipList sl;
    std::vector<int> list = {10, 20, 30, 40, 50, 60};
    sl.build_skip_list_deterministic(list);
    // Try functions below which can be found in SkipList.h
    sl.print();
    return 0;
}