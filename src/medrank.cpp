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

void MEDRANK::GenLines(int dim_line, int num_line) {
    num_line_ = num_line;
    dim_line_ = dim_line;
    lines_ = new float*[num_line];
    for (int i = 0; i < num_line; ++i) {
        lines_[i] = new float[dim_line];
        GenRandomVector(dim_line, lines_[i]);
    }
}

float* MEDRANK::GetLine(int index) {
    return lines_[index];
}

void MEDRANK::Init(char *output_folder) {
    q_ = new float[num_line_];
    
}

int MEDRANK::num_line() {
    return num_line_;
}

void MEDRANK::set_q(int index, float value) {
    q_[index] = value;
}
