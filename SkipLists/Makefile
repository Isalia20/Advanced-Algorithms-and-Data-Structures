CXX = g++
CXXFLAGS = -std=c++17

.PHONY: all force

all: skip_list tests_skip_list

main: force main.cpp
	$(CXX) $(CXXFLAGS) -o main main.cpp SkipList.cpp LinkedList.cpp BST.cpp

skip_list_tests: force skip_list_tests.cpp
	$(CXX) $(CXXFLAGS) -o skip_list_tests skip_list_tests.cpp SkipList.cpp

bst: force bst.cpp
	$(CXX) $(CXXFLAGS) -o bst BST.cpp

node_count: force get_node_count_per_level.cpp
	$(CXX) $(CXXFLAGS) -o node_count get_node_count_per_level.cpp SkipList.cpp

force: ;