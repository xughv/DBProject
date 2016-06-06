#include "ann.h"
#include "util.h"

void GenRandomVector(int dem, float *vec) {
    for (int i = 0; i < dem; ++i) {
        vec[i] = Rand();
    }
}

void Indexing(int num, int dim, int page_size, char* data_set, char* output_folder) {

    float** data = new float*[num];
    for (int i = 0; i < num; i++) data[i] = new float[dim];
    if (!ReadSetFromFile(data_set, num, dim, data)) {
        // TODO: Error
    }

}
