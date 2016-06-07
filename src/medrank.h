#include "b_tree.h"


class MEDRANK {
public:
    // constructor
    MEDRANK();
    // destructor
    ~MEDRANK();

    // -------------------------------------------------------------------------
    void Init();

private:
    int* h;
    int* l;
    BTree** trees_;
};