#include "main.h"

#ifdef _WIN32
#include <windows.h>
#include <malloc.h>
#define aligned_alloc_compat(align, size) _aligned_malloc(size, align)
#define aligned_free_compat(ptr) _aligned_free(ptr)
#else
#define aligned_alloc_compat(align, size) aligned_alloc(align, size)
#define aligned_free_compat(ptr) free(ptr)
#endif

double get_time() {
#ifdef _WIN32
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / freq.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
#endif
}

void print_first_10(const char* label, float* Z, int n) {
    printf("%s:\n", label);
    for (int i = 0; i < 10 && i < n; i++) {
        printf("  Z[%d] = %.2f\n", i, Z[i]);
    }
    printf("\n");
}

int verify(float* Z_c, float* Z_asm, int n) {
    for (int i = 0; i < n; i++) {
        if (fabsf(Z_c[i] - Z_asm[i]) > 1e-5f) {
            printf("ERROR at index %d: C=%.2f, ASM=%.2f\n", i, Z_c[i], Z_asm[i]);
            return 0;
        }
    }
    return 1;
}

int main() {
    int sizes[] = {1 << 20, 1 << 24, 1 << 28};
    const int RUNS = 30;
    
    printf("SAXPY Performance Test\n");
    printf("======================\n\n");
    
    srand((unsigned)time(NULL));
    
    for (int s = 0; s < 3; s++) {
        int n = sizes[s];
        printf("n = 2^%d (%d elements)\n", 20 + s*4, n);
        printf("------------------------\n");
        
        float *X = aligned_alloc_compat(16, n * sizeof(float));
        float *Y = aligned_alloc_compat(16, n * sizeof(float));
        float *Z_c = aligned_alloc_compat(16, n * sizeof(float));
        float *Z_asm = aligned_alloc_compat(16, n * sizeof(float));
        
        if (!X || !Y || !Z_c || !Z_asm) {
            printf("Memory allocation failed!\n");
            continue;
        }
        
        float A = 2.0f;
        for (int i = 0; i < n; i++) {
            X[i] = (float)rand() / RAND_MAX * 10.0f;
            Y[i] = (float)rand() / RAND_MAX * 10.0f;
        }
        
        printf("A = %.2f\n\n", A);
        
        // Warmup
        saxpy_c(n, A, X, Y, Z_c);
        saxpy_asm(n, A, X, Y, Z_asm);
        
        // Time C version
        double start = get_time();
        for (int r = 0; r < RUNS; r++) {
            saxpy_c(n, A, X, Y, Z_c);
        }
        double time_c = (get_time() - start) / RUNS;
        
        // Time ASM version
        start = get_time();
        for (int r = 0; r < RUNS; r++) {
            saxpy_asm(n, A, X, Y, Z_asm);
        }
        double time_asm = (get_time() - start) / RUNS;
        
        print_first_10("C version", Z_c, n);
        print_first_10("ASM version", Z_asm, n);
        
        printf("Verification: %s\n\n", verify(Z_c, Z_asm, n) ? "PASSED" : "FAILED");
        printf("C version:   %.6f seconds\n", time_c);
        printf("ASM version: %.6f seconds\n", time_asm);
        printf("Speedup:     %.2fx\n\n", time_c / time_asm);
        
        aligned_free_compat(X);
        aligned_free_compat(Y);
        aligned_free_compat(Z_c);
        aligned_free_compat(Z_asm);
    }
    
    return 0;
}