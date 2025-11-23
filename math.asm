global saxpy_asm

section .text

; Windows:
;   ecx   = n (number of elements)
;   xmm1  = A (scalar multiplier, already in XMM register)
;   r8    = X (pointer to input array)
;   r9    = Y (pointer to input array)
;   [rsp+40] = Z (pointer to output array)
; Linux/macOS:
;   edi   = n (number of elements)
;   xmm0  = A (scalar multiplier, already in XMM register)
;   rsi   = X (pointer to input array)
;   rdx   = Y (pointer to input array)
;   rcx   = Z (pointer to output array)
%ifdef WIN64
saxpy_asm:
    test ecx, ecx
    jle .done_win

    xor eax, eax
    mov r10, [rsp + 40]        ; load Z pointer (5th argument)

.loop_win:
    movss xmm2, [r8 + rax*4]
    mulss xmm2, xmm1
    movss xmm3, [r9 + rax*4]
    addss xmm2, xmm3
    movss [r10 + rax*4], xmm2

    inc eax
    cmp eax, ecx
    jl .loop_win

.done_win:
    ret

%else
saxpy_asm:
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
%endif
