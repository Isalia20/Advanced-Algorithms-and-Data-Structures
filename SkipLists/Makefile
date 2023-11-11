CXX = g++
CXXFLAGS = -std=c++11

.PHONY: all force

all: skip_list tests_skip_list

skip_list: force main.cpp
	$(CXX) $(CXXFLAGS) -o skip_list main.cpp SkipList.cpp

skip_list_tests: force skip_list_tests.cpp
	$(CXX) $(CXXFLAGS) -o skip_list_tests skip_list_tests.cpp SkipList.cpp

force: ;