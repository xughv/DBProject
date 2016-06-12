#include "ann.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {
    // set random seed
    srand((unsigned)time(NULL));

    int page_size = PAGE_SIZE_DEFAULT;
    int num_data  = -1;
    int num_query = -1;
    int dim       = -1;
    int num_line  = NUM_RANDOM_LINE_DEFAULT;

    char data_set[20];
    char query_set[20];
    char dir[20];

    bool failed = false;
    int  cnt = 1;

    while (cnt < argc && !failed) {
        if (strcmp(argv[cnt], "-n") == 0) {
            num_data = atoi(argv[++cnt]);
            printf("num_data = %d\n", num_data);
            if (num_data <= 0) {
                failed = true;
                break;
            }
        }
        else if (strcmp(argv[cnt], "-d") == 0) {
            dim = atoi(argv[++cnt]);
            printf("dim = %d\n", dim);
            if (dim <= 0) {
                failed = true;
                break;
            }
        }
        else if (strcmp(argv[cnt], "-qn") == 0) {
            num_query = atoi(argv[++cnt]);
            printf("num_query = %d\n", num_query);
            if (num_query <= 0) {
                failed = true;
                break;
            }
        }
        else if (strcmp(argv[cnt], "-ds") == 0) {
            strncpy(data_set, argv[++cnt], sizeof(data_set));
            printf("data set = %s\n", data_set);
        }
        else if (strcmp(argv[cnt], "-qs") == 0) {
            strncpy(query_set, argv[++cnt], sizeof(query_set));
            printf("query set = %s\n", query_set);
        }
        else {
            failed = true;
            printf("Parameters error!\n");
            break;
        }
        cnt++;
    }
    printf("\n");

    strcpy(dir, OUTPUT_DIR_DEFAULT);
    int* result_id = new int[num_query];

    Indexing(num_data, dim, num_line, page_size, data_set, dir);
    CalcANN(num_query, dim, query_set, dir, result_id);
    LinearScan(num_data, num_query, dim, query_set, data_set, result_id);

//    delete[] data_set;
//    delete[] query_set;
//    delete[] dir;
    delete[] result_id;
    return 0;
}