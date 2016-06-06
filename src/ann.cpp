#include <cstring>
#include "ann.h"
#include "util.h"

void GenRandomVector(int dem, float *vec) {
    for (int i = 0; i < dem; ++i) {
        vec[i] = Rand();
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
    }

}
