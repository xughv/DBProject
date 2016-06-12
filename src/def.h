#ifndef _DEF_H_
#define _DEF_H_

#include <climits>
#include <cfloat>

// -----------------------------------------------------------------------------
//  Typedefs
// -----------------------------------------------------------------------------
typedef char Block[];

// -----------------------------------------------------------------------------
//  Constants
// -----------------------------------------------------------------------------
const float PI = 3.141592654F;
// file header size
const int BFHEAD_LENGTH = (int) (sizeof(int) * 2);

// default page size
const int PAGE_SIZE_DEFAULT = 1024;

// default number of random lines
const int NUM_RANDOM_LINE_DEFAULT = 50;

// default output dir
const char OUTPUT_DIR_DEFAULT[] = "results/";

// size of type <int>
const size_t SIZE_INT   = sizeof(int);
// size of type <char>
const size_t SIZE_CHAR  = sizeof(char);
// size of type <float>
const size_t SIZE_FLOAT = sizeof(float);


const float FLOAT_MIN = 0 - FLT_MAX;

const float MINFREQ = 0.5f;
#endif // _DEF_H_
