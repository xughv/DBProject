#ifndef __B_TREE_H
#define __B_TREE_H

#include "block_file.h"
#include "util.h"
#include "b_node.h"

class BNode;

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
    void InitFromFile(char* file_name);

    // -------------------------------------------------------------------------
    // bulkload b-tree from hash table in mem
    // <num>: number of entries
    void BulkLoad(Pair* pairs, int num);

    // -------------------------------------------------------------------------
    // get cursor not greater than key
    // <cursor>: (return)
    int Search(float key, Cursor *cursor);

    // -------------------------------------------------------------------------
    // load root of b-tree
    BNode* LoadRoot();

    // -------------------------------------------------------------------------
    // get <file_>
    BlockFile* file() const;

private:
    // -------------------------------------------------------------------------
    int root_block_;                // address of disk for root
    BlockFile* file_;               // file in disk to store
    BNode* root_ptr_;               // pointer of root

    // -------------------------------------------------------------------------
    // read <root> from buffer
    void ReadRootFromBuffer(char* buf);

    // write <root> into buffer
    void WriteRootToBuffer(char* buf);

    // -------------------------------------------------------------------------
    // delete root of b-tree
    void DeleteRoot();

};

#endif
