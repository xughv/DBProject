#include "b_tree.h"

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


class MEDRANK {
public:
    // constructor
    MEDRANK();
    // destructor
    ~MEDRANK();

    // -------------------------------------------------------------------------
    void Init();


private:
    BTree** trees_;
};