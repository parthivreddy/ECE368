#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    FILE *outfile = fopen("output.bin", "wb");

    if (outfile == NULL) {
        fprintf (stderr, "Error opening file");
    return 1;
    }

    srand(time(NULL));
    // Enter whatever grid dimension you want
    short rows = 1000;
    short cols = 1000;
  
    fwrite(&rows, sizeof(short), 1, outfile);
    fwrite(&cols, sizeof(short), 1, outfile);
	
    short value;
    for (int i = 0; i < (rows * cols); i++) {
    // Generate a random short value between 0 and __SHRT_MAX__
        value = rand() % 100;
        fwrite(&value, sizeof(short), 1, outfile);
    }

    fclose(outfile);
    return 0;
}