#include "SkipList.h"
#include <random>
#include <map>

using namespace std;

void check_build_func_large(){
    // Create a vector with numbers from 1 to 1,000,000
    std::vector<int> numbers;
    for (int i = 1; i <= 1000000; i++) {
        numbers.push_back(i);
    }
    // Create a random device and generator
    std::random_device rd;
    std::mt19937 g(rd());
    // Shuffle the vector
    std::shuffle(numbers.begin(), numbers.end(), g);

    // Random
    {
        SkipList sl;
        sl.build_skip_list_random(numbers);
    }
    // Deterministic
    {
        SkipList sl;
        sl.build_skip_list_deterministic(numbers);
    }
}

void check_build_func_small(){
    // Create a vector with small amount of numbers
    std::vector<int> numbers = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    // Random
    {
        
        SkipList sl;
        sl.build_skip_list_random(numbers);
        cout << "SKIP LIST RANDOM SMALL" << endl;
        sl.print();
    }
    // Deterministic
    {
        SkipList sl;
        sl.build_skip_list_deterministic(numbers);
        cout << "SKIP LIST DETERMINISTIC SMALL" << endl;
        sl.print();
    }
}

void check_build_func_one(){
    // Create a vector with a single element
    std::vector<int> numbers = {1};

    // Deterministic
    {
        
        SkipList sl;
        sl.build_skip_list_deterministic(numbers);
        cout << "SKIP LIST DETERMINISTIC ONE" << endl;
        sl.print();
    }
    // Random
    {
        // Create a vector with a single element
        std::vector<int> numbers = {1};
        SkipList sl;
        sl.build_skip_list_random(numbers);
        cout << "SKIP LIST RANDOM ONE" << endl;
        sl.print();
    }
}

void check_find_element(){
    // Deterministic
    {
        SkipList sl;
        vector<int> list = {0, 1, 2, 3, 4, 5, 6, 7, 8};
        
        sl.build_skip_list_deterministic(list);
        cout << sl.find_element(5) -> val << endl;
        cout << sl.find_element(0) -> val << endl;
        cout << sl.find_element(1) -> val << endl;
        cout << sl.find_element(2) -> val << endl;
        cout << sl.find_element(8) -> val << endl;
    }
    // Random
    {
        SkipList sl;
        vector<int> list = {0, 1, 2, 3, 4, 5, 6, 7, 8};
        
        sl.build_skip_list_random(list);
        cout << sl.find_element(5) -> val << endl;
        cout << sl.find_element(0) -> val << endl;
        cout << sl.find_element(1) -> val << endl;
        cout << sl.find_element(2) -> val << endl;
        cout << sl.find_element(8) -> val << endl;
    }
}

void check_num(int skiplist_val, int expected_val){
    if (skiplist_val != expected_val){
        throw std::runtime_error("INCORRECT TAIL!!! Expected " + std::to_string(expected_val) + " but got " + std::to_string(skiplist_val));
    }
}
void remove_and_check(SkipList* sl, int remove_val, int expected_head, int expected_tail) {
    sl -> remove_element(remove_val);
    check_num(sl -> head -> val, expected_head);
    check_num(sl -> tail -> val, expected_tail);
}

void check_lots_of_removals(SkipList* sl) {
    remove_and_check(sl, 100, 1, 99);
    remove_and_check(sl, 99, 1, 98);
    remove_and_check(sl, 98, 1, 97);
    remove_and_check(sl, 50, 1, 97);
    remove_and_check(sl, 10, 1, 97);
    remove_and_check(sl, 25, 1, 97);
    remove_and_check(sl, 1, 2, 97);
    remove_and_check(sl, 2, 3, 97);
    remove_and_check(sl, 3, 4, 97);
}

void check_remove_all_elements(SkipList* sl){
    while (sl -> tail != NULL){
        try {
            sl -> remove_element(sl -> head -> val);
        } catch (const std::runtime_error& e){
            cerr << e.what() << endl;
            break;
        }
    }
}

void check_remove_elements(){
    vector<int> numbers;
    for (int i = 1; i <= 100; i++) {
        numbers.push_back(i);
    }
    
    // Random
    {
        SkipList* sl = new SkipList;
        sl -> build_skip_list_random(numbers);
        check_lots_of_removals(sl);
        check_remove_all_elements(sl);
    }

    // Deterministic
    {
        SkipList* sl = new SkipList;
        sl -> build_skip_list_deterministic(numbers);
        check_lots_of_removals(sl);
        check_remove_all_elements(sl);
    }
}


void check_find_closest_elements(){
    SkipList sl;
    vector<int> list = {0, 1, 2, 3, 4, 5, 6, 7, 8};

}

void check_build_funcs(){
    // Multiple times just to make sure nothing goes wrong on random large vectors
    for (int i = 0; i < 10; i++){
        cout << i << endl;
        check_build_func_large();
    }
    check_build_func_small();
    check_build_func_one();
}

int main(){
    check_build_funcs();
    check_find_element();
    check_remove_elements();
    return 0;
}
