#ifndef _ANN_H_
#define _ANN_H_

#include "util.h"
#include "medrank.h"

void Indexing(int num, int dim, int num_line, int page_size, char* data_set, char* output_folder);

void CalcANN(int num, int dim, char* query_set, char* output_folder);

void LinearScan(int num_data, int num_query, int dim, char* query_set, char* data_set);

#endif // _ANN_H_
