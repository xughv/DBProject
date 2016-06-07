#include <cstring>
#include <algorithm>
#include <cmath>
#include "ann.h"
#include "medrank.h"

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

void Indexing(int num, int dim, int page_size, char* file_name, char* output_folder) {

    unsigned** data = new unsigned*[num];
    for (int i = 0; i < num; i++) {
        data[i] = new unsigned[dim];
        memset(data[i], 0, sizeof(data[i])*dim);
    }

    if (!ReadSetFromFile(file_name, num, dim, data)) {
        // TODO: Error
    } else {

        char* index_path = NULL;

        strcpy(index_path, output_folder);
        strcat(index_path, "index/");

        if (!CreateDirectory(index_path)) {
            // TODO: Error
        }

        float* line = new float[dim];
        Pair* pairs = new Pair[num];

        char* file_name = new char[20];

        GenRandomVector(dim, line);

        for (int i = 0; i < num; ++i) {
            pairs[i].SetValue(i, CalcProjection(dim, data[i], line));
            std::sort(pairs, pairs + num);

            // generate the file name of the b-tree
            GenTreeFileName(i, index_path, file_name);

            // build a b-tree
            BTree* btree = new BTree();
            btree->Init(file_name, page_size);
            btree->BulkLoad(pairs, num);
        }

        delete index_path;
        delete file_name;

        // TODO: Release space
    }
}
