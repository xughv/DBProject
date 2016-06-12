#include "ann.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {
    // set random seed
    srand((unsigned)time(NULL));

    int page_size = PAGE_SIZE_DEFAULT;
    int num_data  = 60000;
    int num_query = 100;
    int dim       = 784;
    int num_line  = NUM_RANDOM_LINE_DEFAULT;

    char *data_set  = new char[20];
    char *query_set = new char[20];
    char *dir       = new char[20];

    strcpy(data_set, "data/Mnist.ds");
    strcpy(query_set, "data/Mnist.q");
<<<<<<< HEAD
    strcpy(dir, "result/");
=======
    strcpy(dir, OUTPUT_DIR_DEFAULT);

    int* result_id = new int[num_query];

    Indexing(num_data, dim, num_line, page_size, data_set, dir);
    CalcANN(num_query, dim, query_set, dir, result_id);
    LinearScan(num_data, num_query, dim, query_set, data_set, result_id);
>>>>>>> refs/remotes/origin/master

    delete[] data_set;
    delete[] query_set;
    delete[] dir;
    delete[] result_id;
    return 0;
}