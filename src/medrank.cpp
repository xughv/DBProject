#include "medrank.h"

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