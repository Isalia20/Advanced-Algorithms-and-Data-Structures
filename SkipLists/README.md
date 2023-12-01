# Skip Lists implementation

Implementation of a famous data structure skip lists from scratch in C++. Reposistory also includes minimal implementation of BST(Binary search tree) and minimal implementation of Linked List data structures for comparison.

## Running code

If you want to run the code on MacOS or Linux, follow the instructions below:

1. Clone the repo `git clone https://github.com/Isalia20/Advanced-Algorithms-and-Data-Structures.git`

2. Go into the directory `cd SkipLists`

3. Choose whatever function you want to use in main.cpp by using any of the functions used in **SkipLists.h** file

4. Make the main.cpp file with `make main`

5. Run the file `./main`

## Statistical comparison of BST and SkipList

### BST vs SkipList (Searching Last Element)

This chart shows distribution of searching for the last element(e.g in 100 million array, the number we will be searching is 100 million)

![Searching Last Element](comparison_pictures/BST_SkipList_comparison_last_element.png)

### BST vs SkipList (Searching Middle Element)

This chart shows distribution of searching for the middle element(e.g in 100 million array 1/2 the number we will be searching is 50000000)

![Searching Middle Element](comparison_pictures/BST_SkipList_comparison_mid.png)

### BST vs SkipList (Searching Quarter Element)

This chart shows distribution of searching for the quarter element(e.g in 100 million array 1/4 the number we will be searching is 25000000)

![Searching Quarter Element](comparison_pictures/BST_SkipList_comparison_quarter_element.png)

### BST vs SkipList (Searching Third Quarter Element)

This chart shows distribution of searching for the third quarter element(e.g in 100 million array 3/4 the number we will be searching is 75000000)

![Searching Third Quarter Element](comparison_pictures/BST_SkipList_comparison_third_quarter.png)

### SkipList(Random) Level mean and standard deviation

This chart shows mean and standard deviation of the Skiplist data structure if we are building it randomly(i.e. on each element while building we flip a coin and grow one element up if it's 1 and continue onto the next element if it's 0)

![Level Node Count per Level 10k Elements](comparison_pictures/RandomSkipListLevelNodeCount.png)
