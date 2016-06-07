//
// Created by 徐广晖 on 16/6/5.
//

#ifndef _UTIL_H_
#define _UTIL_H_

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


float Rand();

bool ReadSetFromFile(char* file_name, int num, int dim, unsigned** datas);

float CalcProjection(unsigned* object, float* line);

#endif // _UTIL_H_
