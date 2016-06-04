#ifndef __B_TREE_H
#define __B_TREE_H

#include "b_node.h"
#include "b_tree.h"

// -----------------------------------------------------------------------------
//  BTree: b-tree to index ...
// -----------------------------------------------------------------------------
class BTree {
public:
    // constructor
    BTree();

    // destructor                      
    ~BTree();

    // -------------------------------------------------------------------------
    // init a new b-tree
    void Init(char* file_name, int block_length);

    // load an exist b-tree
    void InitRestore(char* file_name);

    // -------------------------------------------------------------------------
    // bulkload b-tree from hash table in mem
    // <num> -- number of entries
    int BulkLoad(Pair* pairs, int num);

private:
    // -------------------------------------------------------------------------
    int root_;                      // address of disk for root
    BlockFile* file_;               // file in disk to store
    BNode* root_ptr_;               // pointer of root

    // -------------------------------------------------------------------------
    // read <root> from buffer
    int ReadHeader(char* buf);

    // write <root> into buffer
    int WriteHeader(char* buf);

    // -------------------------------------------------------------------------
    // load root of b-tree
    void LoadRoot();

    // delete root of b-tree
    void DeleteRoot();

};

#endif
