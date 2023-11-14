CXX = g++
CXXFLAGS = -std=c++17

.PHONY: all force

all: skip_list tests_skip_list

main: force main.cpp
	$(CXX) $(CXXFLAGS) -o main main.cpp SkipList.cpp LinkedList.cpp

skip_list_tests: force skip_list_tests.cpp
	$(CXX) $(CXXFLAGS) -o skip_list_tests skip_list_tests.cpp SkipList.cpp

force: ;