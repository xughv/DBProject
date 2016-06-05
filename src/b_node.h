#ifndef _B_NODE_H
#define _B_NODE_H

#include "block_file.h"

class BTree;
// -----------------------------------------------------------------------------
//  BNode: basic structure of node in B-tree
// -----------------------------------------------------------------------------

class BNode {
public:
    // constructor
    BNode();

    // destructor
    ~BNode();

    // -------------------------------------------------------------------------
    // Init a new node, which not exist
    // <level> -- level (depth) in b-tree
    // <btree> -- b-tree of this node
    void Init(int level, BTree* btree);                  

    // load an exist node from disk to init
    // <btree> -- b-tree of this node
    // <block> -- address of file of this node
    void InitFromFile(BTree* btree, int block);

    // -------------------------------------------------------------------------
    // read a b-node from buffer
    // <buf> -- store info of a b-node
    void ReadFromBuffer(char* buf);

    // write a b-node into buffer
    // <buf> -- store info of a b-node
    void WriteToBuffer(char* buf);

    // -------------------------------------------------------------------------
    // get entry size in b-node
    int getEntrySize();
    
    // find pos just less than input key
    int FindPositionByKey(float key);

    // get <key_> indexed by <index>
    float GetKey(int index);

    // get <son_> indexed by <index>
    float GetSon(int index);
    // -------------------------------------------------------------------------
    // get header size in b-node
    int GetHeaderSize();

    // get key of this node
    float GetKeyOfNode();

    // whether is full?
    bool IsFull();

    // -------------------------------------------------------------------------
    // add new child by key and value of son
    void AddNewChild(float key, int son);

    // -------------------------------------------------------------------------
    // get left sibling node
    BNode* left_sibling();

    // get right sibling node
    BNode* right_sibling();

    // set <left_sibling>
    void set_left_sibling(BNode* node);

    // set <right sibling>
    void set_right_sibling(BNode* node);

    // -------------------------------------------------------------------------
    // get <block_>
    int block() const;

    // get <num_entries_>
    int num_entries() const;

    // get <level_>
    int level() const;

private:
    // -------------------------------------------------------------------------
    char   level_;                  // level of b-tree (level > 0)
    int    num_entries_;            // number of entries in this node
    int    left_sibling_;           // addr in disk for left sibling
    int    right_sibling_;          // addr in disk for right sibling
    float* key_;                    // keys
    int*   son_;                    // addr of son node or object id

    // -------------------------------------------------------------------------
    bool   dirty_;                  // if dirty, write back to file
    int    block_;                  // addr of disk for this node
    int    capacity_;               // max num of entries can be stored
    BTree* btree_;                  // b-tree of this node
};

#endif
