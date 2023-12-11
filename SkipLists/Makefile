CXX = g++
CXXFLAGS = -std=c++17 -Iinclude

.PHONY: all force

search_times: force misc/search_times.cpp
	$(CXX) $(CXXFLAGS) -o bin/search_times misc/search_times.cpp src/SkipList.cpp src/LinkedList.cpp src/BST.cpp

skip_list_tests: force tests/skip_list_tests.cpp
	$(CXX) $(CXXFLAGS) -o bin/skip_list_tests tests/skip_list_tests.cpp src/SkipList.cpp

node_count: force misc/get_node_count_per_level.cpp
	$(CXX) $(CXXFLAGS) -o bin/node_count misc/get_node_count_per_level.cpp src/SkipList.cpp

count_ops: force misc/count_ops.cpp
	$(CXX) $(CXXFLAGS) -o bin/count_ops misc/count_ops.cpp src/SkipList.cpp src/LinkedList.cpp src/BST.cpp

range_query: force misc/range_query_times.cpp
	$(CXX) $(CXXFLAGS) -o bin/range_query misc/range_query_times.cpp src/SkipList.cpp src/BST.cpp src/LinkedList.cpp

try_skiplist: force try_skip_list.cpp
	$(CXX) $(CXXFLAGS) -o bin/skiplist try_skip_list.cpp src/SkipList.cpp

force: ;