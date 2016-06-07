#include "ann.h"
#include "def.h"
#include <cstring>

int main() {
    int page_size = PAGE_SIZE_DEFAULT;
    int num = 60000;
    int dim = 784;
    int num_line = 50;
    char *file_name = new char[20];
    char *dir = new char[20];

    strcpy(file_name, "data/Mnist.ds");
    strcpy(dir, "results/");

    Indexing(num, dim, num_line, page_size, file_name, dir);

    return 0;
}