CXX = g++
CXXFLAGS = -std=c++17 -Iinclude

.PHONY: all force

main: force main.cpp
	$(CXX) $(CXXFLAGS) -o bin/main main.cpp src/SkipList.cpp src/LinkedList.cpp src/BST.cpp

skip_list_tests: force tests/skip_list_tests.cpp
	$(CXX) $(CXXFLAGS) -o bin/skip_list_tests tests/skip_list_tests.cpp src/SkipList.cpp

node_count: force misc/get_node_count_per_level.cpp
	$(CXX) $(CXXFLAGS) -o bin/node_count misc/get_node_count_per_level.cpp src/SkipList.cpp

force: ;