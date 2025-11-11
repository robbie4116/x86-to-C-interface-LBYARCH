@echo off
del *.o *.exe 2>nul
nasm -f win64 -dWIN64 math.asm -o math.o
gcc -O2 -msse -o saxpy.exe main.c math.c math.o -lm
if errorlevel 1 (
    echo Build failed!
    pause
    exit /b 1
)
echo Build successful!
saxpy.exe
pause