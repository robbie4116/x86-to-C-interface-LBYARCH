#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void saxpy_c(int n, float A, float* X, float* Y, float* Z);
void saxpy_asm(int n, float A, float* X, float* Y, float* Z);

#endif