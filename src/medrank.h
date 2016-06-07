#ifndef _MED_RANK_H_
#define _MED_RANK_H_

#include "b_tree.h"

class MEDRANK {
public:
    // Singleton
    static MEDRANK* GetInstance() {
        static MEDRANK* instance;
        return instance;
    }
    // -------------------------------------------------------------------------
    void Init(char* output_folder);

    void GenLines(int dim, int num_line);

    float* GetLine(int index);
private:
    // constructor
    MEDRANK();
    // destructor
    ~MEDRANK();

    int dim_;

    int* h_;
    int* l_;
    float* q_;
    char* index_path_;
    BTree** trees_;

    int num_line_;
    float** lines_;
};

#endif // _MEDRANK_H_