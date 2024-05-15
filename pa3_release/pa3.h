#include <stdlib.h>
#include <stdio.h>

typedef struct heap {
    struct heapNode ** arr; //stores heapNodes
    int currSize; //current size of heap
    int maxSize; //max size of heap m*n
    int * pos; //stores position of the nodes in heap arr
} heap;

typedef struct heapNode {
    int compWeight; //combined weight
    int vert; //number vertex this node is in pos array
} heapNode;

// typedef struct Matrix { 
//     struct Node * node; 
//     Matrix ** arr;
// };

typedef struct Node {
    int index;
    int numEdges;
    int weight;
    //int dir; //1 if above, 2 if below, 3 if right, 4 if left

} Node;