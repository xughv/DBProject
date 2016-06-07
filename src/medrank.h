#include "b_tree.h"

class MEDRANK {
public:
    // Singleton
    static MEDRANK* GetInstance() {
        if (instance_ == NULL) instance_ = new MEDRANK();
        return instance_;
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

    static MEDRANK* instance_;

    int dim_;

    int* h_;
    int* l_;
    float* q_;
    char* index_path_;
    BTree** trees_;

    int num_line_;
    float** lines_;
};
