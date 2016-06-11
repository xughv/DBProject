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
    strcpy(dir, OUTPUT_DIR_DEFAULT);

    Indexing(num_data, dim, num_line, page_size, data_set, dir);
    CalcANN(num_query, dim, query_set, dir);
    LinearScan(num_data, num_query, dim, query_set, data_set);

    delete[] data_set;
    delete[] query_set;
    delete[] dir;
    return 0;
}