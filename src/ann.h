#ifndef _ANN_H_
#define _ANN_H_

#include "util.h"

void GenRandomVector(int dim, float *vec);

void Indexing(int num, int dim, int page_size, char* data_set, char* output_folder);

void LinearScan();

void BuildBTree(Pair* pairs);

#endif // _ANN_H_
