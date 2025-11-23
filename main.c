#include "main.h"

double get_time()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

// print first 10 values from each array
void print_first_10(const char label[], float Z[], int n)
{
    printf("%s:\n", label);
    for (int i = 0; i < 10 && i < n; i++)
    {
        printf("  Z[%d] = %.6f\n", i, Z[i]);
    }
    printf("\n");
}

// compare each value, if there is a difference greater than 0.00001 it will return 0
int verify(float Z_c[], float Z_asm[], int n)
{
    for (int i = 0; i < n; i++)
    {
        if (fabsf(Z_c[i] - Z_asm[i]) > 1e-5f)
        {
            printf("ERROR at index %d: C=%.6f, ASM=%.6f\n", i, Z_c[i], Z_asm[i]);
            return 0;
        }
    }
    return 1;
}

int main()
{
    int sizes[] = {1 << 20, 1 << 24, 1 << 28}; // 2^20, 2^24, 2^28
    const int RUNS = 30;

    srand((unsigned)time(NULL));

    printf("============== SAXPY PERFORMANCE REPORT ==============\n");

    for (int s = 0; s < 3; s++)
    {
        int n = sizes[s];
        printf("------------------------------------------------------\n");
        printf("|                    Test Case %d                     |\n", s + 1);
        printf("------------------------------------------------------\n\n");

        float *X = aligned_alloc(16, n * sizeof(float));
        float *Y = aligned_alloc(16, n * sizeof(float));
        float *Z_c = aligned_alloc(16, n * sizeof(float));
        float *Z_asm = aligned_alloc(16, n * sizeof(float));

        if (!X || !Y || !Z_c || !Z_asm)
        {
            printf("Memory allocation failed!\n");
            continue;
        }

        float A = ((float)rand() + 1.0f) / ((float)RAND_MAX + 1.0f) * 10.0f;

        for (int i = 0; i < n; i++)
        {
            X[i] = (float)rand() / RAND_MAX * 100.0f;
            Y[i] = (float)rand() / RAND_MAX * 100.0f;
        }

        printf("--------------- Input Parameters ---------------\n");
        printf("Size of Array (n) = 2^%d (%d elements)\n", 20 + s * 4, n);
        printf("Scalar Value  A = %.6f\n", A);
        print_first_values("First 10 Values of X", "X", X, n);
        print_first_values("First 10 Values of Y", "Y", Y, n);
        printf("[?] Values of X and Y Arrays are randomized\n");

        saxpy_c(n, A, X, Y, Z_c);
        saxpy_asm(n, A, X, Y, Z_asm);

        printf("------------ C Computation Results -------------\n");
        for (int i = 0; i < 10 && i < n; i++)
        {
            printf("Z[%d] = %.6f\n", i, Z_c[i]);
        }

        printf("----------- ASM Computation Results ------------\n");
        for (int i = 0; i < 10 && i < n; i++)
        {
            printf("Z[%d] = %.6f\n", i, Z_asm[i]);
        }

        printf("----------- Cross-Check Verification -----------\n");
        printf("Status: %s\n", verify(Z_c, Z_asm, n) ? "PASSED" : "FAILED");
        printf("-------------------------------------------------\n");
        printf("Implementation            Time (s)        Speedup\n");
        printf("-------------------------------------------------\n");

        // get average time over all the runs
        double start = get_time();
        for (int r = 0; r < RUNS; r++)
        {
            saxpy_c(n, A, X, Y, Z_c);
        }
        double time_c = (get_time() - start) / RUNS;

        start = get_time();
        for (int r = 0; r < RUNS; r++)
        {
            saxpy_asm(n, A, X, Y, Z_asm);
        }
        double time_asm = (get_time() - start) / RUNS;

        if (time_c < time_asm)
        {
            printf("C Version                 %.6f        %.2fx\n", time_c, time_asm / time_c);
            printf("ASM Version               %.6f            -\n\n", time_asm);
        }
        else
        {
            printf("C Version                 %.6f            -\n", time_c);
            printf("ASM Version               %.6f        %.2fx\n\n", time_asm, time_c / time_asm);
        }

        free(X);
        free(Y);
        free(Z_c);
        free(Z_asm);
    }

    return 0;
}
