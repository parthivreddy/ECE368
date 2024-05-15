#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell_list.h"
#include "list_of_list.h"
#include "shell_array.h"
// #include "shell_array.c"
// #include "shell_list.c"
static int Arraymain(char * infile, char * outfile){
    int size;
    long help = 0;
    long *nsort = &help;
    long *arr = Array_Load_From_File(infile, &size);
    if(arr == NULL) {
        fprintf(stderr,"error:invalid file\n");
        exit(EXIT_FAILURE);
    }
    //printf("%d", size);
    //printf("%d\n", arr);
    Array_Shellsort(arr, size, nsort);
    // for(int i = 0; i < size; i++) {
    //     printf("sorted array[%d] = %ld\n", i, arr[i]);
    // }
    //for (int i =0; i < )
    Array_Save_To_File(outfile, arr, size);
    printf("%ld\n", *nsort);
    free(arr);
    //Array_Shellsort()
    return 0;
}
static void delLL(Node * head) {
    if(head == NULL) {
        return;
    }
    while(head->next!=NULL) {
        Node * tempNode = head->next;
        free(head);
        head = tempNode;
    }
    free(head);
}
static int Listmain(char * infile, char* outfile){
    //int size;
    long help = 0;
    long *n_comp = &help;
    Node * head = List_Load_From_File(infile);
    head = List_Shellsort(head, n_comp);
    List_Save_To_File(outfile, head);
    delLL(head);
    printf("%ld\n", *n_comp);
    //Array_Shellsort()
    //return true;
    exit(EXIT_SUCCESS);
}

int main(int argc, char ** argv) {
    if (argc != 4) {
        return EXIT_FAILURE;
    }
    if(argv[1][1] == 'a') {
        Arraymain(argv[2], argv[3]);
    }
    else if(argv[1][1] == 'l') {
        Listmain(argv[2], argv[3]);
    }
    else {
        exit(EXIT_FAILURE);
    }
}
