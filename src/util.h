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

    int operator -(Pair &pair) const {
        return this->projection() - pair.projection();
    }

private:
    int id_;                            // object id
    float projection_;                  // projection of the object
};


class Cursor {
public:
    Cursor() {
        invalid_ = true;
    }
    void SetValue(int node_block, int index, int id, float projection, BTree* tree) {
        node_block_ = node_block;
        index_ = index;
        pair.SetValue(id, projection);
        tree_ = tree;
        invalid_ = false;
    }

    void SetInvalid() {
        invalid_ = true;
    }

    int node_block() const { return node_block_; }
    int index() const { return index_; }
    int id() const { return pair.id(); }
    float projection() const { return pair.projection(); }
    bool invalid() const { return invalid_; }

    // 前缀自加重载
    Cursor& operator ++() {
        if (invalid()) return *this;
        int pos = index_;
        BNode* cur_node = new BNode();

        cur_node->InitFromFile(tree_, index_);

        // get the first key which greater than key
        if (pos + 1 < cur_node->num_entries()) {
            // in same node
            pos++;
        } else {
            // in right_sibling node
            int block = cur_node->right_sibling();
            if (block < 0) {
                // current node don't have right_sibling
                this->SetInvalid();
                return *this;
            }
            delete cur_node;
            cur_node = new BNode();
            cur_node->InitFromFile(tree_, block);
            // pos at first in right_sibling
            pos = 0;
        }

        // get result
        this->SetValue(cur_node->block(), pos,
                        cur_node->GetSon(pos), cur_node->GetKey(pos), tree_);

        //  Release space
        delete cur_node;

        return *this;
    }

    // 前缀自减重载
    Cursor& operator --() {
        if (invalid()) return *this;
        int pos = index_;
        BNode* cur_node = new BNode();

        cur_node->InitFromFile(tree_, index_);

        // get the first key which less than key
        if (pos > 0) {
            // in same node
            pos--;
        } else {
            // in left_sibling node
            int block = cur_node->left_sibling();
            if (block < 0) {
                // current node don't have left_sibling
                this->SetInvalid();
                return *this;
            }
            delete cur_node;
            cur_node = new BNode();
            cur_node->InitFromFile(tree_, block);

            // pos at last in left_sibling
            pos = cur_node->num_entries() - 1;
        }

        // get result
        this->SetValue(cur_node->block(), pos,
                       cur_node->GetSon(pos), cur_node->GetKey(pos), tree_);

        //  Release space
        delete cur_node;

        return *this;
    }
private:
    int node_block_;
    int index_;
    Pair pair;

    bool invalid_;

    BTree* tree_;
};

float Rand();

void GenRandomVector(int dim, float* vec);

bool ReadSetFromFile(char* file_name, int num, int dim, unsigned** datas);

float CalcProjection(int dim, unsigned* object, float* line);

float CalcPointsDistance(unsigned* point1, unsigned* point2, int dim);

bool CreateDirectory(const char* path);

void GenTreeFileName(int tree_id, char* path, char* file_name);

int Compare(const void *a , const void *b);

#endif // _UTIL_H_
