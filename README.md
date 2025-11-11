# x86-to-C-interface-LBYARCH

## Setup

### Windows
Run `build.bat`.

### macOS / Linux (POSIX shell)
```bash
nasm -f elf64 math.asm -o math.o
gcc -O2 -msse2 -c math.c -o math_c.o
gcc -O2 -c main.c -o main.o
gcc main.o math_c.o math.o -lm -o saxpy
```


i.) comparative execution time and short analysis of the performance of the kernels

ii.) Take a screenshot of the program output with the correctness check (C).

iii.) Take a screenshot of the program output, including the correctness check (x86-64).

iv.) short videos (5-10mins) showing your source code, compilation, and execution of the C and x86-64 program
