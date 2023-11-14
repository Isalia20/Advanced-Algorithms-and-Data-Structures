#include "SkipList.h"
#include <random>
#include <map>
#include <cassert>


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
        cout << "--------------------------------------------------------------------" << endl;
    }
    // Deterministic
    {
        SkipList sl;
        sl.build_skip_list_deterministic(numbers);
        cout << "SKIP LIST DETERMINISTIC SMALL" << endl;
        sl.print();
        cout << "--------------------------------------------------------------------" << endl;
    }
}

void check_build_func_one(){
    // Create a vector with a single element
    std::vector<int> numbers = {1};

    // Deterministic
    {
        
        SkipList sl;
        sl.build_skip_list_deterministic(numbers);
        cout << "SKIP LIST DETERMINISTIC ONE ELEMENT" << endl;
        sl.print();
        cout << "--------------------------------------------------------------------" << endl;
    }
    // Random
    {
        // Create a vector with a single element
        std::vector<int> numbers = {1};
        SkipList sl;
        sl.build_skip_list_random(numbers);
        cout << "SKIP LIST RANDOM ONE ELEMENT" << endl;
        sl.print();
        cout << "--------------------------------------------------------------------" << endl;
    }
}

void check_find_element(){
    // Deterministic
    {
        SkipList sl;
        vector<int> list = {0, 1, 2, 3, 4, 5, 6, 7, 8};
        
        sl.build_skip_list_deterministic(list);
        assert(sl.find_element(5) -> val == 5);
        assert(sl.find_element(0) -> val == 0);
        assert(sl.find_element(1) -> val == 1);
        assert(sl.find_element(2) -> val == 2);
        assert(sl.find_element(8) -> val == 8);
        cout << "Deterministic check_find_element passed" << endl;
        cout << "--------------------------------------------------------------------" << endl;
    }
    // Random
    {
        SkipList sl;
        vector<int> list = {0, 1, 2, 3, 4, 5, 6, 7, 8};
        
        sl.build_skip_list_random(list);
        assert(sl.find_element(5) -> val == 5);
        assert(sl.find_element(0) -> val == 0);
        assert(sl.find_element(1) -> val == 1);
        assert(sl.find_element(2) -> val == 2);
        assert(sl.find_element(8) -> val == 8);
        cout << "Random check_find_element passed" << endl;
        cout << "--------------------------------------------------------------------" << endl;
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

enum class RemovalType {
    Start,
    Mid,
    End
};

std::string generateError(const std::string& location, const std::string& node) {
    return "check find closest element isn't working correctly. Bug in finding the closest element for the " + location + " element (" + node + " node)";
}


void check_find_closest_elements_nodes(vector<SkipList::Node*> nodes, RemovalType type_of_removal){
    if (nodes.size() < 2) {
        throw std::runtime_error("Insufficient number of nodes.");
    }
    switch (type_of_removal) {
        case RemovalType::Start:
            if (nodes[0] != NULL){
               throw std::runtime_error(generateError("start", "left"));
            }
            if (nodes[1] -> val != 1){
                throw std::runtime_error(generateError("start", "right"));
            }
            break;
        case RemovalType::Mid:
            if (nodes[0] == NULL || nodes[0] -> val != 4){
                throw std::runtime_error(generateError("mid", "left"));
            }
            if (nodes[1] == NULL || nodes[1] -> val != 6){
                throw std::runtime_error(generateError("mid", "right"));
            }
            break;
        case RemovalType::End:
            if (nodes[0] == NULL || nodes[0] -> val != 7){
                throw std::runtime_error(generateError("end", "left"));
            }
            if (nodes[1] != NULL){
                throw std::runtime_error(generateError("end", "right"));
            }
            break;
    }
}

void check_find_closest_elements(bool is_random) {
    // Lambda function to handle the common operations
    auto process = [](SkipList& sl, int end_val, int mid_val, int start_val){
        vector<SkipList::Node*> nodes_end = sl.find_closest_elements(end_val); // end of the skip list 
        vector<SkipList::Node*> nodes_mid = sl.find_closest_elements(mid_val); // middle of the skip list 
        vector<SkipList::Node*> nodes_start = sl.find_closest_elements(start_val); // start of the skip list

        check_find_closest_elements_nodes(nodes_start, RemovalType::Start); // check start
        check_find_closest_elements_nodes(nodes_mid, RemovalType::Mid); // check mid
        check_find_closest_elements_nodes(nodes_end, RemovalType::End); // check end
    };

    SkipList sl;
    vector<int> list = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    
    if (is_random) {
        sl.build_skip_list_random(list);
    } else {
        sl.build_skip_list_deterministic(list);
    }

    process(sl, 8, 5, 0);
    cout << "--------------------------------------------------------------------" << endl;
    cout << "find_closest_elements check has passed" << endl;
    cout << "--------------------------------------------------------------------" << endl;
}

void insert_and_check(SkipList* sl, int element, const int expected_head_val, const int expected_tail_val, const std::string& error_message) {
    sl->insert_element(element);
    if (sl->tail->val != expected_tail_val || sl->head->val != expected_head_val) {
        throw std::runtime_error(error_message);
    }
}


void insert_elements(SkipList* sl){
    std::map<int, std::string> insert_element_error_map;
    insert_element_error_map[0] = "Something is wrong with inserting elements at the start!!!";
    insert_element_error_map[1] = "Something is wrong with inserting elements in the middle!!!";
    insert_element_error_map[2] = "Something is wrong with inserting elements at the end!!!";
    insert_element_error_map[3] = "Something is wrong with inserting elements!!!";

    // insert in end elements
    insert_and_check(sl, 800, 0, 800, insert_element_error_map[2]);
    insert_and_check(sl, 900, 0, 900, insert_element_error_map[2]);
    insert_and_check(sl, 1000, 0, 1000, insert_element_error_map[2]);
    
    // insert in mid elements
    insert_and_check(sl, 25, 0, 1000, insert_element_error_map[1]);
    insert_and_check(sl, 225, 0, 1000, insert_element_error_map[1]);
    insert_and_check(sl, 375, 0, 1000, insert_element_error_map[1]);
    
    // insert in the start
    insert_and_check(sl, -1, -1, 1000, insert_element_error_map[0]);
    insert_and_check(sl, -50, -50, 1000, insert_element_error_map[0]);
    insert_and_check(sl, -100, -100, 1000, insert_element_error_map[0]);
    
    // Insert in middle/start/end
    insert_and_check(sl, 230, -100, 1000, insert_element_error_map[3]);
    insert_and_check(sl, -120, -120, 1000, insert_element_error_map[3]);
    insert_and_check(sl, 1200, -120, 1200, insert_element_error_map[3]);
}

void check_insert_elements(){
    
    SkipList sl_deterministic;
    SkipList sl_random;

    vector<int> list = {0, 1, 10, 100, 150, 200, 250, 300, 350, 400, 450, 500};
    sl_deterministic.build_skip_list_deterministic(list);
    sl_random.build_skip_list_random(list);
    insert_elements(&sl_deterministic);
    insert_elements(&sl_random);
}

void remove_elements(SkipList* sl){
    // Remove from the middle
    sl -> remove_element(100);
    sl -> remove_element(150);
    sl -> remove_element(250);

    // Remove from the start, remove all elements till we go down to the original list's head and remove that as well
    sl -> remove_element(-120);
    sl -> remove_element(-100);
    sl -> remove_element(-50);
    sl -> remove_element(-1);
    sl -> remove_element(0);

    // Remove from the end, remove the tail till we go down to the original list's tail and remove that as well
    sl -> remove_element(1200);
    sl -> remove_element(1000);
    sl -> remove_element(900);
    sl -> remove_element(800);
    sl -> remove_element(500);
}


void check_insert_remove_elements(){
    vector<int> list = {0, 1, 10, 100, 150, 200, 250, 300, 350, 400, 450, 500};
    SkipList sl_deterministic;
    SkipList sl_random;

    sl_deterministic.build_skip_list_deterministic(list);
    sl_random.build_skip_list_random(list);

    insert_elements(&sl_deterministic);
    insert_elements(&sl_random);
    
    remove_elements(&sl_deterministic);
    remove_elements(&sl_random);
    cout << "insert remove elements checks have passed!" << endl;
    cout << "--------------------------------------------------------------------" << endl;
}


void check_build_funcs(){
    // Multiple times just to make sure nothing goes wrong on random large vectors
    for (int i = 0; i < 10; i++){
        check_build_func_large();
    }
    check_build_func_small();
    check_build_func_one();
}

int main(){
    check_build_funcs();
    check_find_element();
    check_remove_elements();
    check_find_closest_elements(true);
    check_find_closest_elements(false);
    check_insert_elements();
    check_insert_remove_elements();
    cout << "ALL TESTS HAVE PASSED!!!" << endl;
    return 0;
}
