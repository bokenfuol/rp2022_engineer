#ifndef __FILTER_H
#define __FILTER_H

#include "config.h"

void moving_filter(int16_t *input,float *output , int size);
float Lowpass( float X_last, float X_new, float K);

#endif
