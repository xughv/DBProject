#include "ann.h"
#include <cstdlib>

void Indexing(int num, int dim, int num_line, int page_size, char* data_set, char* output_folder) {

    // 新建data数据unsigned型二维数组
    unsigned** data = new unsigned*[num];
    for (int i = 0; i < num; i++) {
        data[i] = new unsigned[dim];
        memset(data[i], 0, sizeof(unsigned) * dim);
    }

    // 从文件获取数据至data二维数组
    if (!ReadSetFromFile(data_set, num, dim, data)) {
        // TODO: Error
        printf("Read DataSet Failed.\n");
    } else {

        // 构建路径名称
        char* index_path = new char[strlen(output_folder) + 20];
        strcpy(index_path, output_folder);
        strcat(index_path, "index/");

        if (!CreateDirectory(index_path)) {
            printf("Create Directory Failed.\n");
            // TODO: Error
        }

        MEDRANK* medrank = MEDRANK::GetInstance();

        // 生成随机线段，参数（线的维数，线的数量）
        medrank->GenLines(dim, num_line);

        // 生成num对<id, projection>
        Pair* pairs = new Pair[num];

        char* file_name = new char[20];

        for (int i = 0; i < num_line; ++i) {
            for (int j = 0; j < num; ++j) {
                pairs[j].SetValue(j, CalcProjection(dim, data[j], medrank->GetLine(i)));
            }
            // sort
            qsort(pairs, num, sizeof(Pair), Compare);


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

    unsigned **query_data = new unsigned *[num];
    int* result = new int[num];
    for (int i = 0; i < num; i++) {
        query_data[i] = new unsigned[dim];
        memset(query_data[i], 0, sizeof(query_data[i]) * dim);
    }

    if (!ReadSetFromFile(query_set, num, dim, query_data)) {
        // TODO: Error
    } else {

        MEDRANK* medrank = MEDRANK::GetInstance();
        medrank->Init();

        // 初始化查询条件的投影
        for (int i = 0; i < num; ++i) {
            // Calc q
            for (int j = 0; j < medrank->num_line(); ++j) {
                medrank->set_q(j, CalcProjection(dim, query_data[i], medrank->GetLine(j)));
            }
            result[i] = medrank->GoGoGo();
        }

        // 输出到文件
        char* file_name = new char[20];
        strcpy(file_name, output_folder);
        strcat(file_name, "medrank.out");

        // open data file
        FILE* fp = fopen(file_name, "w");

        if (!fp) {
            printf("I could not open %s.\n", file_name);
            return;
        }

        for (int i = 0; i < num; ++i) {
            fprintf(fp, "%d\n", result[i]);
        }

        fclose(fp);

        delete[] result;
    }

    // 清除查询数据二维指针
    for (int i = 0; i < num; i++) {
        delete[] query_data[i];
    }
    delete [] query_data;
}

void LinearScan(int num, int dim, char* query_set, char* data_set) {

    // 读取查询数据
    int num_q_data = 100;
    unsigned **q_data = new unsigned *[num_q_data];
    for (int i = 0; i < num_q_data; i++) {
        q_data[i] = new unsigned[dim];
        memset(q_data[i], 0, sizeof(q_data[i]) * dim);
    }

    // 读取数据
    unsigned** db_data = new unsigned*[num];
    for (int i = 0; i < num; i++) {
        db_data[i] = new unsigned[dim];
        memset(db_data[i], 0, sizeof(unsigned) * dim);
    }

    if (!ReadSetFromFile(query_set, num_q_data, dim, q_data)) {
        // TODO: Error
    } else if (!ReadSetFromFile(data_set, num, dim, db_data)) {
        // TODO: Error
    } else {
        float min_dis = FLT_MAX;
        int *min_point_index = new int[num_q_data];
        memset(min_point_index, 0, sizeof(min_point_index) * num_q_data);

        for (int i = 0; i < num_q_data; ++i) {
            min_dis = FLT_MAX;
            for (int j = 0; j < num; ++j) {
                float dis = CalcPointsDistance(q_data[i], db_data[j], dim);
                if (min_dis > dis) {
                    min_dis = dis;
                    min_point_index[i] = j;
                }
            }
        }

        for (int i = 0; i < 100; ++i) {
            printf("num: %d , id: %d\n", i, min_point_index[i]);
        }


        // 清除min_point_index指针
        delete [] min_point_index;

    }

    // 清除查询数据二维指针
    for (int i = 0; i < num_q_data; i++) {
        delete[] q_data[i];
    }
    delete [] q_data;

    // 清除数据二维指针
    for (int i = 0; i < num; i++) {
        delete[] db_data[i];
    }
    delete [] db_data;

}