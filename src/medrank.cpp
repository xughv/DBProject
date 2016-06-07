#include "medrank.h"

MEDRANK::MEDRANK() {
    dim_ = -1;

    h_ = l_ = NULL;
    q_ = NULL;
    index_path_ = NULL;
    trees_ = NULL;

    num_line_ = -1;
    lines_ = NULL;
}

MEDRANK::~MEDRANK() {

}

void MEDRANK::GenLines(int dim, int num_line) {
    num_line_ = num_line;
    dim_ = dim;
    lines_ = new float*[num_line];
    for (int i = 0; i < num_line; ++i) {
        lines_[i] = new float[dim];
        GenRandomVector(dim, lines_[i]);
    }
}

float* MEDRANK::GetLine(int index) {
    return lines_[index];
}

void MEDRANK::Init(char *output_folder) {

}