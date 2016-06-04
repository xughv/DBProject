
// -----------------------------------------------------------------------------
//  BTree: b-tree to index
// -----------------------------------------------------------------------------

BTree::Tree() {
    root_ = -1;
    file_ = NULL;
    root_ptr_ = NULL;
}

// destructor                      
BTree::~BTree() {
    if (root_ptr_ != NULL) {
        delete root_ptr_;
        root_ptr_ = NULL;
    }

    if (file_ != NULL) {
        char* header = new char[file_->]
    }
}

// -------------------------------------------------------------------------
// init a new b-tree
void BTree::Init(char* file_name, int block_length) {

}

// load an exist b-tree
void BTree::InitRestore(char* file_name) {

}

// -------------------------------------------------------------------------
// bulkload b-tree from hash table in mem
// <num> -- number of entries
int BTree::BulkLoad(Pair* pairs, int num) {

}


// -------------------------------------------------------------------------
// read <root> from buffer
int BTree::ReadHeader(char* buf) {

}

// write <root> into buffer
int BTree::WriteHeader(char* buf) {

}

// -------------------------------------------------------------------------
// load root of b-tree
void BTree::LoadRoot() {

}

// delete root of b-tree
void BTree::DeleteRoot() {

}
