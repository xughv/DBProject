#include "ann.h"
#include <cstdlib>
#include <ctime>

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
void Indexing(int num, int dim, int num_line, int page_size, char* data_set, char* output_folder) {

    // Init the data set.
    unsigned char** data = new unsigned char*[num];
    for (int i = 0; i < num; i++) {
        data[i] = new unsigned char[dim];
        memset(data[i], 0, sizeof(unsigned char) * dim);
    }

    // Mark the start time of read the file.
    clock_t start_time = clock();

    // -------------------------------------------------------------------------
    // ReadSetFromFile
    // Open the file of <data_set> and read the data into <data>
    // If the file not exist, return false.
    // -------------------------------------------------------------------------
    if (!ReadSetFromFile(data_set, num, dim, data)) {
        printf("Read Data-Set Failed.\n");
        return;
    }

    // Mark the end time of read the file.
    clock_t end_time = clock();

    // Printf the time of read the file.
    printf ("Read Dataset: %.6f sec\n", ((float)end_time - start_time) / CLOCKS_PER_SEC);

    // Set the index output path. In the results/index/
    char* index_path = new char[strlen(output_folder) + 20];
    strcpy(index_path, output_folder);
    strcat(index_path, "index/");

    // -------------------------------------------------------------------------
    // CreateDirectory
    // Create a directory "results/index/".
    // In the Linux and OS X system can use the API in CreateDirectory function.
    // But not in Windows system, so, if you want to run the application in
    // windows, you can create the folder "results/index/" by yourself and comment
    // the function CreateDirectory.
    // -------------------------------------------------------------------------
    if (!CreateDirectory(index_path)) {
        printf("Create Directory Failed.\n");
        return;
    }

    // Get the instance of medrank.
    MEDRANK* medrank = MEDRANK::GetInstance();

    // -------------------------------------------------------------------------
    // InitVote
    // <num>: the number of candidate
    // -------------------------------------------------------------------------
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
        qsort(pairs, (size_t)num, sizeof(Pair), Pair::Compare);

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

    // ------------------------------------------------------------------------
    // Write the result in result.out
    // open file result.out
    char* output_path = new char[strlen(output_folder) + 20];
    strcpy(output_path, output_folder);
    strcat(output_path, "result.out");
    FILE* fp = fopen(output_path, "w");
    if (!fp)  {
        printf("Cannot open output file.\n");
    } else {
        fprintf(fp, "Indexing Time: %.6f sec\n", index_time / CLOCKS_PER_SEC);
    }
    fclose(fp);

    //-------------------------------------------------------------------------

    printf("Prepare Random Lines Time: %.6f sec\n\n", gen_lines_time / CLOCKS_PER_SEC);
    printf("Indexing Time: %.6f sec\n", index_time / CLOCKS_PER_SEC);

    // release space
    delete[] index_path;
    delete[] output_path;
    delete[] file_name;
    delete[] pairs;

    for (int i = 0; i < num; i++) {
        delete[] data[i];
    }
    delete[] data;
}


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

void CalcANN(int num, int dim, char* query_set, char* output_folder, int* result_id) {

    unsigned char**query_data = new unsigned char*[num];

    for (int i = 0; i < num; i++) {
        query_data[i] = new unsigned char[dim];
        memset(query_data[i], 0, sizeof(unsigned char) * dim);
    }

    if (!ReadSetFromFile(query_set, num, dim, query_data)) {
        printf("Read Query-Set Failed.\n");
        return;
    }

    MEDRANK* medrank = MEDRANK::GetInstance();
    medrank->Init(output_folder);

    int io_cost = 0;
    float running_time = 0.0f;

    // Init the projection of query set
    for (int i = 0; i < num; ++i) {
        // Set current q
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

    // ------------------------------------------------------------------------
    // Write the result in result.out
    // open file result.out
    char* output_path = new char[strlen(output_folder) + 20];
    strcpy(output_path, output_folder);
    strcat(output_path, "result.out");
    FILE* fp = fopen(output_path, "a+");
    if (!fp)  {
        printf("Cannot open output file.\n");
    } else {
        fprintf(fp, "Running Time:  %.6f sec\n", running_time / num);
        fprintf(fp, "IO Cost:       %d times\n", io_cost / num);
    }

    fclose(fp);

    //-------------------------------------------------------------------------

    printf("Running Time:  %.6f sec\n", running_time / num);
    printf("IO Cost:       %d times\n", io_cost / num);

    // release space
    for (int i = 0; i < num; i++) {
        delete[] query_data[i];
    }
    delete[] query_data;
    delete[] output_path;
}



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

void LinearScan(int num_data, int num_query, int dim, char* query_set, char* data_set, char* output_folder, int* result_id) {

    unsigned char** db_data = new unsigned char*[num_data];
    for (int i = 0; i < num_data; i++) {
        db_data[i] = new unsigned char[dim];
        memset(db_data[i], 0, sizeof(unsigned char) * dim);
    }

    unsigned char **q_data = new unsigned char *[num_query];
    for (int i = 0; i < num_query; i++) {
        q_data[i] = new unsigned char[dim];
        memset(q_data[i], 0, sizeof(unsigned char) * dim);
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


    // ------------------------------------------------------------------------
    // Write the result in result.out
    // open file result.out
    char* output_path = new char[strlen(output_folder) + 20];
    strcpy(output_path, output_folder);
    strcat(output_path, "result.out");
    FILE* fp = fopen(output_path, "a+");
    if (!fp)  {
        printf("Cannot open output file.\n");
    } else {
        fprintf(fp, "Overall Ratio: %.6f\n\n", overall_ratio / num_query);
        fprintf(fp, "The MEDRANK result:\n");
    }

    for (int i = 0; i < num_query; ++i) {
        fprintf(fp, "#%-3d: id = %d\n", i, result_id[i]);
    }

    fclose(fp);

    //-------------------------------------------------------------------------

    printf("Overall Ratio: %.6f\n", overall_ratio / num_query);

    // release space
    for (int i = 0; i < num_query; i++) {
        delete[] q_data[i];
    }
    delete[] q_data;

    for (int i = 0; i < num_data; i++) {
        delete[] db_data[i];
    }
    delete[] db_data;
    delete[] output_path;
}
