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

    // Load an exist node from disk to init
    // <btree>: b-tree of this node
    // <block>: address of file of this node
    void InitFromFile(BTree* btree, int block);

    // -------------------------------------------------------------------------
    // Read a b-node from buffer
    // <buf>: store info of a b-node
    void ReadFromBuffer(char* buf);

    // Write a b-node into buffer
    // <buf>: store info of a b-node
    void WriteToBuffer(char* buf);

    // -------------------------------------------------------------------------
    // Get entry size in b-node
    int GetEntrySize();
    
    // Find pos just less than input key
    int FindPositionByKey(float key);

    // Get <key_> indexed by <index>
    float GetKey(int index);

    // Get <son_> indexed by <index>
    int GetSon(int index);
    // -------------------------------------------------------------------------
    // Get header size in b-node
    int GetHeaderSize();

    // Get key of this node
    float GetKeyOfNode();

    // Whether is full?
    bool IsFull();

    // -------------------------------------------------------------------------
    // Add new child by key and value of son
    void AddNewChild(float key, int son);

    // -------------------------------------------------------------------------
    // Get left sibling node
    int left_sibling();

    // Get right sibling node
    int right_sibling();

    // Set <left_sibling>
    void set_left_sibling(BNode* node);

    // Set <right sibling>
    void set_right_sibling(BNode* node);

    // -------------------------------------------------------------------------
    // Get <block_>
    int block() const;

    // Get <num_entries_>
    int num_entries() const;

    // Get <level_>
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
