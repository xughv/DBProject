
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
        char* header = new char[file_->block_length()];
        WriteHeader(header);
        file_->SetHeader(header);

        delete[] header;
        header = NULL;

        delete file_;
        file_ = NULL;
    }
}

// -------------------------------------------------------------------------
// init a new b-tree
void BTree::Init(char* file_name, int block_length) {

    FILE* file = fopen(file_name, "r");
    if (file != NULL) {
        fclose(file);
        // [TODO] file is exist
    }

    file_ = new BlockFile(file_name, block_length);
    root_ptr_ = new BNode();
    root_ptr_->Init(0, this);

    root_ = root_ptr_->block();

    DeleteRoot();
}

// load an exist b-tree
void BTree::InitRestore(char* file_name) {

    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        // [TODO] file is not exist
    }
    fclose(file);
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
