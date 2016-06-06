//
// Created by 徐广晖 on 16/6/5.
//

#ifndef _UTIL_H_
#define _UTIL_H_

float Rand();

bool ReadSetFromFile(char* file_name, int num, int dim, unsigned** datas);

float CalcProjection(unsigned* object, float* line);

#endif // _UTIL_H_
