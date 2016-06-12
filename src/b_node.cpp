#include "b_node.h"
#include <cstring>

#include "b_tree.h"

// constructor
BNode::BNode() {
    level_ = -1;
    num_entries_ = 0;
    left_sibling_ = right_sibling_ = -1;
    key_ = NULL;

    block_ = capacity_ = -1;
    dirty_ = false;
    btree_ = NULL;
}

// destructor
BNode::~BNode() {
    // if dirty, rewrite to disk
    if (dirty_) {
        int block_length = btree_->file()->block_length();

        char* buf = new char[block_length];
        WriteToBuffer(buf);
        btree_->file()->WriteBlock(buf, block_);

        delete[] buf;
    }

    // release
    delete[] key_;
    key_ = NULL;

    delete[] son_;
    son_ = NULL;
}

// -------------------------------------------------------------------------
// Init a new node, which not exist
// <level>: level (depth) in b-tree
// <btree>: b-tree of this node
void BNode::Init(int level, BTree* btree) {
    level_ = (char)level;
    btree_ = btree;

    dirty_ = true;

    int block_length = btree_->file()->block_length();
    capacity_ = (block_length - GetHeaderSize()) / GetEntrySize();

    // init <key_>
    key_ = new float[capacity_];
    for (int i = 0; i < capacity_; i++) {
        key_[i] = FLOAT_MIN;
    }
    // init <son_>
    son_ = new int[capacity_];
    memset(son_, -1, sizeof(int) * capacity_);

    char* buf = new char[block_length];
    block_ = btree_->file()->AppendBlock(buf);
    delete[] buf;
}

// Load an exist node from disk to init
// <btree>: b-tree of this node
// <block>: address of file of this node
void BNode::InitFromFile(BTree* btree, int block) {

    btree_ = btree;
    block_ = block;

    int block_length = btree_->file()->block_length();
    capacity_ = (block_length - GetHeaderSize()) / GetEntrySize();

    // init <key_>
    key_ = new float[capacity_];
    for (int i = 0; i < capacity_; i++) {
        key_[i] = FLOAT_MIN;
    }
    // init <son_>
    son_ = new int[capacity_];
    memset(son_, -1, sizeof(int) * capacity_);

    char* buf = new char[block_length];

    btree_->file()->ReadBlock(buf, block);
    ReadFromBuffer(buf);

    delete[] buf;
}

// -------------------------------------------------------------------------
// Read a b-node from buffer
// <buf>: store info of a b-node
void BNode::ReadFromBuffer(char* buf) {
    int pos = 0;
    // read <level_>
    memcpy(&level_, &buf[pos], SIZE_CHAR);
    pos += SIZE_CHAR;
    // read <num_entries_>
    memcpy(&num_entries_, &buf[pos], SIZE_INT);
    pos += SIZE_INT;
    // read <left_sibling_>
    memcpy(&left_sibling_, &buf[pos], SIZE_INT);
    pos += SIZE_INT;
    // read <right_sibling_>
    memcpy(&right_sibling_, &buf[pos], SIZE_INT);
    pos += SIZE_INT;

    for (int i = 0; i < num_entries_; i++) {
        // read <key_>
        memcpy(&key_[i], &buf[pos], SIZE_FLOAT);
        pos += SIZE_FLOAT;
        // read <son_>
        memcpy(&son_[i], &buf[pos], SIZE_INT);
        pos += SIZE_INT;
    }
}

// Write a b-node into buffer
// <buf>: store info of a b-node
void BNode::WriteToBuffer(char* buf) {
    int pos = 0;
    // write <level_>
    memcpy(&buf[pos], &level_, SIZE_CHAR);
    pos += SIZE_CHAR;
    // write <num_entries_>
    memcpy(&buf[pos], &num_entries_, SIZE_INT);
    pos += SIZE_INT;
    // write <left_sibling_>
    memcpy(&buf[pos], &left_sibling_, SIZE_INT);
    pos += SIZE_INT;
    // write <right_sibling_>
    memcpy(&buf[pos], &right_sibling_, SIZE_INT);
    pos += SIZE_INT;

    for (int i = 0; i < num_entries_; i++) {
        // write <key_>
        memcpy(&buf[pos], &key_[i], SIZE_FLOAT);
        pos += SIZE_FLOAT;
        // write <son_>
        memcpy(&buf[pos], &son_[i], SIZE_INT);
        pos += SIZE_INT;
    }
}

// -------------------------------------------------------------------------
// Get entry size in b-node
int BNode::GetEntrySize() {
    return SIZE_FLOAT + SIZE_INT;
}

// -----------------------------------------------------------------------------
//  Find position of entry that is just less than or equal to input entry.
//  If input entry is smaller than all entry in this node, we'll return -1.
//  The scan order is from right to left.
// -----------------------------------------------------------------------------
// Find pos just less than input key
int BNode::FindPositionByKey(float key) {
    int pos = -1;
    for (int i = num_entries_ - 1; i >= 0; --i) {
        if (key_[i] <= key) {
            pos = i;
            break;
        }
    }
    return pos;
}

// Get <key_> indexed by <index>
float BNode::GetKey(int index) {
    if (index < 0 || index >= num_entries_) {
        // TODO: Error
    }
    return key_[index];
}

// Get <son_> indexed by <index>
int BNode::GetSon(int index) {
    if (index < 0 || index >= num_entries_) {
        // TODO: Error
    }
    return son_[index];
}

// -------------------------------------------------------------------------
// Get header size in b-node
int BNode::GetHeaderSize() {
    return SIZE_CHAR + SIZE_INT * 3;
}

// Get key of this node
float BNode::GetKeyOfNode() {
    return key_[0];
}

// Whether is full?
bool BNode::IsFull() {
    return num_entries_ >= capacity_;
}

// -------------------------------------------------------------------------
// Add new child by key and value of son
void BNode::AddNewChild(float key, int son) {
    if (num_entries_ >= capacity_) {
        // TODO: Error
    }

    key_[num_entries_] = key;
    son_[num_entries_] = son;

    num_entries_++;
    // node modified, <dirty_> is true
    dirty_ = true;
}

// -------------------------------------------------------------------------
// Get left sibling node
int BNode::left_sibling() {
    return left_sibling_;
}

// Get right sibling node
int BNode::right_sibling() {
    return right_sibling_;
}

// Set <left_sibling>
void BNode::set_left_sibling(BNode* node) {
    left_sibling_ = node->block();
}

// Set <right sibling>
void BNode::set_right_sibling(BNode* node) {
    right_sibling_ = node->block();
}

// -------------------------------------------------------------------------
// Get <block_>
int BNode::block() const {
    return block_;
}

// Get <num_entries_>
int BNode::num_entries() const {
    return num_entries_;
}

// Get <level_>
int BNode::level() const {
    return (int)level_;
}
