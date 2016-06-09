#include "ann.h"
#include <cstring>
#include <cstdlib>

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
        int num_line_ = 50;  // 50条线
        MEDRANK* medrank = MEDRANK::GetInstance();

        medrank->Init("result");

        // 生成随机线段
        // 第一个参数为线段的维数（就是点的数量num）
        medrank->GenLines(num, num_line_);

        // 初始化查询条件的投影
        for (int i = 0; i < num; ++i) {
            // Calc q
            for (int j = 0; j < medrank->num_line(); ++j) {
                medrank->set_q(j, CalcProjection(dim, data[i], medrank->GetLine(j)));
            }
        }

        // 初始化l, h
        medrank->GenH();
        medrank->GenL();

        // 初始化候选人票数为0
        medrank->InitVotes();

        // 遍历所有线段
        for (int i = 0; i < num_line_; ++i) {
            float h_dis = abs(); // h的点与q的点的距离差（我不知道怎么获取节点的投影..）
            float l_dis = abs();

            if (h_dis <= l_dis) {
                int result = medrank->Vote(h_[i]); // 如果有票数过半的候选人就返回候选人，否则返回-1

                if (result != -1) {
                    // 已找到
                }

            } else if (h_dis > l_dis) {
                int result = medrank->Vote(l_[i]); // 如果有票数过半的候选人就返回候选人，否则返回-1

                if (result != -1) {
                    // 已找到
                }
            }
        }
    }
}