#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include "pa3.h"

short * readInBinary(char * filename, short * nrows, short * ncols, char * outfile) { //including all vertices plus 1 extra ending node
    FILE * fp = fopen(filename, "rb");
    if(fp == NULL) {
        fprintf(stderr, "Can't Open File\n");
        return NULL;
    }
    FILE * ofp = fopen(outfile, "w");
    if(ofp == NULL) {
        fprintf(stderr, "Can't Open File\n");
        return NULL;
    }
    fread(nrows, sizeof(short), 1, fp);
    fread(ncols, sizeof(short), 1, fp);
    fprintf(ofp, "%hd %hd\n", *nrows, *ncols);
    //     printf("hello\n");
    //     return NULL;
    // }

    int nums = ((*nrows) * (*ncols));
    short * arr = malloc((nums+2) * sizeof(short));
    //printf("hello\n");
    arr[0] = 0; //source node weight
    for(int n = 0; n < *nrows; n++) {
        for(int c = 0; c < *ncols; c++) {
            //fread(arr[n], sizeof(short), 1, fp);
            fread(&arr[(n * (*ncols) + c)+1], sizeof(short), 1, fp);
            if(c == *ncols - 1) {
                fprintf(ofp, "%hd", arr[(n * (*ncols) + c)+1]);
            }
            else {
                fprintf(ofp, "%hd ", arr[(n * (*ncols) + c)+1]);
            }
            
        }
        fprintf(ofp, "\n");
    }
    // for(int i = 0; i < nums; i++){
    //     fread((arr+i), sizeof(short), 1, fp);
    //     //fprintf(ofp, )
    //     printf("%hu\n", arr[i]);
    // }
    arr[nums+1] = 0; //imaginary node
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
    for(int i = 0; i < (nrows * ncols)+2; i++) {
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
    if(colVal == 0 || colVal == ncols - 1) { //side node
        // if(rowVal == 0 || rowVal == nrows - 1) {
        //     return 2; //corner node
        // }
        if(rowVal == nrows-1) {
            return 2; //corner node in bottom row
        }

        return 3; //edge node or top row corner node
    }
    // if(rowVal == 0 || rowVal == nrows - 1) {
    //     return 3; //edge node
    // }
    if(rowVal == nrows-1) { 
        return 3; //bottom row middle nodes
    }
    //if bottom row must connnect to destination node so has extra edge
    return 4; //middle node or top row middle node
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
    Node ** adjMatrix = malloc((nums+2) * sizeof(Node*));
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
        if (rowVal == 0) { //if its in the top row
            adjMatrix[i][loc].index = nums;
            adjMatrix[i][loc].weight = arr[i+1];
            loc++;
        }
        else {
            int up = retIndex(rowVal-1, colVal, ncols);
            if(isValidNode(rowVal-1, colVal, nrows, ncols)) {
                adjMatrix[i][loc].index = up;
                adjMatrix[i][loc].weight = arr[i+1];
            // adjMatrix[i][loc].dir = 1;
                loc++;
            }
        }

      
        int down = retIndex(rowVal + 1, colVal, ncols);
        if(isValidNode(rowVal+1, colVal, nrows, ncols)) {
            adjMatrix[i][loc].index = down;
            adjMatrix[i][loc].weight = arr[i+1];
            // adjMatrix[i][loc].dir = 2;
            loc++;
            
        }

        
        int right = retIndex(rowVal, colVal+1, ncols);
        if(isValidNode(rowVal, colVal+1, nrows, ncols)) {
            adjMatrix[i][loc].index = right;
            adjMatrix[i][loc].weight = arr[i+1];
           // adjMatrix[i][loc].dir = 3;
            loc++;
        }
        int left = retIndex(rowVal, colVal - 1, ncols);
        if(isValidNode(rowVal,colVal -1, nrows, ncols)) {
            adjMatrix[i][loc].index = left;
            adjMatrix[i][loc].weight = arr[i+1];
            //adjMatrix[i][loc].dir = 4;
            loc++;
        }
        assert(loc == nEdges);
        // if(i == 36) {
        //     for(int k = 0; k < adjMatrix[i]->numEdges; k++) {
        //             printf("(%d, %d)\n", findRow(adjMatrix[i][k].index, ncols), findCol(adjMatrix[i][k].index, ncols));
        //         }
        //         printf("---------------------\n");
        // }
    }
    //now add destination node kind of starting node

    //make starting node at nums index (above graph)
    adjMatrix[nums] = malloc(sizeof(Node));
    adjMatrix[nums]->weight = arr[0];
    adjMatrix[nums]->numEdges = 0;
    adjMatrix[nums]->index = nums;


    //make ending node at nums+1 index (below graph)
    adjMatrix[nums+1] = malloc((ncols+1) * sizeof(Node));
    //adjMatrix[nums+1]->index = nums+1;
    adjMatrix[nums+1]->numEdges = ncols;
    adjMatrix[nums+1]->weight = arr[nums+1];
    for(int i = 0; i < ncols; i++) {
        //adjMatrix[nums+1][i+1].weight = arr[i+1 + ((nrows-1) * ncols)]; //getting indices and weights of bottom row nodes
        adjMatrix[nums+1][i].index = i + ((nrows-1) * ncols);
        //fprintf(stderr, "weight: %d index %d\n", adjMatrix[nums+1][i].weight, adjMatrix[nums+1][i].index);
    }
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

void deleteNode(heapNode * node) {
    free(node);
}

void deleteHeap(heap * minHeap) {
    free(minHeap->arr);
    free(minHeap->pos);
    free(minHeap);
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

heapNode * createNode(int vert, int time) {
    heapNode * node = malloc(sizeof(heapNode));
    node->vert = vert;
    node->compWeight = time;
    return node;
    //we don't have to do any shifting 
    //because anytime insert a new node
    //compWeight will be infinity
}


int down_heapify(heap * minHeap, int currInd) { //now using double pointers so no need to return anything
    // printf("CURRIND: %d, MAXSIZE %d", currInd, minHeap->currSize);
    // for(int k = 0; k< minHeap->currSize; k++) {
    //                 printf("Ind: %d, Weights: %hu\n", minHeap->arr[k]->vert, minHeap->arr[k]->compWeight);
    // }
    // printf("\n-------------------\n");
    int swap = 0;
    int leftNode = findLeft(currInd, minHeap->currSize);
    int rightNode = findRight(currInd, minHeap->currSize);
    int temp = currInd;
    if (leftNode!=-1 && minHeap->arr[leftNode]->compWeight < minHeap->arr[temp]->compWeight) {
        temp = leftNode; //leftNode val is smaller than currNode
        swap = 1;
    }
    if(rightNode != -1 && minHeap->arr[rightNode]->compWeight < minHeap->arr[temp]->compWeight) {
        temp = rightNode;
        swap = 1;
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
        swap = down_heapify(minHeap, temp);
    }
    return swap;   
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
    int swap = down_heapify(minHeap, 0);
    if(!swap) {
        minHeap->pos[minHeap->arr[0]->vert] = 0;
    }
    return root;
}

void changeTimes(heap * minHeap, int vertInd, int weight) {
    int posInd = minHeap->pos[vertInd]; //getting index of node in pos array
    // if(vertInd == 42) {
    //     printf("-------------------------------\n\n");
    // }
    minHeap->arr[posInd]->compWeight = weight; //updating weight of node

    while(posInd > 0 && minHeap->arr[posInd]->compWeight < minHeap->arr[findParent(posInd)]->compWeight) {
        minHeap->pos[minHeap->arr[posInd]->vert] = findParent(posInd);
        minHeap->pos[minHeap->arr[findParent(posInd)]->vert] = posInd;
        

        swapNodes(&(minHeap->arr[posInd]), &(minHeap->arr[findParent(posInd)]));

        // printf("\nAfter Swap\nPosInd: %d, VertNum, %d Weights: %hu\n", posInd, minHeap->arr[posInd]->vert, minHeap->arr[posInd]->compWeight);
        // printf("ParInd: %d, VertNum, %d Weights: %hu\n\n", findParent(posInd), minHeap->arr[findParent(posInd)]->vert, minHeap->arr[findParent(posInd)]->compWeight);

        posInd = findParent(posInd);
    }
    // for(int i = 0; i < minHeap->currSize; i++) {
    //     printf("Ind: %d, Weights: %hu\n", minHeap->arr[i]->vert, minHeap->arr[i]->compWeight);
    // }

    //printf("hello\n");
}

void printArr(int dist[], int n, short ncols)
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d: (%d,%d) \t\t %hu\n", i, findRow(i, ncols), findCol(i, ncols), dist[i]);
}

void implDijkstra(Node ** graph, int source, short ncols, short nrows, int * path, int * accTimes) { //source is 0 node
    //path[0] = source;
    //int pathloc = 1;
    int numVerts = (ncols * nrows) + 2; //might not need to include ending node (nums + 1)
    //numVerts is V
    heap * minHeap = initHeap(numVerts);
    //short accTimes[numVerts]; //dist arr
    //short * accTimes = malloc(numVerts * sizeof(short));
    //int path[ncols * nrows];
    for(int i = 0; i < numVerts; i++) {
        accTimes[i] = INT_MAX;
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
        // if(minHeap->currSize == 1) {
        //     printf("%d\n", minHeap->arr[0]->vert);
        //     minHeap->pos[minHeap->arr[0]->vert] = 0;
        // }

        // printf("------TESTING: %d-------\n%d", minNode->vert, minHeap->currSize);
        // printf("\nAfter Extracting:\n");
        
        int vert = (minNode->vert); //getting vertex number of extracted node
        // if(vert < ncols) {
        //     printf("%d\n", vert);
        // }
        
        for(int i=0; i < graph[vert]->numEdges; i++) { //now iterate through all adjacent nodes
            // if(vert==numVerts-1) {
            //     i=i+1;
            // }
            // int edges = graph[vert]->numEdges;
            // int adjNode = graph[vert][i].index;
            // int weight = graph[vert]->weight;

            
            if(inHeap(minHeap, graph[vert][i].index) && accTimes[vert] != INT_MAX && (graph[vert]->weight + accTimes[vert]) < accTimes[graph[vert][i].index]) {
                accTimes[graph[vert][i].index] = accTimes[vert] + graph[vert]->weight;
                //location++;
                //int temp1 = accTimes[graph[vert][i].index];
                // if(vert == 35) {
                //     printf("------------------------------\n");
                // }
                changeTimes(minHeap, graph[vert][i].index, accTimes[graph[vert][i].index]);
                path[graph[vert][i].index] = vert; //saving where we are coming from
                //short afterWeight = accTimes[source][l];
                //printf("\n");
                //int h = path[source][graph[vert][i].index];
                //printf("%d\n",h);

                //down_heapify(minHeap, 0);
                // printf("\n\n");
                // if(vert == 0) {
                //     for(int k = 0; k < graph[vert]->numEdges; k++) {
                //         printf("(%d, %d)\n", findRow(graph[vert][k].index, ncols), findCol(graph[vert][i].index, ncols));
                //     }
                //     printf("---------------------\n");
                // }
                
            }
        }
        deleteNode(minNode);
        //pathloc++;
    }
    //return accTimes;
    //printArr(accTimes, numVerts, ncols);
    deleteHeap(minHeap);
}
// void freePath(int ** path, short ncols) {
//     for(int i = 0; i < ncols; i++) {
//         free(path[i]);
//     }
//     free(path);
// }

// void freeTimes(int ** times, short ncols) {
//     for(int i = 0; i < ncols; i++) {
//         free(times[i]);
//     }
//     free(times);
// }
void writePath(char * outfile, int * path, int pathLength, int endTime, short ncols) {
    FILE * fp = fopen(outfile, "wb");
    //FILE * tfp = fopen("text.txt", "w");
    int time = (int) endTime;
    fwrite(&time, sizeof(int), 1, fp);
    fwrite(&pathLength, sizeof(int), 1, fp);
    //fprintf(tfp, "%d\n", time);
    //fprintf(tfp, "%d\n", pathLength);
    for(int i = 0; i < pathLength; i++) {
        short rowval = (short) findRow(path[i], ncols);
        short colVal = (short) findCol(path[i], ncols);
        fwrite(&rowval, sizeof(short), 1, fp);
        fwrite(&colVal, sizeof(short), 1, fp);
        //fprintf(tfp,"%d %d\n", rowval, colVal);
    }
    //fprintf(tfp,"%d", path[pathLength-1]);
    fclose(fp);
    //fclose(tfp);
}

void writeTimes(char * outfile, int * times, short ncols, int verts) {
    FILE * fp = fopen(outfile, "wb");
    //FILE * ofp = fopen("times.txt", "w");
    fwrite(&ncols, sizeof(short), 1, fp);
    //fprintf(ofp, "%hu\n", ncols);
    for(int i = 0; i < ncols; i++) {
        int temp = times[i];
        fwrite(&temp, sizeof(int), 1, fp);
        //fprintf(ofp, "%d\n", temp);
    }
    fclose(fp);
    //fclose(ofp);
}

//need to make accTimes an integer array because sometimes the short overflows
int main(int argc, char ** argv) {
    //printf("hello\n");
    short nrows;
    short ncols;
    //printf("%s\n", argv[2]);
   // printf("hello\n");
    //char * l = argv[1];
    short * arr = readInBinary(argv[1], &nrows, &ncols, argv[2]);
    if(arr == NULL) {
        return EXIT_FAILURE;
    }
    //printf("hello\n");
    
    int nums = ncols*nrows;
    Node ** adjMatrix = adjList(arr, nrows, ncols);
    //printf("hello2\n");
    //freeMatrix(arr, rows);
    // fprintf(stderr, "---------------------------------------------\n");
    // //fprintf(stderr, "ind: %d Weight: %hu", i, arr[i]);
    // // fprintf(stderr, "Source Node: coord: (-1, -1)");
    // // for(int j=0; j < ncols; j++) {
    // //     fprintf(stderr, "->(%d, %d)", findRow(adjMatrix[0][j].index, ncols), findCol(adjMatrix[0][j].index, ncols));
    // // }
    // // fprintf(stderr, "\n");
    // for(int i = (nums+1); i > 0; i--) {
    //     fprintf(stderr, "Start Node:%d Edges ", adjMatrix[i]->numEdges);
    //     //fprintf(stderr, "%hu", arr[adjMatrix[i]-index]);
    //     fprintf(stderr, "coord: (%d, %d) Weight: %hu", findRow(i-1, ncols), findCol(i-1, ncols), arr[i]);
    //     for(int k = 0; k < adjMatrix[i]->numEdges; k++) {
    //         fprintf(stderr, "->(%d, %d)", findRow(adjMatrix[i][k].index, ncols), findCol(adjMatrix[i][k].index, ncols));
    //     }
    //     fprintf(stderr, "\n");
    // }
    // fprintf(stderr, "Start Node:%d Edges ", adjMatrix[0]->numEdges);
    // fprintf(stderr, "coord: (%d, %d)\n", findRow(adjMatrix[0][0].index, ncols), findCol(adjMatrix[0][0].index, ncols));
    free(arr);
    //freeMatrix(adjMatrix, nrows, ncols);
    // //fprintf(stderr, "------------\n%d\n", adjMatrix[2][0].index);
    #if 1
    int * path = malloc((nums+2) * sizeof(int));
    int * accTimes = malloc((nums+2) * sizeof(int));
    implDijkstra(adjMatrix, nums+1, ncols, nrows, path, accTimes);


    for(int i = 0; i < ncols; i++) {
        accTimes[i] = accTimes[i] + adjMatrix[i]->weight;
    }
    //printArr(accTimes, nums+2, ncols);
    //("done\n");
    //printf("%d\n",path[0][50]);
    //printf("hello\n");
    // for(int i = 0; i < nums+1; i++) {
    //     printf("%d: (%d,%d) \t\t %hu\n", i, findRow(i, ncols), findCol(i, ncols), dist[i]);
    // }
    
    //printArr(accTimes[0], nums+1, ncols);

    //printArr(path[shortest], nums+1, ncols);
    // short ** times = malloc(ncols * (nums+1) * sizeof(short));
    // times = implDijkstra(adjMatrix, 0, ncols, nrows, path);
    // for(int i = 1; i < ncols; i++) {
    //     short * tempTimes = implDijkstra(adjMatrix, i, ncols, nrows, path);
    //     if(tempTimes[nums] < times[nums]) {

    //     }
    //     printf("\nStarting new path:\n");
    // }
    
    //printArr(times, nums+1, ncols);

    //printf("%d\n", path[0][0]);
    //int place = nums;
    // for(int i = 0; i <  nums+1; i++) {
    //     printf("from: %d, to: %d\n", path[shortest][i], i);
    // }
    int place2 = 0;
    // while(path[place] != nums) {
    //     printf("Forward Traversal: from: %d, to %d\n", place, path[place]);
    //     place = path[place];
    // }
    int * pathForward = malloc(nums * sizeof(int));
    //pathForward[0] = shortest;
    //printf("hello\n");
    int place = nums;
    // for (int i = 1; i < ncols; i++) {
    //     if(accTimes[i] < accTimes[i-1]) { //finding top row ending node
    //         place = i;
    //     }
    // }
    
    // for(int i = 0; i < 100; i++) {
    //     printf("%d\n", path[i]);
    // }
    while(path[place] != nums+1) {

        pathForward[place2] = path[place];
        place2++;
        place = path[place];
    }
    // while(path[shortest][place] != shortest) {
    //     //printf("Backwards Traversal: from: %d, to %d\n", place, path[shortest][place]);
    //     pathForward[place2] = path[shortest][place];
    //     place2++;
    //     place = path[shortest][place];

    // }
    // pathForward[place2] = shortest;
    // for(int i = 0; i < place2; i++) {
    //     printf("Path: %d\n", pathForward[i]);
    // }
    // while(place != (nums)) {
    //     printf("inds: %d\n", path[place]);
    //     place = path[place];
    // }
    writePath(argv[4], pathForward, place2, accTimes[nums], ncols);
    writeTimes(argv[3], accTimes, ncols, nums);
    
    //implDijkstra(adjMatrix, 0, ncols, nrows);
    //implDijkstra(adjMatrix, 1, ncols, nrows);
    free(accTimes);
    free(path);
    // freeTimes(accTimes, ncols);
    // freePath(path, ncols);
    free(pathForward);
    freeMatrix(adjMatrix, nrows, ncols);
    //printf("hello\n");
    //printf("done\n");
    #endif
    return EXIT_SUCCESS;
}