#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell_array.h"
long *Array_Load_From_File(char *filename, int *size) {
    int index = 0;
    long num;
    int counter = 0;
    FILE *fp;
    fp = fopen(filename, "rb");
    if(fp == NULL) {
        *size = 0;
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    int sizeFile = ftell(fp);
    rewind(fp);
    // printf("found file size%d \n", sizeFile);
    // printf("longsize %d", sizeof(long));
    *size = sizeFile / sizeof(long);
    // if(*size % 8 != 0) {
    //     return NULL;
    // }
    //printf("checking%d\n", *size);
    long *arr = (long *)malloc(sizeof(long) * (*size));
    if(arr == NULL) {
        *size = 0;
        return NULL;
    }
    
    while (ftell(fp) < sizeFile) {

        fread(&num, sizeof(num), 1, fp);
        // num = (num & 0x00000000FFFFFFFF) << 32 | (num & 0xFFFFFFFF00000000) >> 32;
        // num = (num & 0x0000FFFF0000FFFF) << 16 | (num & 0xFFFF0000FFFF0000) >> 16;
        // num = (num & 0x00FF00FF00FF00FF) << 8  | (num & 0xFF00FF00FF00FF00) >> 8;
        arr[index] =num;
        //int len = ftell(fp);
        //printf("sizeof(num):%d\n", sizeof(long));
        //printf("num: %ld index: %d ftells:%d\n",num, index, len);
        counter+=8;
        index++;
    }
    //printf("obtained numbers\n");
    fclose(fp);
    // for(int i =0; i<*size; i++) {
    //     printf("arr[%d]: %ld\n", i, arr[i]);
    // }
    return arr;
}

int Array_Save_To_File(char *filename, long *array, int size){
    int count = 0;
    FILE *fp;
    fp = fopen(filename, "wb");
    if(array == NULL || size == 0) {
        
    }
    else {
        while(size > 0) {
            //printf("vals: %ld\n", array[count]);
            fwrite((array+count), sizeof(long), 1, fp);
            //fprintf(fp, "%lb", array[count]);
            count++;
            size--;
        }
    }
    fclose(fp);
    return count;
}

void Array_Shellsort(long *array, int size, long *n_comp) {
    int k = 1;
    while (k*3+1 < size){
        k = k*3+1;
    }
    while (k > 0) {
        for (long j = k; j < size; j++) {
            long temp = array[j];
            long i = j;
            while(i >= k && array[i-k] > temp) {
                array[i] = array[i-k];
                *n_comp = *n_comp + 1;
                i -=k;
            }
            (*n_comp)++;
            array[i] = temp;
        }
        // for(int i =0; i<size; i++) {
        //     printf("arr[%d]: %ld\n", i, array[i]);
        // }
        k /= 3;
    }
}

