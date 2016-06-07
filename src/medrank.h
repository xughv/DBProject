#include "b_tree.h"


class MEDRANK {
public:
    // constructor
    MEDRANK();
    // destructor
    ~MEDRANK();

    // -------------------------------------------------------------------------
    void Init(char* output_folder);

private:
    int* h_;
    int* l_;
    char* index_path_;
    BTree** trees_;
};