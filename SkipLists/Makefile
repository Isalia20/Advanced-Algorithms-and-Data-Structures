CXX = g++
CXXFLAGS = -std=c++11

.PHONY: all force

all: skip_list

skip_list: force skip_list.cpp
	$(CXX) $(CXXFLAGS) -o skip_list skip_list.cpp

tests_skip_list: force skip_list_tests.cpp
	$(CXX) $(CXXFLAGS) -o skip_list_tests skip_list_tests.cpp

force: ;