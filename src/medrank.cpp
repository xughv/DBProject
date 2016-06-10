
#include "medrank.h"

MEDRANK::MEDRANK() {

    num_line_ = -1;

    h_ = l_ = NULL;
    q_ = NULL;

    trees_ = NULL;

    num_line_ = -1;
    dim_line_ = -1;
    lines_ = NULL;
    votes_ = NULL;
}

MEDRANK::~MEDRANK() {
    delete q_;
    q_ = NULL;

    for (int i = 0; i < num_line_; i++) {
        delete[] lines_[i];
        delete trees_[i];
        delete h_[i];
        delete l_[i];
    }
    delete[] lines_;
    lines_ = NULL;
    delete[] trees_;
    trees_ = NULL;

    delete h_;
    h_ = NULL;

    delete l_;
    l_ = NULL;

    delete votes_;
    votes_ = NULL;
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

    // initial <h_>
    h_ = new Cursor*[num_line_];

    // initial <l_>
    l_ = new Cursor*[num_line_];

    // reset all votes
    votes_ = new int[dim_line_];

    // initial <trees_>
    char* index_path = new char[strlen(output_folder) + 20];
    strcpy(index_path, output_folder);
    strcat(index_path, "index/");

    char* file_name = new char[20];

    trees_ = new BTree*[num_line_];
    for (int i = 0; i < num_line_; ++i) {
        // generate the file name of the b-tree
        GenTreeFileName(i, index_path, file_name);

        trees_[i] = new BTree();
        trees_[i]->InitFromFile(file_name);

        h_[i] = new Cursor();
        l_[i] = new Cursor();
    }

    delete[] index_path;
    delete[] file_name;
}

void MEDRANK::InitCursor() {
    for (int i = 0; i < num_line_; ++i) {
        trees_[i]->GetCursorNotGreaterThanKey(q_[i], h_[i]);
        trees_[i]->GetCursorGreaterThanKey(q_[i], l_[i]);
    }
    memset(votes_, 0, sizeof(int) * dim_line_);
}

int MEDRANK::Vote(int candidate) {
    votes_[candidate]++;
    // 如果候选人的票数超过线段数量的一半，返回候选人的号码，否则返回-1
    if (votes_[candidate] >= num_line_/2) {
        return candidate;
    }
    return -1;
}

int MEDRANK::num_line() {
    return num_line_;
}

void MEDRANK::set_q(int index, float value) {
    q_[index] = value;
}

int MEDRANK::GoGoGo() {
    // 遍历所有线段
    for (int i = 0; i < num_line_; ++i) {
        float h_dis = FLT_MAX;
        float l_dis = FLT_MAX;
        if (!h_[i]->invalid()) h_dis = q_[i] - h_[i]->projection();
        if (!l_[i]->invalid()) l_dis = l_[i]->projection() - q_[i];

        if (h_dis <= l_dis) {
            int result = Vote(h_[i]->id()); // 如果有票数过半的候选人就返回候选人，否则返回-1
            --(*h_[i]);
            if (result != -1) {
                // 已找到
                return result;
            }

        } else {
            int result = Vote(l_[i]->id()); // 如果有票数过半的候选人就返回候选人，否则返回-1
            ++(*l_[i]);
            if (result != -1) {
                // 已找到
                return result;
            }
        }
    }
    return -1;
}