global saxpy_asm

section .text

; Registers
;   edi   = n (number of elements)
;   xmm0  = A (scalar multiplier, already in XMM register)
;   rsi   = X (pointer to input array)
;   rdx   = Y (pointer to input array)
;   rcx   = Z (pointer to output array)
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
