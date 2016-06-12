#ifndef _ANN_H_
#define _ANN_H_

#include "util.h"
#include "medrank.h"


// -----------------------------------------------------------------------------
// ALGORITHM 1 : Indexing
//  1) Projects the objects on m random lines {L 1 ,L 2 ,...,L m }
//  2) Indexes the objects on each lines with a B+tree.
// -----------------------------------------------------------------------------
// <num>:            the number of data set
// <dim>:            the dimension of data set
// <num_line>:       the number of line
// <page_size>:      one page size, here default is 1KB
// <data_set>:       the file path of data set
// <output_folder>:  the path of output
void Indexing(int num, int dim, int num_line, int page_size, char* data_set, char* output_folder);


// -----------------------------------------------------------------------------
// ALGORITHM 2: MEDRANK
//  1) Init the projection of query set on m random lines {L 1 ,L 2 ,...,L m }
//  2) Init the h cursor and the l cursor in the B+tree.
//  3) Execute the medrank and get the nearest point.
// -----------------------------------------------------------------------------
// <num>:            the number of query set
// <dim>:            the dimension of query set
// <query_set>:      the file path of query set
// <output_folder>:  the path of output
// <result_id>:      get the nearest point id
void CalcANN(int num, int dim, char* query_set, char* output_folder, int* result_id);


// -----------------------------------------------------------------------------
// LinearScan
// Just use violent search to find out the nearest point.
// -----------------------------------------------------------------------------
// <num_data>:       the number of data set
// <num_query>:      the number of query set
// <dim>:            the dimension of query set and data set
// <query_set>:      the file path of query set
// <data_set>:       the file path of data set
// <result_id>:      get the nearest point id
void LinearScan(int num_data, int num_query, int dim, char* query_set, char* data_set, int* result_id);

#endif // _ANN_H_
