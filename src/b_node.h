#ifndef _B_NODE_H
#define _B_NODE_H

// -----------------------------------------------------------------------------
//  BNode: basic structure of node in B-tree
// -----------------------------------------------------------------------------
class BNode {
public:
    // constructor
    BNode();

    // destructor
    virtual ~BNode();

    // -------------------------------------------------------------------------
    // Init a new node, which not exist
    // <level> -- level (depth) in b-tree
    // <btree> -- b-tree of this node
    virtual void Init(int level, BTree* btree);                  

    // load an exist node from disk to init
    // <btree> -- b-tree of this node
    // <block> -- address of file of this node
    virtual void init_restore(BTree* btree, int block);

    // -------------------------------------------------------------------------
    // read a b-node from buffer
    // <buf> -- store info of a b-node
    virtual void read_from_buffer(char* buf);

    // write a b-node into buffer
    // <buf> -- store info of a b-node
    virtual void write_to_buffer(char* buf);

    // -------------------------------------------------------------------------
    // get entry size in b-node
    virtual int get_entry_size();
    
    // find pos just less than input key
    virtual int find_position_by_key(float key);

    // get <key> indexed by <index>
    virtual float get_key(int index);

    // -------------------------------------------------------------------------
    // get left sibling node
    virtual BNode* get_left_sibling();

    // get right sibling node
    virtual BNode* get_right_sibling();

    // -------------------------------------------------------------------------
    // get <block_>
    int get_block();

    // get <num_entries_>
    int get_num_entries();

    // get <level_>
    int get_level();

    // -------------------------------------------------------------------------
    // get header size in b-node
    int  get_header_size();

    // get key of this node
    float get_key_of_node();

    // whether is full?
    bool isFull();

    // -------------------------------------------------------------------------
    // set <left_sibling>
    void set_left_sibling(int left_sibling);

    // set <right sibling>
    void set_right_sibling(int right_sibling);

protected:
    // -------------------------------------------------------------------------
    char   level_;                  // level of b-tree (level > 0)
    int    num_entries_;            // number of entries in this node
    int    left_sibling_;           // addr in disk for left sibling
    int    right_sibling_;          // addr in disk for right sibling
    float* key_;                    // keys

    // -------------------------------------------------------------------------
    bool   dirty_;                  // if dirty, write back to file
    int    block_;                  // addr of disk for this node
    int    capacity_;               // max num of entries can be stored
    BTree* btree_;                  // b-tree of this node
};

// -----------------------------------------------------------------------------
//  BIndexNode: structure of index node in b-tree
// -----------------------------------------------------------------------------
class BIndexNode : public BNode {
public:
    BIndexNode();                   // constructor
    virtual ~BIndexNode();          // destructor

    // -------------------------------------------------------------------------
    // Init a new node, which not exist
    // <level> -- level (depth) in b-tree
    // <btree> -- b-tree of this node
    virtual void Init(int level, BTree* btree);                  

    // load an exist node from disk to init
    // <btree> -- b-tree of this node
    // <block> -- address of file of this node
    virtual void init_restore(BTree* btree, int block);

    // -------------------------------------------------------------------------
    // read a b-node from buffer
    // <buf> -- store info of a b-node
    virtual void read_from_buffer(char* buf);

    // write a b-node into buffer
    // <buf> -- store info of a b-node
    virtual void write_to_buffer(char* buf);

    // -------------------------------------------------------------------------
    // get entry size in b-node
    virtual int get_entry_size();

    // find pos just less than input key
    virtual int find_position_by_key(float key);

    // get <key_> indexed by <index>
    virtual float get_key(int index);

    // -------------------------------------------------------------------------
    // get left sibling node
    virtual BIndexNode* get_left_sibling();

    // get right sibling node
    virtual BIndexNode* get_right_sibling();

    // -------------------------------------------------------------------------
    // get <son_> indexed by <index>
    int get_son(int index);

    // -------------------------------------------------------------------------
    // add new child by its child node
    void add_new_child(float key,  int son);

protected:
    int* son_;                      // addr of son node
};


// -----------------------------------------------------------------------------
//  BLeafNode: structure of leaf node in b-tree
// -----------------------------------------------------------------------------
class BLeafNode : public BNode {
public:
    // constructor
    BLeafNode();

    // destructor
    virtual ~BLeafNode();

    // -------------------------------------------------------------------------
    // Init a new node, which not exist
    // <level> -- level (depth) in b-tree
    // <btree> -- b-tree of this node
    virtual void Init(int level, BTree* btree);                  

    // load an exist node from disk to init
    // <btree> -- b-tree of this node
    // <block> -- address of file of this node
    virtual void init_restore(BTree* btree, int block);

    // -------------------------------------------------------------------------
    // read a b-node from buffer
    // <buf> -- store info of a b-node
    virtual void read_from_buffer(char* buf);

    // write a b-node into buffer
    // <buf> -- store info of a b-node
    virtual void write_to_buffer(char* buf);

    // -------------------------------------------------------------------------
    // get entry size in b-node
    virtual int get_entry_size();
    
    // find pos just less than input key
    virtual int find_position_by_key(float key);

    // get <key_> indexed by <index>
    virtual float get_key(int index);

    // -------------------------------------------------------------------------
    // get left sibling node
    virtual BLeafNode* get_left_sibling();

    // get right sibling node
    virtual BLeafNode* get_right_sibling();

    // -------------------------------------------------------------------------
    // get key size of this node
    int get_key_size(int block_length);

    // get <increment>
    int get_increment();

    // get <num_keys_>
    int get_num_keys();             

    // get entry id indexed by <index>
    int get_entry_id(int index);

    // -------------------------------------------------------------------------
    // add new child by input id and key
    // <id>  -- input object id
    // <key> -- input key
    void add_new_child(int id, float key);

protected:
    int  num_keys_;                 // number of keys
    int* id_;                       // object id

    int capacity_keys_;             // max num of keys can be stored
};

#endif
