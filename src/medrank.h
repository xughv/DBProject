#ifndef _MED_RANK_H_
#define _MED_RANK_H_

#include "b_tree.h"
#include <cstring>

class MEDRANK {
public:
    // ------------------------------------------------------------------------
    // Singleton
    static MEDRANK* GetInstance() {
        static MEDRANK instance;
        return &instance;
    }

    // destructor
    ~MEDRANK();
    // ------------------------------------------------------------------------
    // Init the MEDRANK
    void Init(char *output_folder);

    // Init <h_> and <l_> and reset value
    void InitCursor();

    // Initial votes
    void InitVote(int num);

    // ------------------------------------------------------------------------
    // Execute
    // For every random line. Get one point nearest the query (on the projection),
    // it maybe the h cursor or the l cursor.
    // Calculate the distance of them between query.
    // And then the nearest candidate get one vote.
    // If the h cursor is more near, then the candidate in h get one vote, and the h cursor move left.
    // If the l cursor is more near, then the candidate in l get one vote, and the h cursor move right.
    // Do this until found a winner.
    int Execute();


    // ------------------------------------------------------------------------
    // Generate the random line
    // <dim_line>:     the dimension of the line
    // <num_line>:     the number of the line
    void GenRandomVectors(int dim, int num_line);

    // Get the random vector
    float* GetRandomVector(int index);

    //-------------------------------------------------------------------------
    // Vote and judge
    // If the candidate has more than the half of the number of line, then the
    // candidate win.
    // If there a candidate win, return the id of the candidate
    // else return -1.
    int VoteAndJudge(int candidate);


    // ------------------------------------------------------------------------
    // Get number of vector.
    int num_line();

    // Set the value of query
    void set_q(int index, float value);

    // Get the cost of IO operation
    int io_cost();

private:
    MEDRANK();                      // constructor

    Cursor** h_;                    // mark the cursor that no greater than the the projection of query
    Cursor** l_;                    // mark the cursor that greater than the the projection of query

    float* q_;                      // the projection of query
    BTree** trees_;                 // the B+tree of this application.

    int num_vector_;                // the number of vector

    float** vectors_;               // the random line

    int* votes_;                    // the votes of all the candidate
    int num_data_;                  // the number of data set

    int io_cost_;                   // the times of IO operation.
};

#endif // _MEDRANK_H_