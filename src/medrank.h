#ifndef _MED_RANK_H_
#define _MED_RANK_H_

#include "b_tree.h"

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
    void Init(char* output_folder);

    void GenLines(int dim, int num_line);

    float* GetLine(int index);

    int num_line();

    void set_q(int index, float value);
private:
    // constructor
    MEDRANK();

    int* h_;
    int* l_;
    float* q_;
    char* index_path_;
    BTree** trees_;

    int num_line_;
    int dim_line_;
    float** lines_;
};

#endif // _MEDRANK_H_