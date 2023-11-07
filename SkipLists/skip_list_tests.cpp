#include "SkipList.h"
#include <random>

using namespace std;

SkipList check_build_func_large(bool is_random_skip_list){
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

    SkipList sl;
    if (is_random_skip_list){
        sl.build_skip_list_random(numbers);
    } else {
        sl.build_skip_list_deterministic(numbers);
    }
    return sl;
}

SkipList check_build_func_small(bool is_random_skip_list){
    // Create a vector with small amount of numbers
    std::vector<int> numbers = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    SkipList sl;
    if (is_random_skip_list){
        sl.build_skip_list_random(numbers);
        cout << "SKIP LIST RANDOM SMALL" << endl;
        sl.print();
    } else {
        sl.build_skip_list_deterministic(numbers);
        cout << "SKIP LIST DETERMINISTIC SMALL" << endl;
        sl.print();
    }
    return sl;
}

SkipList check_build_func_one(bool is_random_skip_list){
    // Create a vector with a single element
    std::vector<int> numbers = {1};

    SkipList sl;
    sl.build_skip_list_deterministic(numbers);
    if (is_random_skip_list){
        sl.build_skip_list_random(numbers);
        cout << "SKIP LIST RANDOM ONE" << endl;
        sl.print();
    } else {
        sl.build_skip_list_deterministic(numbers);
        cout << "SKIP LIST DETERMINISTIC ONE" << endl;
        sl.print();
    }
    return sl;
}

bool check_find_element(){
    SkipList sl;

    sl = check_build_func_large(false);
    cout << sl.find_element(5) -> val << endl;
    return true;
}

bool check_build_funcs(){
    // Multiple times just to make sure nothing goes wrong on large vectors
    for (int i = 0; i < 10; i++){
        cout << i << endl;
        check_build_func_large(true);
        check_build_func_large(false);
    }
    check_build_func_small(true);
    check_build_func_small(false);
    check_build_func_one(true);
    check_build_func_one(true);
    return true;
}

int main() {
    check_build_funcs();
    check_find_element();
    return 0;
}
