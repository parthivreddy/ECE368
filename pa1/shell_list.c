#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "shell_list.h"
#include "list_of_list.h"


Node *List_Load_From_File(char *filename){
    int done = 0;
    long num;
    FILE *fp;
    fp = fopen(filename, "rb");
    if(fp == NULL) {
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    int sizeFile = ftell(fp);
    rewind(fp);
    if (sizeFile == 0) {
        fclose(fp);
        return NULL;
    }
    Node * head = malloc(sizeof(Node));
    Node * ptr = head;

    while (!done) {
        fread(&num, sizeof(num), 1, fp);
        (ptr->value) = num;
        if (ftell(fp) >= sizeFile) {
            done = 1;
        }
        else {
            (ptr->next) = malloc(sizeof(Node));
            ptr = (ptr->next);
        }

    }
    (ptr->next) = NULL;
    fclose(fp);
    return head;
}

int List_Save_To_File(char *filename, Node *list){
    int counter=0;
    FILE *fp;
    fp = fopen(filename, "wb");
    if(fp == NULL || list == NULL) {
        fclose(fp);
        return counter;
    }
    Node * ptr = list;
    while (ptr != NULL) {
        //printf("ptrVal = %d:%x\n",ptr->value, ptr->value);
        fwrite(&(ptr->value), sizeof(long), 1, fp);
        ptr = (ptr->next);
        counter++;
    }
    fclose(fp);
    return counter;
}

static void delList(List * headofList) {
    while(headofList->next != NULL) {
        List * tempList = headofList->next;
        free(headofList);
        headofList = tempList;
    }
    //free(headofList->node);
    free(headofList);
}
static List * makeSublists(List * headList, Node * head, int size, int k) { //making sublists in parallel with each other
    List * storeHeads = NULL;
    Node * ptr = head; //getting head of entire linked list
    headList = malloc(sizeof(List));
    List * currList = headList; //getting start of list of sublists
    headList->node = NULL;
    storeHeads = malloc(sizeof(List));
    storeHeads->node = NULL;
    storeHeads->next = NULL;
    List * addstoreHeads = storeHeads;
    //(*listofHeads)->node = &(headList->node); 
    //int k = 4;
    while (k > 1) {
        storeHeads->next = malloc(sizeof(List));
        storeHeads = storeHeads->next;
        storeHeads->node = NULL;
        storeHeads->next = NULL;
        currList->next = malloc(sizeof(List)); //allocating space for k linked lists (sublists)
        //(*listofHeads)->next = &(currList->next);
        currList = currList->next; //set curr to new list
        //*listofHeads = (*listofHeads)->next;
        currList->node = NULL;
        k--;
    }
    //Node * nextPtr = ptr -> next; //points to one node after ptr
    currList->next = NULL; //point last sublist to NULL
    currList = headList;
    storeHeads = addstoreHeads;
    //listofHeads = &headList;
    //currList->node = ptr; //initialize first sublist to point to first element of og linked list
    //currList->node = ptr; //setting first sublist node to start of og linked list
    for (int i = 0; i < size; i++) {
        if (currList == NULL) {
            currList= headList;
        }
        Node * cptr = NULL;
        if (currList->node == NULL) {
            currList->node = ptr;
            storeHeads->node = ptr;
            storeHeads = storeHeads->next; 
        }
        else {
           // Node ** listHeads = &currList->node;
            // cptr = currList->node;
            // while (cptr->next != NULL) {
            //     cptr = cptr->next;
            // }
            currList->node->next = ptr;
            currList->node = currList->node->next;
            cptr = currList->node;
            //cptr->next = ptr;
        }
        cptr = ptr;
        ptr = ptr->next;
        cptr->next = NULL;
    
        currList = currList->next;

    }
    //free()
    
    
    // for(int i = 0; i < size; i++) {
    //     if (i < k) {
    //         currList->node = ptr;
    //     }
    //     Node * headofList = currList->node; //keeping track of the head node in each linked list
    //     if (i >= k) { //if there are now more than one element going into each linked list
            
    //         while (currList->node->next != NULL) { //while going through each element in each linked list
    //             currList->node = currList->node->next; //go to next node in each linked list until you reach the end
    //         }
    //         currList->node->next = ptr;
    //         currList->node = currList->node->next;
    //     }
    //     currList->node->next = NULL; //remove nodes in sublists from connecting to rest of list (allows us to connect to next element in sublist)
    //     currList->node = headofList; //rewind list back to head
    //     if (nextPtr != NULL) {
    //         ptr = nextPtr; //getting next node in og linked list
    //         nextPtr = nextPtr -> next; //getting next node in og linked list (one after ptr)

    //     }
    //     currList = currList->next; //next sublist 
    //     if (currList == NULL) {
    //         currList = headList;
    //     }
        
    // }
    //storeHeads->next = NULL;
    currList = headList;
    delList(currList);
    return addstoreHeads;
}

// Node * insertNode(Node * head, Node * sortNode, long *n_comp) { //returns head of linked list with one node sorted
//     if(head == NULL) {
//         sortNode->next = NULL;
//         return sortNode;
//     }
//     if (sortNode->value < head->value) {
//         *n_comp = *n_comp+1;
//         sortNode->next = head;
//         return sortNode;
//     }
//     Node * curr= head;
//     while (curr->next != NULL && curr->next->value < sortNode->value) {
//         curr = curr->next;
//         *n_comp = *n_comp +1;
//     }
//     sortNode->next = curr->next;
//     curr->next = sortNode;
//     return head;
// }


//     // printf("in insert node\n");
//     // Node * currNode = head->next;
//     // if (sortNode == head) {
//     //     return head;
//     // }
//     // if (sortNode->value < head->value) {
//     //     *n_comp = *n_comp +1;
//     //     sortNode->next = head;
        
//     //     return sortNode;
//     // }

//     // while(currNode!=NULL && currNode->value < sortNode->value) { //might be <=
//     //     currNode = currNode->next; //get out of loop when currNode->next value is greater than sortNode
//     //     *n_comp = *n_comp + 1;
//     // }
//     // if (currNode == sortNode) {

//     // }
//     // else {
//     //     sortNode->next = currNode->next; //point sortNode to node after currNode
//     //     currNode->next = sortNode; //point currNode to sortNode
//     // }
    
//     // return head;
// }

// List * insertionSort(List * listHead, int k, long *n_comp) { //goes through a sublist and sorts it. listHead is head of list of lists
//     //printf("in insertionsort\n");
//     List * currList = listHead;

//     for (int i = 0; i < k; i++) {
//         Node * head = NULL;
//         Node * currptr = currList->node;
//         Node * unsorted = NULL;
//         while (currptr != NULL) {
//             unsorted = currptr->next;
//             currptr->next = NULL;
//             head = insertNode(head, currptr, n_comp);
//             currptr = unsorted;

//         }
//         currList->node = head;
//         currList = currList->next;
//     }
    
//     return listHead; //returns head of list
    
// }
static Node * swapLL(Node * first, Node * second) {
    Node * temp = second->next;
    second->next = first;
    first->next = temp;
    return second;
}

static List * bubbleSort(List * listHead, int k, long *n_comp, int size) {
    List * currList = listHead;
    //Node *check;
    for(int i = 0; i < k; i++) {
        Node ** head = &currList->node;
        int count;
        count = size / k;
        int rem = size % k;
        if (i < rem) {
            count++;
        }
        for(int j = 0; j <= count; j++) {
            int swapBool = 0;
            Node ** sorted = head;
            for(int m = 0; m < count - j - 1; m++) {
                Node * currptr = *sorted;
                if(currptr->value > currptr->next->value) {
                    //*n_comp = *n_comp+1;
                    *sorted = swapLL(currptr, currptr->next);
                    swapBool = 1;
                }
                //head = sorted;
                (*n_comp)++;
                sorted = &(*sorted)->next;
            }
            if (swapBool == 0) {
                break;
            }
        }
        currList->node = *head;
        currList = currList -> next;


    }
    return listHead;
}


static Node * restoreKSortedList(List * listHead, int size, int k) {
    List * currList = listHead;
    Node * head = listHead->node;
    Node * ptr = head;
    Node * nptr = currList->node->next;
    currList->node = nptr;
    currList = currList -> next;
    for(int i = 0; i < size - 1; i++) {
        if(currList == NULL) {
            currList = listHead;
        }
        ptr->next= currList->node;
        ptr = ptr->next;
        nptr = currList->node->next;
        ptr->next = NULL;
        currList->node = nptr;
        currList = currList -> next;

    }
    currList = listHead; //not sure if this is necessary
    return head;
}

Node *List_Shellsort(Node *list, long *n_comp) { //list is head
    Node * ptr = list;
    List * kHeadList = NULL; // defining k linked lists
    if (ptr == NULL) {
        return list;
    }
    int size = 1;
    while (ptr -> next != NULL) {
        ptr = ptr->next;
        size++; //getting size of linked list
    }
    int k = 1;
    while (k*3+1 < size){
        k = k*3+1;
    }
    // we get k, size, and head of linked list
    while (k > 0) {
        kHeadList = makeSublists(kHeadList, list, size, k);

        kHeadList = bubbleSort(kHeadList, k, n_comp, size);

        list = restoreKSortedList(kHeadList, size, k);
        delList(kHeadList);
        kHeadList = NULL;
        k /= 3;
    }
    return list;

}

