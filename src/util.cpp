//
// Created by 徐广晖 on 16/6/5.
//


#include "util.h"

#include <cstring>
#include <cstdlib>
#include <cmath>

#include <sys/stat.h>

#include "def.h"

float Rand() {
    float u1 = (float) rand() / (float) RAND_MAX;
    float u2 = (float) rand() / (float) RAND_MAX;

    float x = sqrt(-2.0f * log(u1)) * cos(2.0f * PI * u2);

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

bool ReadSetFromFile(char* file_name, int num, int dim, unsigned** data) {

    // open data file
    FILE* fp = fopen(file_name, "r");

    if (!fp) {
        printf("Could not open %s.\n", file_name);
        return false;
    }

    fseek(fp, 16, SEEK_SET);

    int i = 0;
    // read data file
    while (!feof(fp) && i < num) {
        for (int j = 0; j < dim; j++) {
            fread(&data[i][j], SIZE_CHAR, 1, fp);
        }
        i++;
    }

    // check the size of set
    if (feof(fp) && i < num) {
        printf("please set the size of data set to be %d. ", i);
        return false;
    }

    // close data file
    fclose(fp);
    return true;
}

float CalcProjection(int dim, unsigned* object, float* line) {
    float length = 0.0f;
    for (int i = 0; i < dim; ++i) {
        length += (float)object[i] * line[i];
    }
    return length;
}

bool CreateDirectory(const char* path) {
    if (path == NULL) {
        return false;
    }

    char tmp_path[100];
    memset(tmp_path, 0, sizeof(tmp_path));

    int tmp_pos = 0;
    const char* cur_pos = path;

    while (*cur_pos++ != '\0') {

        tmp_path[tmp_pos++] = *(cur_pos-1);

        if ((*cur_pos == '/' || *cur_pos == '\0') && strlen(tmp_path) > 0) {
            tmp_path[tmp_pos] = '\0';
            // check the directory exists or not
            int exist = access(tmp_path, F_OK);
            if (exist != 0) {
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
    char* tmp = new char[20];
    strcpy(file_name, path);
    sprintf(tmp, "%d", tree_id);
    strcat(file_name, tmp);
    strcat(file_name, ".index");
    delete[] tmp;
}

float CalcPointsDistance(unsigned* point1, unsigned* point2, int dim) {
    float dis = 0;
    for (int i = 0; i < dim; ++i) {
        dis += (point1[i] - point2[i])*(point1[i] - point2[i]);
    }

    dis = sqrtf(dis);

    return dis;
}

int Compare(const void *a , const void *b) {
//    return (*(Pair **)a)->projection() > (*(Pair **)b)->projection() ? 1 : -1;
    return ((Pair *)a)->projection() > ((Pair *)b)->projection() ? 1 : -1;
}