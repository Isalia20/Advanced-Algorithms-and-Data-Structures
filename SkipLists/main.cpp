#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>  // For timing
#include <limits>
#include "SkipList.h"
#include "LinkedList.h"

using namespace std;

int main(){
    SkipList sl;
    LinkedList ll;
    vector<int> list = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

    // for (int i = 0; i < 15; i++){
    //     list.push_back(i);
    // }

    sl.build_skip_list_deterministic(list);
    cout << "DONE BUILDING SKIP LIST" << endl;
    cout << "---------------------------------------------------------------------------------------------------------" << endl;
    ll.build_ll(list);
    cout << "DONE BUILDING LINKED LIST" << endl;
    cout << "---------------------------------------------------------------------------------------------------------" << endl;
    sl.print();
    // cout << sl.go_up_till_end(sl.tail) -> up -> val << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------------" << endl;
    sl.insert_element(-1);
    sl.print();

    // sl.insert_element();
    // ll.print_ll();

    // Start timing here
    // auto start = std::chrono::high_resolution_clock::now();
    // ll.find_element(10 - 1);
    // // Stop timing here
    // auto end = std::chrono::high_resolution_clock::now();
    // // Compute the difference between the two times in milliseconds
    // auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    // cout << "LinkedList find_element took " << duration << " nanoseconds" << endl;
    // cout << "------------------------------------------------------------" << endl;
    
    // // Start timing here
    // start = std::chrono::high_resolution_clock::now();
    // sl.find_element(10 - 1);
    // // Stop timing here
    // end = std::chrono::high_resolution_clock::now();

    // // Compute the difference between the two times in milliseconds
    // duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    // cout << "SkipList find_element took " << duration << " nanoseconds" << endl;
    return 0;
}