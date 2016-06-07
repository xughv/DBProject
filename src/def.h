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

// file header size
const int BFHEAD_LENGTH = (int) (sizeof(int) * 2);

// default page size
const int PAGE_SIZE_DEFAULT = 1024;

// size of type <int>
const size_t SIZE_INT   = sizeof(int);
// size of type <char>
const size_t SIZE_CHAR  = sizeof(char);
// size of type <float>
const size_t SIZE_FLOAT = sizeof(float);


const float FLOAT_MIN = 0 - FLT_MAX;

#endif // _DEF_H_
