#include "main.h"
#include <xmmintrin.h>

void saxpy_c(int n, float A, float X[], float Y[], float Z[]) {
    for (int i = 0; i < n; i++) {
        __m128 a = _mm_load_ss(&A);
        __m128 x = _mm_load_ss(&X[i]);
        __m128 y = _mm_load_ss(&Y[i]);

        __m128 result = _mm_mul_ss(a, x);
        result = _mm_add_ss(result, y);

        _mm_store_ss(&Z[i], result);
    }
}