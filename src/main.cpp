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

    strcpy(dir, OUTPUT_DIR_DEFAULT); // 默认输出文件夹

    int  cnt = 1;

    while (cnt < argc) {
        if (strcmp(argv[cnt], "-n") == 0) {
            num_data = atoi(argv[++cnt]);
            printf("Data Num    = %d\n", num_data);
        }
        else if (strcmp(argv[cnt], "-d") == 0) {
            dim = atoi(argv[++cnt]);
            printf("Dim         = %d\n", dim);
        }
        else if (strcmp(argv[cnt], "-qn") == 0) {
            num_query = atoi(argv[++cnt]);
            printf("Query Num   = %d\n", num_query);
        }
        else if (strcmp(argv[cnt], "-ds") == 0) {
            strncpy(data_set, argv[++cnt], sizeof(data_set));
            printf("Data Set    = %s\n", data_set);
        }
        else if (strcmp(argv[cnt], "-qs") == 0) {
            strncpy(query_set, argv[++cnt], sizeof(query_set));
            printf("Query Set   = %s\n", query_set);
        }
        else if (strcmp(argv[cnt], "-of") == 0) {
            strncpy(dir, argv[++cnt], sizeof(dir));
            printf("Output Dir  = %s\n", dir);
        }
        else {
            printf("Parameters error! Some parameters are invalid. \n");
            printf("Please entry these parameters: -n -d -qn -ds -qs \n");
            return 0;
        }
        cnt++;
    }

    if (num_data <= 0 || dim <= 0 || num_query <= 0) {
        printf("Parameters error! Some parameters are missed or the value is smaller than zero\n");
        return 0;
    }

    int* result_id = new int[num_query];

    Indexing(num_data, dim, num_line, page_size, data_set, dir);
    CalcANN(num_query, dim, query_set, dir, result_id);
    LinearScan(num_data, num_query, dim, query_set, data_set, dir, result_id);


    delete[] result_id;
    return 0;
}