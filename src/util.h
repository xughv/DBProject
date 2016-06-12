//
// Created by 徐广晖 on 16/6/5.
//

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

private:
    int id_;                            // object id
    float projection_;                  // projection of the object
};

// -----------------------------------------------------------------------------
//  Cursor: a pointer on the object of B-tree
// -----------------------------------------------------------------------------
class Cursor {
public:
    Cursor() {
        invalid_ = true;
        node_ = NULL;
    }

    ~Cursor() {
        delete node_;
        node_ = NULL;
    }

    void SetValue(BNode* node, int pos, BTree* tree) {
        node_ = node;
        pos_ = pos;
        tree_ = tree;
        invalid_ = false;
    }

    void SetInvalid() {
        invalid_ = true;
    }

    void Release() {
        delete node_;
        node_ = NULL;
    }

    int MoveRight() {
        int io_cost = 0;

        if (invalid()) return io_cost;
        int pos = pos_;

        // get the first key which greater than key
        if (pos + 1 < node_->num_entries()) {
            // in same node
            pos++;
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
            pos = 0;
        }

        // get result
        this->SetValue(node_, pos, tree_);

        return io_cost;
    }

    int MoveLeft() {
        int io_cost = 0;

        if (invalid()) return io_cost;
        int pos = pos_;

        // get the first key which less than key
        if (pos > 0) {
            // in same node
            pos--;
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
            pos = node_->num_entries() - 1;
        }

        // get result
        this->SetValue(node_, pos, tree_);

        return io_cost;
    }

    int id() const {
        if (node_) return node_->GetSon(pos_);
        return -1;
    }

    float projection() const {
        if (node_) return node_->GetKey(pos_);
        return -1;
    }

    bool invalid() const { return invalid_; }

private:
    BNode* node_;
    int pos_;

    bool invalid_;

    BTree* tree_;
};

float Rand();

void GenRandomVector(int dim, float* vec);

bool ReadSetFromFile(char* file_name, int num, int dim, unsigned** data);

float CalcProjection(int dim, unsigned* object, float* line);

float CalcPointsDistance(unsigned* point1, unsigned* point2, int dim);

bool CreateDirectory(const char* path);

void GenTreeFileName(int tree_id, char* path, char* file_name);

int Compare(const void *a , const void *b);

#endif // _UTIL_H_
