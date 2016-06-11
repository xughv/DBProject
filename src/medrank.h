#ifndef _MED_RANK_H_
#define _MED_RANK_H_

#include "b_tree.h"
#include <cstring>

class MEDRANK {
public:
    // Singleton
    static MEDRANK* GetInstance() {
        static MEDRANK instance;
        return &instance;
    }

    // destructor
    ~MEDRANK();
    // -------------------------------------------------------------------------
    void Init(char *output_folder);

    void InitCursor();

    void InitVote(int num);

    int Execute();

    void GenRandomVectors(int dim, int num_line);

    float* GetRandomVector(int index);

    int VoteAndJudge(int candidate);


    int num_line();

    void set_q(int index, float value);

    int io_cost();

private:
    // constructor
    MEDRANK();

    Cursor** h_;
    Cursor** l_;

    float* q_;
    BTree** trees_;

    int num_vector_;

    float** vectors_;

    int* votes_;
    int num_data_;

    int io_cost_;
};

#endif // _MEDRANK_H_