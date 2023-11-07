#include "SkipList.h"
#include <random>

void check_build_func_deterministic(){
    // Create a vector with numbers from 1 to 1,000,000
    std::vector<int> numbers;
    for (int i = 1; i <= 1000000; ++i) {
        numbers.push_back(i);
    }

    // Create a random device and generator
    std::random_device rd;
    std::mt19937 g(rd());
    
    // Shuffle the vector
    std::shuffle(numbers.begin(), numbers.end(), g);

    // Print first 10 numbers from the shuffled vector
    SkipList sl;
    sl.build_skip_list_deterministic(numbers);
}

void check_build_func_random(){
    // Create a vector with numbers from 1 to 1,000,000
    std::vector<int> numbers;
    for (int i = 1; i <= 1000000; ++i) {
        numbers.push_back(i);
    }

    // Create a random device and generator
    std::random_device rd;
    std::mt19937 g(rd());
    
    // Shuffle the vector
    std::shuffle(numbers.begin(), numbers.end(), g);

    // Print first 10 numbers from the shuffled vector
    SkipList sl;
    sl.build_skip_list_random(numbers);
}

bool check_build_funcs(){
    for (int i = 0; i < 10; i++){
        cout << i << endl;
        check_build_func_deterministic();
        check_build_func_random();
    }
    return true;
}

int main() {
    check_build_funcs();

    return 0;
}
