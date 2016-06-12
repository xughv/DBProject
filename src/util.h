#ifndef _UTIL_H_
#define _UTIL_H_

#include <cstdio>
#include "b_node.h"

// -----------------------------------------------------------------------------
//  Pair: structure of a set of pairs
// -----------------------------------------------------------------------------
class Pair {
public:
    int id() const { return id_; }
    float projection() const { return projection_; }

    void SetValue(int id, float projection) {
        id_ = id;
        projection_ = projection;
    }

    // to use the qsort
    static int Compare(const void *a , const void *b) {
        return ((Pair *)a)->projection() > ((Pair *)b)->projection() ? 1 : -1;
    }

private:
    int id_;                            // object id
    float projection_;                  // projection of the object
};

// -----------------------------------------------------------------------------
//  Cursor: a pointer on the object of B-tree
// -----------------------------------------------------------------------------
class Cursor {
public:
    // constructor
    Cursor() {
        invalid_ = true;
        node_ = NULL;
    }

    // destructor
    ~Cursor() {
        delete node_;
        node_ = NULL;
    }

    // set the value of cursor
    void SetValue(BNode* node, int pos, BTree* tree) {
        node_ = node;
        pos_ = pos;
        tree_ = tree;
        invalid_ = false;
    }

    // set the cursor invalid
    void SetInvalid() {
        Release();
        invalid_ = true;
    }

    // release the node of cursor
    void Release() {
        delete node_;
        node_ = NULL;
    }

    // reload the equal operation of Cursor
    Cursor& operator =(const Cursor& a) {
        this->pos_ = a.pos();
        this->tree_ = a.tree();
        this->invalid_ = false;
        this->node_ = new BNode();
        this->node_->InitFromFile(tree_, a.node()->block());
        return *this;
    }

    // ------------------------------------------------------------------------
    // MoveRight
    // Set the cursor right.
    // If there is no block on the right of cursor, set the cursor invalid
    // return: IO Cost
    // ------------------------------------------------------------------------
    int MoveRight() {
        int io_cost = 0;

        if (invalid()) return io_cost;

        // get the first key which greater than key
        if (pos_ + 1 < node_->num_entries()) {
            // in same node
            pos_++;
        } else {
            // in right_sibling node
            int block = node_->right_sibling();
            if (block < 0) {
                // current node don't have right_sibling
                this->SetInvalid();
                return io_cost;
            }
            delete node_;
            node_ = new BNode();
            node_->InitFromFile(tree_, block);
            io_cost++;

            if (node_->num_entries() < 1) {
                this->SetInvalid();
                return io_cost;
            }
            // pos at first in right_sibling
            pos_ = 0;
        }

        return io_cost;
    }

    // ------------------------------------------------------------------------
    // MoveLeft
    // Set the cursor left.
    // If there is no block on the left of cursor, set the cursor invalid
    // return: IO Cost
    // ------------------------------------------------------------------------
    int MoveLeft() {
        int io_cost = 0;

        if (invalid()) return io_cost;

        // get the first key which less than key
        if (pos_ > 0) {
            // in same node
            pos_--;
        } else {
            // in left_sibling node
            int block = node_->left_sibling();
            if (block < 0) {
                // current node don't have left_sibling
                this->SetInvalid();
                return io_cost;
            }
            delete node_;
            node_ = new BNode();
            node_->InitFromFile(tree_, block);
            io_cost++;

            if (node_->num_entries() < 1) {
                this->SetInvalid();
                return io_cost;
            }
            // pos at last in left_sibling
            pos_ = node_->num_entries() - 1;
        }

        return io_cost;
    }

    // get the node of cursor
    BNode* node() const { return node_; }

    // get the position of cursor
    int pos() const { return pos_; }

    // get the btree of this cursor in.
    BTree* tree() const { return tree_; }

    // get id
    int id() const {
        if (node_) return node_->GetSon(pos_);
        return -1;
    }

    // get projection
    float projection() const {
        if (node_) return node_->GetKey(pos_);
        return -1;
    }

    // get invalid
    bool invalid() const { return invalid_; }

private:
    BTree* tree_;                  // the b-tree this cursor in
    BNode* node_;                  // the node this cursor point to
    int pos_;                      // the position in the node

    bool invalid_;                 // if the cursor out of the block, it is invalid
};


// Use Box-Muller method to generate a random variable from N(0,1).
float Rand();

// Get a vector by Box-Muller and then normalize it.
void GenRandomVector(int dim, float* vec);

// ----------------------------------------------------------------------------
// ReadSetFromFile
// read the data from a file <file_name> in binary.
// This is the format of data set:
// [offset] [type]          [value]          [description]
// 0000     32 bit integer  0x00000803(2051) magic number
// 0004     32 bit integer  60000            number of images
// 0008     32 bit integer  28               number of rows
// 0012     32 bit integer  28               number of columns
// 0016     unsigned byte   ??               pixel
// 0017     unsigned byte   ??               pixel
// ........
// xxxx     unsigned byte   ??               pixel
//
// Thus we skip the 16 bytes of the file and just read the data after the 16 offset.
// ----------------------------------------------------------------------------
// <file_name>:     the path of the file
// <num>:           the number of the data
// <dim>:           the dimension of the data
// <data>:          get the data
bool ReadSetFromFile(char* file_name, int num, int dim, unsigned char** data);

// Use multiplication to calculate the projection of a vector on a line
float CalcProjection(int dim, unsigned char* object, float* line);

// Use the Euclidean distance to calculate the distance of two point
float CalcPointsDistance(unsigned char* point1, unsigned char* point2, int dim);

// -------------------------------------------------------------------------
// CreateDirectory
// In the Linux and OS X system can use the API in CreateDirectory function.
// But not in Windows system, so, if you want to run the application in
// windows, you can create the folder "results/index/" by yourself and comment
// the function CreateDirectory.
// -------------------------------------------------------------------------
bool CreateDirectory(const char* path);

// Generate the index file name
void GenTreeFileName(int tree_id, char* path, char* file_name);

#endif // _UTIL_H_
