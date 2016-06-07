//
// Created by 徐广晖 on 16/6/5.
//


#include "util.h"

#include <cstring>
#include <cstdlib>
#include <cmath>

#include <sys/stat.h>

float Rand() {
    float u1 = (float) rand() / (float) RAND_MAX;
    float u2 = (float) rand() / (float) RAND_MAX;

    float x = sqrt(-2.0f * log(u1)) * cos(2.0f * M_PI * u2);

    return x;
}

void GenRandomVector(int dim, float* vec) {
    float length = 0;
    for (int i = 0; i < dim; ++i) {
        vec[i] = Rand();
        length += vec[i] * vec[i];
    }

    // Normalize
    length = sqrt(length);
    for (int i = 0; i < dim; ++i) {
        vec[i] = vec[i]/length;
    }
}

bool ReadSetFromFile(char* file_name, int num, int dim, unsigned** datas) {
    int i = 0;
    int j = 0;
    FILE* fp = NULL;

    fp = fopen(file_name, "rb");			// open data file
    if (!fp) {
        printf("I could not open %s.\n", file_name);
        return false;
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
        printf("The size of set is larger than you input\n");
    }
    else if (feof(fp) && i < num) {
        printf("Set the size of dataset to be %d. ", i);
        printf("And try again\n");
    }


    fclose(fp);						// close data file
    return true;
}

float CalcProjection(int dim, unsigned* object, float* line) {
    float length = 0;
    for (int i = 0; i < dim; ++i) {
        length += object[i] * line[i];
    }
    return length;
}

bool CreateDirectory(const char* path) {
    if (path == NULL) {
        return false;
    }

    FILE *fp = NULL;

    char tmp_path[100];
    memset(tmp_path, 0, sizeof(tmp_path));

    int tmp_pos = 0;
    const char* cur_pos = path;

    while (*cur_pos++ != '\0') {

        tmp_path[tmp_pos++] = *(cur_pos-1);

        if ((*cur_pos == '/' || *cur_pos == '\0') && strlen(tmp_path) > 0) {
            // check the directory exists or not
            fp = fopen(tmp_path, "w");
            if (fp == NULL) {
                // create directory
                if (mkdir(tmp_path, 0777) != 0) {
                    // TODO: Error
                    return false;
                }
            }
        }
    }
    return true;
}

void GenTreeFileName(int tree_id, char* path, char* file_name) {
    char tmp[20];
    strcpy(file_name, path);
    sprintf(tmp, "%d", tree_id);
    strcat(file_name, tmp);
    strcat(file_name, ".index");
}