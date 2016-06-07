#include <cstring>
#include "ann.h"
#include "medrank.h"


void Indexing(int num, int dim, int num_line, int page_size, char* data_set, char* output_folder) {

    unsigned** data = new unsigned*[num];
    for (int i = 0; i < num; i++) {
        data[i] = new unsigned[dim];
        memset(data[i], 0, sizeof(data[i])*dim);
    }

    if (!ReadSetFromFile(data_set, num, dim, data)) {
        // TODO: Error
    } else {

        char* index_path = NULL;

        strcpy(index_path, output_folder);
        strcat(index_path, "index/");

        if (!CreateDirectory(index_path)) {
            // TODO: Error
        }

        MEDRANK* medrank = MEDRANK::GetInstance();
        medrank->GenLines(dim, num_line);

        Pair* pairs = new Pair[num];

        char* file_name = new char[20];

        for (int i = 0; i < num; ++i) {
            pairs[i].SetValue(i, CalcProjection(dim, data[i], medrank->GetLine(i));
            // std::sort(pairs, pairs + num);

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

void CalcANN(int num, int dim, char* query_set, char* output_folder) {

    unsigned **data = new unsigned *[num];
    for (int i = 0; i < num; i++) {
        data[i] = new unsigned[dim];
        memset(data[i], 0, sizeof(data[i]) * dim);
    }

    if (!ReadSetFromFile(query_set, num, dim, data)) {
        // TODO: Error
    } else {

    }
}