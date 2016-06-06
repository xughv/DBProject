//
// Created by 徐广晖 on 16/6/5.
//


#include "util.h"

void ReadSetFromFile(char* file_name, int num, int dim, unsigned** datas) {
    int i = 0;
    int j = 0;
    FILE* fp = NULL;

    fp = fopen(file_name, "rb");			// open data file
    if (!fp) {
        printf("I could not open %s.\n", file_name);
        return;
    }

    fseek(fp, 16, SEEK_SET);
    i = 0;

    while (!feof(fp) && i < num) {	// read data file
        for (j = 0; j < dim; j++) {
            fread(&datas[i][j], sizeof(char), 1, fp);
        }
        i++;
    }
    if (!feof(fp) && i == num) {		// check the size of set
        printf("The size of set is larger than you input\n", false);
    }
    else if (feof(fp) && i < num) {
        printf("Set the size of dataset to be %d. ", i);
        printf("And try again\n", true);
    }


    fclose(fp);						// close data file
    return;
}