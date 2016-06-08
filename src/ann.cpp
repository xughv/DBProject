#include "ann.h"
#include <cstring>

void Indexing(int num, int dim, int num_line, int page_size, char* data_set, char* output_folder) {

    unsigned** data = new unsigned*[num];
    for (int i = 0; i < num; i++) {
        data[i] = new unsigned[dim];
        memset(data[i], 0, sizeof(unsigned) * dim);
    }

    if (!ReadSetFromFile(data_set, num, dim, data)) {
        // TODO: Error
        printf("Read DataSet Failed.\n");
    } else {

        char* index_path = new char[strlen(output_folder) + 20];

        strcpy(index_path, output_folder);
        strcat(index_path, "index/");

        if (!CreateDirectory(index_path)) {
            printf("Create Directory Failed.\n");
            // TODO: Error
        }

        MEDRANK* medrank = MEDRANK::GetInstance();
        medrank->GenLines(num, num_line);

        Pair* pairs = new Pair[num];

        char* file_name = new char[20];

        for (int i = 0; i < num_line; ++i) {
            for (int j = 0; j < num; ++j) {
                pairs[j].SetValue(j, CalcProjection(dim, data[j], medrank->GetLine(i)));
            }
            // sort
            Sort(pairs, pairs + num);

            // generate the file name of the b-tree
            GenTreeFileName(i, index_path, file_name);

            // build a b-tree
            BTree* btree = new BTree();
            btree->Init(file_name, page_size);
            btree->BulkLoad(pairs, num);
        }

        delete[] index_path;
        delete[] file_name;
        delete[] pairs;
    }

    for (int i = 0; i < num; i++) {
        delete[] data[i];
    }
    delete [] data;
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

        MEDRANK* medrank = MEDRANK::GetInstance();
        medrank->Init();



        for (int i = 0; i < num; ++i) {
            for (int j = 0; j < medrank->num_line(); ++j) {
                medrank->set_q(j, CalcProjection(dim, data[i], medrank->GetLine(j)));
            }
        }

    }
}