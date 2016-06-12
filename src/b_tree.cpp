#include "b_tree.h"

#include <cstring>

#include "b_node.h"

// -----------------------------------------------------------------------------
//  BTree: b-tree to index
// -----------------------------------------------------------------------------

BTree::BTree() {
    root_block_ = -1;
    file_ = NULL;
    root_ptr_ = NULL;
}

// destructor                      
BTree::~BTree() {
    if (file_ != NULL) {
        char* header = new char[file_->block_length()];
        WriteRootToBuffer(header);
        file_->SetHeader(header);

        delete[] header;

        delete file_;
        file_ = NULL;
    }

    DeleteRoot();
}

// -------------------------------------------------------------------------
// init a new b-tree
void BTree::Init(char* file_name, int block_length) {

    file_ = new BlockFile(file_name, block_length);

    // -------------------------------------------------------------------------
    //  Init the first node: to store <blocklength> (page size of a node),
    //  <number> (number of nodes including both index node and leaf node),
    //  and <root> (address of root node)
    // -------------------------------------------------------------------------
    root_ptr_ = new BNode();
    root_ptr_->Init(0, this);

    root_block_ = root_ptr_->block();

    DeleteRoot();
}

// load an exist b-tree
void BTree::InitFromFile(char* file_name) {
    // check whether the file exists
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error log\n");
        return;
    }
    fclose(file);

    file_ = new BlockFile(file_name);

    char* header = new char[file_->block_length()];
    file_->ReadHeader(header);
    ReadRootFromBuffer(header);

    delete[] header;
}

// -------------------------------------------------------------------------
// read <root> from buffer
void BTree::ReadRootFromBuffer(char* buf) {
    memcpy(&root_block_, buf, SIZE_INT);
}

// write <root> into buffer
void BTree::WriteRootToBuffer(char* buf) {
    memcpy(buf, &root_block_, SIZE_INT);
}

// -------------------------------------------------------------------------
// load root of b-tree
BNode* BTree::LoadRoot() {
    if (root_ptr_ == NULL) {
        root_ptr_ = new BNode();
        root_ptr_->InitFromFile(this, root_block_);
    }
    return root_ptr_;
}

// delete root of b-tree
void BTree::DeleteRoot() {
    if (root_ptr_ != NULL) {
        delete root_ptr_;
        root_ptr_ = NULL;
    }
}

// -------------------------------------------------------------------------
// get <file_>
BlockFile* BTree::file() const {
    return file_;
}

// -------------------------------------------------------------------------
// bulkload b-tree from hash table in mem
// <num>: number of entries
void BTree::BulkLoad(Pair* pairs, int num) {

    int start_block = -1;
    int end_block   = -1;

    BNode* prev_node = NULL;
    BNode* cur_node  = NULL;

    bool first_node = true;

    // ---------------------------------------------------------------------
    //  Build leaf node from <pairs> (level = 0)
    // ---------------------------------------------------------------------
    for (int i = 0; i < num; ++i) {

        int id = pairs[i].id();
        float key = pairs[i].projection();

        if (cur_node == NULL) {
            cur_node = new BNode();
            cur_node->Init(0, this);

            if (first_node) {
                first_node = false;
                start_block = cur_node->block();
            } else {
                cur_node->set_left_sibling(prev_node);
                prev_node->set_right_sibling(cur_node);

                delete prev_node;
                prev_node = NULL;
            }
            end_block = cur_node->block();
        }
        cur_node->AddNewChild(key, id);

        if (cur_node->IsFull()) {
            prev_node = cur_node;
            cur_node = NULL;
        }
    }

    // release space
    delete prev_node;
    prev_node = NULL;

    delete cur_node;
    cur_node = NULL;

    // -------------------------------------------------------------------------
    //  Build the b-tree level by level
    // -------------------------------------------------------------------------
    int cur_level = 1;
    int last_start_block = start_block;
    int last_end_block   = end_block;

    // Stop consition: lastEndBlock == lastStartBlock (only one node, as root)
    while (last_end_block > last_start_block) {
        first_node = true;
        for (int block = last_start_block; block <= last_end_block; ++block) {

            BNode* child = new BNode();
            child->InitFromFile(this, block);

            float key = child->GetKeyOfNode();

            delete child;

            if (cur_node == NULL) {
                cur_node = new BNode();
                cur_node->Init(cur_level, this);

                if (first_node) {
                    first_node = false;
                    start_block = cur_node->block();
                } else {
                    cur_node->set_left_sibling(prev_node);
                    prev_node->set_right_sibling(cur_node);

                    delete prev_node;
                    prev_node = NULL;
                }
                end_block = cur_node->block();
            }
            cur_node->AddNewChild(key, block);

            if (cur_node->IsFull()) {
                prev_node = cur_node;
                cur_node = NULL;
            }
        }

        // next level
        cur_level++;
        last_start_block = start_block;
        last_end_block   = end_block;

        // release the space
        delete prev_node;
        prev_node = NULL;
        delete cur_node;
        cur_node = NULL;
    }
    root_block_ = last_start_block;
}

// -------------------------------------------------------------------------
// get cursor not greater than key
// <cursor>: (return)
int BTree::Search(float key, Cursor *cursor) {
    int io_cost = 0;

    BNode* cur_node = LoadRoot();
    io_cost++;

    // search in index nodes
    while (cur_node->level() > 0) {
        int pos = cur_node->FindPositionByKey(key);
        // check whether the key less than min key or not
        if (pos < 0) {
            // key < min_key of the tree
            root_ptr_ = NULL;
            cursor->SetInvalid();
            return io_cost;
        }
        // next level
        int block = cur_node->GetSon(pos);

        delete cur_node;

        cur_node = new BNode();
        cur_node->InitFromFile(this, block);
        io_cost++;
    }

    // search in leaf node
    int pos = cur_node->FindPositionByKey(key);

    // get result
    // release space of <cur_node> at Cursor
    cursor->SetValue(cur_node, pos, this);

    root_ptr_ = NULL;
    return io_cost;
}
