#include "medrank.h"

MEDRANK::MEDRANK() {
    num_vector_ = -1;

    h_ = l_ = NULL;
    q_ = NULL;

    trees_ = NULL;

    num_vector_ = -1;

    vectors_ = NULL;
    votes_ = NULL;

    io_cost_ = 0;
}

MEDRANK::~MEDRANK() {
    delete[] q_;
    q_ = NULL;

    for (int i = 0; i < num_vector_; i++) {
        delete[] vectors_[i];
        delete trees_[i];
        delete h_[i];
        delete l_[i];
    }
    delete[] vectors_;
    vectors_ = NULL;

    delete[] trees_;
    trees_ = NULL;

    delete[] h_;
    h_ = NULL;

    delete[] l_;
    l_ = NULL;

    delete[] votes_;
    votes_ = NULL;
}

// initial votes
void MEDRANK::InitVote(int num) {
    num_data_ = num;
    votes_ = new int[num];
};

void MEDRANK::GenRandomVectors(int dim_line, int num_line) {
    num_vector_ = num_line;
    vectors_ = new float*[num_line];
    for (int i = 0; i < num_line; ++i) {
        vectors_[i] = new float[dim_line];
        GenRandomVector(dim_line, vectors_[i]);
    }
}

float* MEDRANK::GetRandomVector(int index) {
    return vectors_[index];
}

void MEDRANK::Init(char *output_folder) {

    q_ = new float[num_vector_];

    // initial <h_>
    h_ = new Cursor*[num_vector_];

    // initial <l_>
    l_ = new Cursor*[num_vector_];

    // initial <trees_>
    char* index_path = new char[strlen(output_folder) + 20];
    strcpy(index_path, output_folder);
    strcat(index_path, "index/");

    char* file_name = new char[20];

    trees_ = new BTree*[num_vector_];
    for (int i = 0; i < num_vector_; ++i) {
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

// init <h_> and <l_> and reset value
void MEDRANK::InitCursor() {
    // reset all votes
    memset(votes_, 0, sizeof(int) * num_data_);
    // reset io cost
    io_cost_ = 0;

    // init <h_> and <l_>
    for (int i = 0; i < num_vector_; ++i) {
        // release the node which use last time
        h_[i]->Release();
        l_[i]->Release();

        io_cost_ += trees_[i]->Search(q_[i], h_[i]);

        // h_[i] is invalid (less than the min of key in tree)
        if (h_[i]->invalid()) {
            float key = trees_[i]->LoadRoot()->GetKeyOfNode();
            io_cost_ += trees_[i]->Search(key, l_[i]);
        } else {
            // l_[i] = h_[i] move right
            (*l_[i]) = (*h_[i]);
            io_cost_ += l_[i]->MoveRight() + 1;
        }
    }
}

int MEDRANK::VoteAndJudge(int candidate) {
    votes_[candidate]++;
    // 如果候选人的票数超过线段数量的一半，返回候选人的号码，否则返回-1
    if ((float)votes_[candidate] / num_vector_ > MINFREQ) {
        return candidate;
    }
    return -1;
}

int MEDRANK::Execute() {
    // 遍历所有线段
    for (int i = 0; i < num_vector_; ++i) {
        float h_dis = FLT_MAX;
        float l_dis = FLT_MAX;
        if (!h_[i]->invalid()) h_dis = q_[i] - h_[i]->projection();
        if (!l_[i]->invalid()) l_dis = l_[i]->projection() - q_[i];

        if (h_dis <= l_dis) {
            int result = VoteAndJudge(h_[i]->id()); // 如果有票数过半的候选人就返回候选人，否则返回-1
            io_cost_ += h_[i]->MoveLeft();
            if (result != -1) {
                // 已找到
                return result;
            }
        } else {
            int result = VoteAndJudge(l_[i]->id()); // 如果有票数过半的候选人就返回候选人，否则返回-1
            io_cost_ += l_[i]->MoveRight();
            if (result != -1) {
                // 已找到
                return result;
            }
        }
        // next round
        if (i == num_vector_ - 1) i = 0;
    }
    return -1;
}


int MEDRANK::num_line() {
    return num_vector_;
}

void MEDRANK::set_q(int index, float value) {
    q_[index] = value;
}

int MEDRANK::io_cost() {
    return io_cost_;
}