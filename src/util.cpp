//
// Created by 徐广晖 on 16/6/5.
//

#include "util.h"
#include <cstdlib>
#include <cmath>

float Rand() {
    float u1 = (float) rand() / (float) RAND_MAX;
    float u2 = (float) rand() / (float) RAND_MAX;

    float x = sqrt(-2.0f * log(u1)) * cos(2.0f * PI * u2);

    return x;
}

