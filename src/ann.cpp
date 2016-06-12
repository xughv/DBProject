#include "ann.h"
#include <cstdlib>
#include <ctime>

void Indexing(int num, int dim, int num_line, int page_size, char* data_set, char* output_folder) {

    // 新建data数据unsigned型二维数组
    unsigned** data = new unsigned*[num];
    for (int i = 0; i < num; i++) {
        data[i] = new unsigned[dim];
        memset(data[i], 0, sizeof(unsigned) * dim);
    }

    clock_t start_time = clock();

    // 从文件获取数据至data二维数组
    if (!ReadSetFromFile(data_set, num, dim, data)) {
<<<<<<< HEAD
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

        // 初始化候选人
        medrank->InitVote(num);

        // 生成num对<id, projection>
        Pair* pairs = new Pair[num];

        char* file_name = new char[20];
=======
        printf("Read DataSet-Failed.\n");
        return;
    }
>>>>>>> refs/remotes/origin/master

    clock_t end_time = clock();
    printf ("Read Dataset: %.6f sec\n", ((float)end_time - start_time) / CLOCKS_PER_SEC);

    // 构建路径名称
    char* index_path = new char[strlen(output_folder) + 20];
    strcpy(index_path, output_folder);
    strcat(index_path, "index/");

    if (!CreateDirectory(index_path)) {
        printf("Create Directory Failed.\n");
        return;
    }

    MEDRANK* medrank = MEDRANK::GetInstance();
    medrank->InitVote(num);
    // -------------------------------------------------------------------------
    start_time = clock();

    // generate <num_line>(50) random vectors
    medrank->GenRandomVectors(dim, num_line);
    end_time = clock();
    float gen_lines_time = ((float)end_time - start_time);

    // -------------------------------------------------------------------------
    // pairs on line[i-th]
    Pair* pairs = new Pair[num];

    char* file_name = new char[20];

    float index_time = 0.0f;
    for (int i = 0; i < num_line; ++i) {
        // -------------------------------------------------------------------------
        // generate pairs on line[i]
        // -------------------------------------------------------------------------
        start_time = clock();
        // calc the projection of all node on the i-th line
        for (int j = 0; j < num; ++j) {
            pairs[j].SetValue(j, CalcProjection(dim, data[j], medrank->GetRandomVector(i)));
        }
        // sort line
        qsort(pairs, (size_t)num, sizeof(Pair), Compare);

        end_time = clock();
        gen_lines_time += ((float)end_time - start_time);
        // -------------------------------------------------------------------------

        // generate the file name of the b-tree
        GenTreeFileName(i, index_path, file_name);

        // delete index file created before
        remove(file_name);

        // -------------------------------------------------------------------------
        // building the B+-tree
        // -------------------------------------------------------------------------
        start_time = clock();

        // build a b+-tree
        BTree* btree = new BTree();
        btree->Init(file_name, page_size);
        btree->BulkLoad(pairs, num);
        delete btree;

        end_time = clock();
        index_time += ((float)end_time - start_time);
        // -------------------------------------------------------------------------
    }

    printf("Prepare Random Lines Time: %.6f sec\n\n", gen_lines_time / CLOCKS_PER_SEC);
    printf("Indexing Time: %.6f sec\n", index_time / CLOCKS_PER_SEC);

    // release space
    delete[] index_path;
    delete[] file_name;
    delete[] pairs;

    for (int i = 0; i < num; i++) {
        delete[] data[i];
    }
    delete[] data;
}

void CalcANN(int num, int dim, char* query_set, char* output_folder, int* result_id) {

    unsigned **query_data = new unsigned *[num];

    for (int i = 0; i < num; i++) {
        query_data[i] = new unsigned[dim];
        memset(query_data[i], 0, sizeof(unsigned) * dim);
    }

    if (!ReadSetFromFile(query_set, num, dim, query_data)) {
        printf("Read Query-Set Failed.\n");
        return;
    }

    MEDRANK* medrank = MEDRANK::GetInstance();
    medrank->Init(output_folder);

    int io_cost = 0;
    float running_time = 0.0f;

    // 初始化查询条件的投影
    for (int i = 0; i < num; ++i) {
        // set current q
        for (int j = 0; j < medrank->num_line(); ++j) {
            medrank->set_q(j, CalcProjection(dim, query_data[i], medrank->GetRandomVector(j)));
        }
        clock_t start_time = clock();

        medrank->InitCursor();
        result_id[i] = medrank->Execute();

        io_cost += medrank->io_cost();
        clock_t end_time = clock();
        running_time += ((float)end_time - start_time) / CLOCKS_PER_SEC;
    }

    printf("Running Time:  %.6f sec\n", running_time / num);
    printf("IO Cost:       %d times\n", io_cost / num);

    // release space
    for (int i = 0; i < num; i++) {
        delete[] query_data[i];
    }
    delete[] query_data;
}

void LinearScan(int num_data, int num_query, int dim, char* query_set, char* data_set, int* result_id) {

    unsigned** db_data = new unsigned*[num_data];
    for (int i = 0; i < num_data; i++) {
        db_data[i] = new unsigned[dim];
        memset(db_data[i], 0, sizeof(unsigned) * dim);
    }

    unsigned **q_data = new unsigned *[num_query];
    for (int i = 0; i < num_query; i++) {
        q_data[i] = new unsigned[dim];
        memset(q_data[i], 0, sizeof(q_data[i]) * dim);
    }

    if (!ReadSetFromFile(query_set, num_query, dim, q_data)) {
        printf("Read Query-Set Failed.\n");
        return;
    }

    if (!ReadSetFromFile(data_set, num_data, dim, db_data)) {
        printf("Read Data-Set Failed.\n");
        return;
    }

    float overall_ratio = 0.0f;

    for (int i = 0; i < num_query; ++i) {
        float min_dis = FLT_MAX;
        // calc the ANN
        for (int j = 0; j < num_data; ++j) {
            float cur_dis = CalcPointsDistance(q_data[i], db_data[j], dim);
            if (cur_dis < min_dis) min_dis = cur_dis;
        }

        // calc distance by the result of MEDRANK
        float dis = CalcPointsDistance(q_data[i], db_data[result_id[i]], dim);

        // calc the overall ratio
        overall_ratio += dis / min_dis;
    }

    printf("Overall Ratio: %.6lf\n", overall_ratio / num_query);

    // release space
    for (int i = 0; i < num_query; i++) {
        delete[] q_data[i];
    }
    delete[] q_data;

    for (int i = 0; i < num_data; i++) {
        delete[] db_data[i];
    }
    delete[] db_data;
}

//        // 输出到文件
//        char* file_name = new char[strlen(output_folder) + 20];
//        strcpy(file_name, output_folder);
//        strcat(file_name, "medrank.out");
//
//        // open data file
//        FILE* fp = fopen(file_name, "w");
//
//        if (!fp) {
//            printf("I could not open %s.\n", file_name);
//            return;
//        }
//
//        for (int i = 0; i < num; ++i) {
//            fprintf(fp, "%d\n", result_id[i]);
//        }
//
//        fclose(fp);
//
//        delete[] file_name;