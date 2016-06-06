#include <cstring>
#include <algorithm>
#include <cmath>
#include "ann.h"
#include "medrank.h"
#include "util.h"

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

        float* line = new float[dim];
        Pair* pairs = new Pair[num];

        GenRandomVector(dim, line);
        for (int i = 0; i < num; ++i) {
            pairs[i].SetValue(i, CalcProjection(data[i], line));
            std::sort(pairs, pairs + num);
        }

        BTree* b_tree = new BTree();
    }

}
