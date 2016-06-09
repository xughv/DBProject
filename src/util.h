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

    bool operator <(Pair &pair) const {
        return this->projection() < pair.projection();
    }

private:
    int id_;                            // object id
    float projection_;                  // projection of the object
};


class Cursor {
public:
    void SetValue(BNode* node, int index, int id, float projection) {
        node_ = node;
        index_ = index;
        pair.SetValue(id, projection);
    }

    BNode* node() const { return node_; }
    int index() const { return index_; }
    int id() const { return pair.id(); }
    float projection() const { return pair.projection(); }

    // 前缀自加重载
    Cursor& operator ++() {

        return *this;
    }

    // 前缀自减重载
    Cursor& operator --() {

        return *this;
    }
private:
    BNode* node_;
    int index_;
    Pair pair;
};

float Rand();

void GenRandomVector(int dim, float* vec);

bool ReadSetFromFile(char* file_name, int num, int dim, unsigned** datas);

float CalcProjection(int dim, unsigned* object, float* line);

bool CreateDirectory(const char* path);

void GenTreeFileName(int tree_id, char* path, char* file_name);

template<class T>
void Sort(T* begin, T* end);

#endif // _UTIL_H_
