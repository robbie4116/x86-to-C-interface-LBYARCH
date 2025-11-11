%ifdef MACHO
    %define FUNC _saxpy_asm
    global _saxpy_asm
%else
    %define FUNC saxpy_asm
    global saxpy_asm
%endif

section .text

%ifndef WIN64
; ============================================================================
; Linux/macOS: System V AMD64 ABI
; void saxpy_asm(int n, float A, float* X, float* Y, float* Z)
; edi=n, xmm0=A, rsi=X, rdx=Y, rcx=Z
; ============================================================================
FUNC:
    test edi, edi
    jle .done
    xor eax, eax
.loop:
    movss xmm1, [rsi + rax*4]
    mulss xmm1, xmm0
    movss xmm2, [rdx + rax*4]
    addss xmm1, xmm2
    movss [rcx + rax*4], xmm1
    inc eax
    cmp eax, edi
    jl .loop
.done:
    ret

%else
; ============================================================================
; Windows: Microsoft x64 calling convention
; void saxpy_asm(int n, float A, float* X, float* Y, float* Z)
; ecx=n, xmm1=A, r8=X, r9=Y, [rsp+40]=Z
; ============================================================================
FUNC:
    test ecx, ecx
    jle .done
    mov r10, [rsp + 40]
    xor eax, eax
.loop:
    movss xmm0, [r8 + rax*4]
    mulss xmm0, xmm1
    movss xmm2, [r9 + rax*4]
    addss xmm0, xmm2
    movss [r10 + rax*4], xmm0
    inc eax
    cmp eax, ecx
    jl .loop
.done:
    ret

%endif