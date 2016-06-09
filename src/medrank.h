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
    void Init(char* output_folder);
    int GoGoGo();

    void GenLines(int dim, int num_line);

    float* GetLine(int index);

    int Vote(int candidate);

    int num_line();
    int dim_line();

    void set_q(int index, float value);


private:
    // constructor
    MEDRANK();

    Cursor* h_;
    Cursor* l_;

    float* q_;
    char* index_path_;
    BTree** trees_;

    int num_line_;
    int dim_line_;
    float** lines_;

    int* votes_;
};

#endif // _MEDRANK_H_