#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include "pa3.h"

short * readInBinary(char * filename, short * nrows, short * ncols, char * outfile) { //including all vertices plus 1 extra ending node
    FILE * fp = fopen(filename, "rb");
    FILE * ofp = fopen(outfile, "w");
    fread(nrows, sizeof(short), 1, fp);
    fread(ncols, sizeof(short), 1, fp);
    fprintf(ofp, "%hd %hd\n", *nrows, *ncols);
    int nums = ((*nrows) * (*ncols));
    short * arr = malloc((nums+1) * sizeof(short));
    printf("hello\n");
    for(int n = 0; n < *nrows; n++) {
        for(int c = 0; c < *ncols; c++) {
            fread(arr+(n * (*ncols) + c), sizeof(short), 1, fp);
            if(c == *ncols - 1) {
                fprintf(ofp, "%hd", arr[n * (*ncols) + *ncols]);
            }
            else {
                fprintf(ofp, "%hd ", arr[n * (*ncols) + *ncols]);
            }
            
        }
        fprintf(ofp, "\n");
    }
    // for(int i = 0; i < nums; i++){
    //     fread((arr+i), sizeof(short), 1, fp);
    //     //fprintf(ofp, )
    //     printf("%hu\n", arr[i]);
    // }
    arr[nums] = 0; //imaginary node
    fclose(fp);
    fclose(ofp);
    return arr;
}

// short ** readInBinaryMatrix(char * filename, short * rows, short * cols) {
//     FILE * fp = fopen(filename, "rb");
//     fread(rows, sizeof(short), 1, fp);
//     fread(cols, sizeof(short), 1, fp);
//     int nums = ((*rows) * (*cols));
//     //short * arr = malloc(cols*sizeof(short));
//     short ** nestedArr = malloc(*rows * sizeof(short*));
//     for(int i = 0; i < *rows; i++) {
//         nestedArr[i] = malloc(*cols * sizeof(short));
//     }
//     printf("hello\n");
//     for(int i = 0; i < *rows; i++) {
//         for(int k = 0; k < *cols; k++) {
//             fread(&nestedArr[i][k], sizeof(short), 1, fp);
//             printf("%hu\n", nestedArr[i][k]);
//         }
//     }
//     fclose(fp);
//     return nestedArr;
// }


void freeMatrix(Node ** matrix, short nrows, short ncols) {
    for(int i = 0; i < (nrows * ncols)+1; i++) {
        free(matrix[i]);
    }
    free(matrix);

}

int findCol(int index, short ncols) {
    return index % (ncols);
}

int findRow(int index, short ncols) {
    //int pos = index + 1;
    int rowLoc = index / (ncols);
    return rowLoc;
}

int retIndex(int rowVal, int colVal, short ncols) { //returns index given coords
    return ((rowVal*ncols) + colVal);
}

int isValidNode(int rowVal, int colVal, short nrows, short ncols) { //checks if a given node is valid given coords
    if(rowVal < 0 || rowVal >= nrows) {
        return 0;
    }
    if(colVal < 0 || colVal >= ncols) {
        return 0;
    }
    return 1;
}

int numEdges(short nrows, short ncols, int index) { //returns number of edges for a given node
    int colVal = findCol(index, ncols);
    int rowVal = findRow(index, ncols);
    if(colVal == 0 || colVal == ncols - 1) { //corner node
        // if(rowVal == 0 || rowVal == nrows - 1) {
        //     return 2; //corner node
        // }
        if(rowVal == 0) {
            return 2; //corner node in top row
        }
        return 3; //edge node or bottom row corner node
    }
    // if(rowVal == 0 || rowVal == nrows - 1) {
    //     return 3; //edge node
    // }
    if(rowVal == 0) {
        return 3; //top row
    }
    //if bottom row must connnect to destination node so has extra edge
    return 4; //middle node or bottom row node
}







//need to build a adjacency list/matrix
// Matrix * adjacencyList(FILE * fp, short rows, short cols) {
//     fread(&rows, sizeof(short), 1, fp);
//     fread(&cols, sizeof(short), 1, fp);
//     short nums = (rows) * (cols);
//     Matrix * adjList = malloc((nums * sizeof(Matrix));
//     for(int i = 0; i < *rows; i++) {
//         //now need to create individual lists for each node
//         adjList->node = malloc( * sizeof(Node));
//     }
// }

//returns what type of corner it is (top left=0, top right = 1, etc)
int typeCorner(int index, short nrows, short ncols) { 
    if(index == 0) {
        return 0; //top left
    }
    if(index == ncols - 1) {
        return 1; //top right
    }
    if(index == (nrows-1) * ncols) {
        return 2; //bottom left
    }
    if(index == (nrows * ncols) - 1) {
        return 3; //bottom right
    }
    return -1; //should not reach here
}

//returns type of side node (top = 0, left = 1, right = 2, bottom = 3)
int typeSide(int index, short nrows, short ncols) {
    if(index < ncols) {
        return 0; //top side
    }
    if(index % ncols == 0) {
        return 1; //left side
    }
    if((index - (ncols - 1)) % ncols == 0) {
        return 2; //right side
    }
    if(index >= (nrows - 1) * ncols && index < (nrows * ncols)) {
        return 3; //bottom side
    }
    return -1;
}

//returns index of node above
int goUp(int index, short ncols) {
    return index - ncols;
}
int goDown(int index, short ncols) {
    return index + ncols;
}
int goRight(int index) {
    return index + 1;
}
int goLeft(int index) {
    return index - 1;
}

//figure out how to get adjacent nodes and store them
//need array of shorts because need to read into the future nodes for adj list

//ADDING EXTRA 1 TO ALL INDEXES MUST SUBTRACT 1 WEHN FINDING PATH

//don't add source node just use for loop to iterate through different starting nodes (top row are starting nodes)
//can remove adding 1 to indices.
Node ** adjList(short * arr, short nrows, short ncols) {
    int nums = ((nrows) * (ncols));
    Node ** adjMatrix = malloc((nums+1) * sizeof(Node*));
    //need to add src node and connect to top row
    //need to add destination node and have every node in bottom row connected to it
    //no longer need imaginary top node
    // adjMatrix[0] = malloc(ncols * sizeof(Node));

    // //adjMatrix[5] == adjMatrix[5][0];

    // adjMatrix[0]->numEdges = 5;
    // for(int l = 0; l < ncols; l++) {
    //     adjMatrix[0][l].index = l; //creating edges from source node to top row
    //     adjMatrix[0][l].weight = arr[0];
    // }
    for(int i = 0; i < nums; i++){ 
        
        //adjMatrix[i]->numEdges = numEdges(nrows, ncols, i);
        
        int nEdges = numEdges(nrows, ncols, i);
        adjMatrix[i] = malloc(nEdges * sizeof(Node));
        adjMatrix[i]->numEdges = nEdges;
        //adjMatrix[i]->weight = arr[i];
        int rowVal = findRow(i, ncols);
        int colVal = findCol(i, ncols);
        //go up first
        int loc = 0;
        int up = retIndex(rowVal-1, colVal, ncols);
        if(isValidNode(rowVal-1, colVal, nrows, ncols)) {
            adjMatrix[i][loc].index = up;
            adjMatrix[i][loc].weight = arr[i];
           // adjMatrix[i][loc].dir = 1;
            loc++;
        }
        if(i >=((nrows-1) * ncols)) { //special case for bottom row since need to connect to destination node
            adjMatrix[i][loc].index = nums; //should be nums
            adjMatrix[i][loc].weight = arr[i];
            //adjMatrix[i][loc].dir = 2;
            printf("\n%d\n", i);
            loc++;
        }
        else {
            int down = retIndex(rowVal + 1, colVal, ncols);
            if(isValidNode(rowVal+1, colVal, nrows, ncols)) {
                adjMatrix[i][loc].index = down;
                adjMatrix[i][loc].weight = arr[i];
               // adjMatrix[i][loc].dir = 2;
                loc++;
                if(i == 0) {
                    fprintf(stderr, "\nhello%d\n", adjMatrix[0][0].index);
                }
            }

        }
        int right = retIndex(rowVal, colVal+1, ncols);
        if(isValidNode(rowVal, colVal+1, nrows, ncols)) {
            adjMatrix[i][loc].index = right;
            adjMatrix[i][loc].weight = arr[i];
           // adjMatrix[i][loc].dir = 3;
            loc++;
        }
        int left = retIndex(rowVal, colVal - 1, ncols);
        if(isValidNode(rowVal,colVal -1, nrows, ncols)) {
            adjMatrix[i][loc].index = left;
            adjMatrix[i][loc].weight = arr[i];
            //adjMatrix[i][loc].dir = 4;
            if(i == 18){
                printf("18: %d, %d", findRow(left, ncols), findCol(left, ncols));
            }
            loc++;
        }
        assert(loc == nEdges);
    }
    //now add destination node
    adjMatrix[nums] = malloc(sizeof(Node));
    adjMatrix[nums]->index = nums;
    adjMatrix[nums]->numEdges = 0;
    adjMatrix[nums]->weight = arr[nums];
    //adjMatrix[i][loc].dir = 0;
    return adjMatrix;
}

int findParent(int childInd) {
    if(childInd == 0) {
        return -1;
    }
    return (childInd-1) / 2;
}
int findLeft(int parInd, int maxsize) {
    if (2*parInd + 1 >= maxsize) {
        return -1;
    }
    return (2*parInd + 1);
}
int findRight(int parInd, int maxsize) {
    if (2*parInd + 2 >= maxsize) {
        return -1;
    }
    return (2*parInd + 2);
}



heap * initHeap(int maxsize) {
    heap * minHeap = malloc(sizeof(heap));
    minHeap->pos = malloc(maxsize * sizeof(int));
    minHeap->currSize = 0;
    minHeap->maxSize = maxsize;
    minHeap->arr = malloc(maxsize * sizeof(heapNode*));
    return minHeap;
}

void freeHeap(heap * minHeap) {
    if(minHeap == 0) {
        return;
    }
    //need to add something here
    free(minHeap);
}

void swapNodes(heapNode ** n1, heapNode ** n2) {
    heapNode * temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}

heapNode * createNode(int vert, short time) {
    heapNode * node = malloc(sizeof(heapNode));
    node->vert = vert;
    node->compWeight = time;
    return node;
    //we don't have to do any shifting 
    //because anytime insert a new node
    //compWeight will be infinity
}


// heap * insertNode(heap * minHeap, int index) { //index is index of newNode value
//     if(minHeap->currSize == minHeap->maxSize) {
//         fprintf(stderr, "exceeded max Capacity Size\n");
//         return NULL; //heap at capacity can't insert new Node something went wrong
//     }
//     //now inserting new Node
//     //expand size of arr
//     minHeap->arr[minHeap->currSize] = newNode;
//     minHeap->currSize++;
//     //upward sifting if necessary
//     //get index of current element
//     int currInd = minHeap->currSize - 1;
//     while(currInd > 0 && minHeap->arr[findParent(currInd)] > minHeap->arr[curr]) {
//         //Only want to swap until we reach root and parent is greater than child
//         short swap = minHeap->arr[findParent(currInd)];
//         minHeap->arr[findParent(currInd)] = minHeap->arr[currInd];
//         minHeap->arr[curr] = swap;
//         int currInd = findParent(currInd); 
//     }
//     return minHeap;
// }



// heap * deleteNode(heap * minHeap, int nodeInd) {
//     //deleting random node. Can shift this node up to root and then use root delete
//     minHeap->arr[nodeInd] = minHeap->arr[0] - 1;
//     int currInd = nodeInd;
//     while (currInd > 0) { //guaranteed to be a parent
//         short swap = minHeap->arr[findParent(currInd)];
//         minHeap->arr[findParent(currInd, minHeap->maxSize)] = minHeap->arr[currInd];
//         minHeap->arr[currInd] = swap;
//         currInd = findParent(currInd, minHeap->maxSize);
//     }
//     minHeap = deleteRoot(minHeap);
//     return minHeap;


// }

void down_heapify(heap * minHeap, int currInd) { //now using double pointers so no need to return anything
    // printf("CURRIND: %d, MAXSIZE %d", currInd, minHeap->currSize);
    // for(int k = 0; k< minHeap->currSize; k++) {
    //                 printf("Ind: %d, Weights: %hu\n", minHeap->arr[k]->vert, minHeap->arr[k]->compWeight);
    // }
    // printf("\n-------------------\n");
    int leftNode = findLeft(currInd, minHeap->currSize);
    int rightNode = findRight(currInd, minHeap->currSize);
    int temp = currInd;
    if (leftNode!=-1 && minHeap->arr[leftNode]->compWeight < minHeap->arr[temp]->compWeight) {
        temp = leftNode; //leftNode val is smaller than currNode
    }
    if(rightNode != -1 && minHeap->arr[rightNode]->compWeight < minHeap->arr[temp]->compWeight) {
        temp = rightNode;
    }
    //now do swaps if necessary
    if(temp != currInd) {
        heapNode* smallNode = minHeap->arr[temp];
        heapNode * curr = minHeap->arr[currInd];
        //swap the position indexes in pos array to access the nodes
        minHeap->pos[smallNode->vert] = currInd;
        minHeap->pos[curr->vert] = temp;
        //now need to swap actual nodes in arr
        swapNodes(&minHeap->arr[temp], &minHeap->arr[currInd]);
        //might need to switch this
        down_heapify(minHeap, temp);
    }   
}

bool inHeap(heap * minHeap, int loc) {
    if(minHeap->pos[loc] < minHeap->currSize) {
        return true;
    }
    return false;
}

heapNode * deleteRoot(heap * minHeap) {
    if(minHeap->currSize == 0) {
        fprintf(stderr, "heap is empty can't remove anything\n");
        return NULL; //heap not valid
    }
    //updating root with last element (this could be the not full row)
    heapNode * root = minHeap->arr[0];
    minHeap->arr[0] = minHeap->arr[(minHeap->currSize) - 1];
    minHeap->currSize -= 1; //reducing size
    //now must check to make sure still a min heap if not must shift node down
    //down heapify
    down_heapify(minHeap, 0);
    return root;
}

void changeTimes(heap * minHeap, int vertInd, short weight) {
    int posInd = minHeap->pos[vertInd]; //getting index of node in pos array
    minHeap->arr[posInd]->compWeight = weight; //updating weight of node

    while(posInd > 0 && minHeap->arr[posInd]->compWeight < minHeap->arr[findParent(posInd)]->compWeight) {
        minHeap->pos[minHeap->arr[posInd]->vert] = findParent(posInd);
        minHeap->pos[minHeap->arr[findParent(posInd)]->vert] = posInd;

        // printf("\nBefore Swap: \nPosInd: %d, VertNum, %d Weights: %hu\n", posInd, minHeap->arr[posInd]->vert, minHeap->arr[posInd]->compWeight);
        // printf("ParInd: %d, VertNum, %d Weights: %hu\n", findParent(posInd), minHeap->arr[findParent(posInd)]->vert, minHeap->arr[findParent(posInd)]->compWeight);

        swapNodes(&(minHeap->arr[posInd]), &(minHeap->arr[findParent(posInd)]));

        // printf("\nAfter Swap\nPosInd: %d, VertNum, %d Weights: %hu\n", posInd, minHeap->arr[posInd]->vert, minHeap->arr[posInd]->compWeight);
        // printf("ParInd: %d, VertNum, %d Weights: %hu\n\n", findParent(posInd), minHeap->arr[findParent(posInd)]->vert, minHeap->arr[findParent(posInd)]->compWeight);

        posInd = findParent(posInd);
    }
    // for(int i = 0; i < minHeap->currSize; i++) {
    //     printf("Ind: %d, Weights: %hu\n", minHeap->arr[i]->vert, minHeap->arr[i]->compWeight);
    // }
}

void printArr(short dist[], int n, short ncols)
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d: (%d,%d) \t\t %hu\n", i, findRow(i, ncols), findCol(i, ncols), dist[i]);
}

short * implDijkstra(Node ** graph, int source, short ncols, short nrows, int ** path) { //source is 0 node
    //path[0] = source;
    //int pathloc = 1;
    int numVerts = (ncols * nrows) + 1; //might not need to include ending node (nums + 1)
    //numVerts is V
    heap * minHeap = initHeap(numVerts);
    //short accTimes[numVerts]; //dist arr
    short * accTimes = malloc(numVerts * sizeof(short));
    //int path[ncols * nrows];
    for(int i = 0; i < numVerts; i++) {
        accTimes[i] = SHRT_MAX;
        minHeap->arr[i] = createNode(i, accTimes[i]);
        minHeap->pos[i] = i;
    }
    //minHeap->arr[source] = createNode(source, accTimes[source]);
    //extract source node first
    minHeap->pos[source] = source;
    accTimes[source] = 0;
    changeTimes(minHeap, source, accTimes[source]);

    minHeap->currSize = numVerts;
    //int location = 0;

    while(minHeap->currSize != 0) {
        // printf("\nBefore Extracting:\n");
        // for(int k = 0; k< minHeap->currSize; k++) {
        //             printf("Ind: %d, Weights: %hu\n", minHeap->arr[k]->vert, minHeap->arr[k]->compWeight);
        // }
        // printf("----------------\n");
        heapNode * minNode = deleteRoot(minHeap); //everytime i extract a root i will save it in order to determine the path
        

        // printf("------TESTING: %d-------\n%d", minNode->vert, minHeap->currSize);
        // printf("\nAfter Extracting:\n");
        // for(int k = 0; k< minHeap->currSize; k++) {
        //             printf("Ind: %d, Weights: %hu\n", minHeap->arr[k]->vert, minHeap->arr[k]->compWeight);
        // }
        // printf("----------------\n");
        int vert = minNode->vert; //getting vertex number of extracted node

        for(int i=0; i < graph[vert]->numEdges; i++) { //now iterate through all adjacent nodes
            short temp = accTimes[vert];
            int l = graph[vert][i].index;
            short weight = graph[vert]->weight;
            if(inHeap(minHeap, graph[vert][i].index) && accTimes[vert] != SHRT_MAX && (graph[vert]->weight + accTimes[vert]) < accTimes[graph[vert][i].index]) {
                accTimes[graph[vert][i].index] = accTimes[vert] + graph[vert]->weight;
                //location++;
                changeTimes(minHeap, graph[vert][i].index, accTimes[graph[vert][i].index]);
                path[0][graph[vert][i].index] = vert;
                int h = path[0][graph[vert][i].index];
                printf("%d\n",h);

                //down_heapify(minHeap, 0);
                // printf("\n\n");
                // for(int k = 0; k< minHeap->currSize; k++) {
                //     printf("Ind: %d, Weights: %hu\n", minHeap->arr[k]->vert, minHeap->arr[k]->compWeight);
                // }
                // printf("\n\n");
            }
        }
        //pathloc++;
    }
    return accTimes;
    //printArr(accTimes, numVerts, ncols);
}

int main(int argc, char ** argv) {
    short nrows;
    short ncols;
    short * arr = readInBinary("examples/4_5.b", &nrows, &ncols, "outfile.txt");
    int nums = ncols*nrows;
    Node ** adjMatrix = adjList(arr, nrows, ncols);
    //freeMatrix(arr, rows);
    fprintf(stderr, "---------------------------------------------\n");
    //fprintf(stderr, "ind: %d Weight: %hu", i, arr[i]);
    // fprintf(stderr, "Source Node: coord: (-1, -1)");
    // for(int j=0; j < ncols; j++) {
    //     fprintf(stderr, "->(%d, %d)", findRow(adjMatrix[0][j].index, ncols), findCol(adjMatrix[0][j].index, ncols));
    // }
    // fprintf(stderr, "\n");
    for(int i = 0; i < (nums); i++) {
        fprintf(stderr, "Start Node:%d Edges ", adjMatrix[i]->numEdges);
        //fprintf(stderr, "%hu", arr[adjMatrix[i]-index]);
        fprintf(stderr, "coord: (%d, %d) Weight: %hu", findRow(i, ncols), findCol(i, ncols), arr[i]);
        for(int k = 0; k < adjMatrix[i]->numEdges; k++) {
            fprintf(stderr, "->(%d, %d)", findRow(adjMatrix[i][k].index, ncols), findCol(adjMatrix[i][k].index, ncols));
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "Start Node:%d Edges ", adjMatrix[nums]->numEdges);
    fprintf(stderr, "coord: (%d, %d)\n", findRow(adjMatrix[nums][0].index, ncols), findCol(adjMatrix[nums][0].index, ncols));
    free(arr);
    //fprintf(stderr, "------------\n%d\n", adjMatrix[2][0].index);
    int ** path = malloc(sizeof(int*));
    *path = malloc((nums+1) * sizeof(int));
    //int * path = calloc(nums, sizeof(int));
    for(int i = 0; i < nums; i++) {
        path[0][i] = -1;
        //printf("inds: %d\n", path[i]);
    }
    short * times = malloc(ncols * (nums+1) * sizeof(short));
    times = implDijkstra(adjMatrix, 0, ncols, nrows, path);
    for(int i = 1; i < ncols; i++) {
        short * tempTimes = implDijkstra(adjMatrix, i, ncols, nrows, path);
        if(tempTimes[nums] < times[nums]) {

        }
        printf("\nStarting new path:\n");
    }
    
    //printArr(times, nums+1, ncols);

    printf("%d\n", path[0][0]);
    int place = nums;
    for(int i = 0; i <  nums+1; i++) {
        printf("from: %d, to: %d\n", path[0][i], i);
    }
    int place2 = 0;
    // while(path[place] != nums) {
    //     printf("Forward Traversal: from: %d, to %d\n", place, path[place]);
    //     place = path[place];
    // }
    int * pathForward = malloc(nums * sizeof(int));
    while(path[0][place] != -1) {
        printf("Backwards Traversal: from: %d, to %d\n", place, path[0][place]);
        pathForward[place2] = path[0][place];
        place2++;
        place = path[0][place];

    }
    for(int i = place2 - 1; i >= 0; i--) {
        printf("Path: %d\n", pathForward[i]);
    }
    // while(place != (nums)) {
    //     printf("inds: %d\n", path[place]);
    //     place = path[place];
    // }
    
    
    
    //implDijkstra(adjMatrix, 0, ncols, nrows);
    //implDijkstra(adjMatrix, 1, ncols, nrows);
    freeMatrix(adjMatrix, nrows, ncols);
    return 0;
}