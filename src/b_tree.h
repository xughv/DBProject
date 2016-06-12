#ifndef __B_TREE_H
#define __B_TREE_H

#include "block_file.h"
#include "util.h"
#include "b_node.h"

class BTree {
public:
    // constructor
    BTree();

    // destructor                      
    ~BTree();

    // -------------------------------------------------------------------------
    // Init a new b-tree
    void Init(char* file_name, int block_length);

    // Load an exist b-tree
    void InitFromFile(char* file_name);

    // -------------------------------------------------------------------------
    // Bulkload b-tree from hash table in mem
    // <num>: number of entries
    void BulkLoad(Pair* pairs, int num);

    // -------------------------------------------------------------------------
    // Get cursor not greater than key
    // <cursor>: (return)
    int Search(float key, Cursor *cursor);

    // -------------------------------------------------------------------------
    // Load root of b-tree
    BNode* LoadRoot();

    // -------------------------------------------------------------------------
    // Get <file_>
    BlockFile* file() const;

private:
    // -------------------------------------------------------------------------
    int root_block_;                // address of disk for root
    BlockFile* file_;               // file in disk to store
    BNode* root_ptr_;               // pointer of root

    // -------------------------------------------------------------------------
    // Read <root> from buffer
    void ReadRootFromBuffer(char* buf);

    // Write <root> into buffer
    void WriteRootToBuffer(char* buf);

    // -------------------------------------------------------------------------
    // Delete root of b-tree
    void DeleteRoot();

};

#endif
