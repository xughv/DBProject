//
// Created by 徐广晖 on 16/6/5.
//


#include "util.h"

#include <cstring>
#include <cstdlib>
#include <cmath>

#include <sys/stat.h>

#include "def.h"

// Use Box-Muller method to generate a random variable from N(0,1).
float Rand() {
    float u1 = (float) rand() / (float) RAND_MAX;
    float u2 = (float) rand() / (float) RAND_MAX;

    float x = sqrt(-2.0f * log(u1)) * cos(2.0f * PI * u2);

    return x;
}

// Get a vector by Box-Muller and then normalize it.
void GenRandomVector(int dim, float* vec) {
    float length = 0.0f;
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

// ----------------------------------------------------------------------------
// ReadSetFromFile
// read the data from a file <file_name> in binary.
// This is the format of data set:
// [offset] [type]          [value]          [description]
// 0000     32 bit integer  0x00000803(2051) magic number
// 0004     32 bit integer  60000            number of images
// 0008     32 bit integer  28               number of rows
// 0012     32 bit integer  28               number of columns
// 0016     unsigned byte   ??               pixel
// 0017     unsigned byte   ??               pixel
// ........
// xxxx     unsigned byte   ??               pixel
//
// Thus we skip the 16 bytes of the file and just read the data after the 16 offset.
// ----------------------------------------------------------------------------
// <file_name>:     the path of the file
// <num>:           the number of the data
// <dim>:           the dimension of the data
// <data>:          get the data
bool ReadSetFromFile(char* file_name, int num, int dim, unsigned char** data) {

    // open data file
    FILE* fp = fopen(file_name, "rb");

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
        printf("Please set the size of data set to be %d.\n", i);
        return false;
    }

    // close data file
    fclose(fp);
    return true;
}

// Use multiplication to calculate the projection of a vector on a line
float CalcProjection(int dim, unsigned char* object, float* line) {
    float length = 0.0f;
    for (int i = 0; i < dim; ++i) {
        length += (float)object[i] * line[i];
    }
    return length;
}

// -------------------------------------------------------------------------
// CreateDirectory
// In the Linux and OS X system can use the API in CreateDirectory function.
// But not in Windows system, so, if you want to run the application in
// windows, you can create the folder "results/index/" by yourself and comment
// the function CreateDirectory.
// -------------------------------------------------------------------------
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


// Generate the index file name
void GenTreeFileName(int tree_id, char* path, char* file_name) {
    char* tmp = new char[20];
    strcpy(file_name, path);
    sprintf(tmp, "%d", tree_id);
    strcat(file_name, tmp);
    strcat(file_name, ".index");
    delete[] tmp;
}


// Use the Euclidean distance to calculate the distance of two point
float CalcPointsDistance(unsigned char* point1, unsigned char* point2, int dim) {
    float dis = 0;
    for (int i = 0; i < dim; ++i) {
        dis += ((float)point1[i] - point2[i])*(point1[i] - point2[i]);
    }
    dis = sqrtf(dis);
    return dis;
}
