#include "ann.h"
#include "def.h"

int main() {
    int page_size = PAGE_SIZE_DEFAULT;
    int num = 60000;
    int dim = 784;
    int num_line = 50;
    Indexing(num, dim, num_line, page_size, "data/Mnist.ds", "results/");
//
//    MEDRANK* medrank = new MEDRANK();
//    medrank->Init();
//    medrank->Calc();
}