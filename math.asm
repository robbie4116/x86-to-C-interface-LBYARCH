
%ifdef MACHO
    %define FUNC _saxpy_asm  ; macOS requires underscore prefix
    global _saxpy_asm
%else
    %define FUNC saxpy_asm   ; Linux/Windows use no prefix
    global saxpy_asm
%endif

section .text

%ifndef WIN64
; if not Windows, we use the System V AMD64 calling convention for mac
;   edi   = n (number of elements)
;   xmm0  = A (scalar multiplier, already in XMM register)
;   rsi   = X (pointer to input array)
;   rdx   = Y (pointer to input array)
;   rcx   = Z (pointer to output array)
FUNC:
    ; Check if n <= 0, if it is we have to skip to done
    test edi, edi
    jle .done
    
    ; Initializing the loop counter
    xor eax, eax
    
.loop:
    ; Load X[i] into xmm1  
    movss xmm1, [rsi + rax*4]  ; rax*4 because floats are 4 bytes
    
    ; Multiply A * X[i], store result in xmm1   
    mulss xmm1, xmm0
    
    ; Load Y[i] into xmm2  
    movss xmm2, [rdx + rax*4]
    
    ; Add Y[i] to get final result: A*X[i] + Y[i] 
    addss xmm1, xmm2
    
    ; Store result to Z[i]  
    movss [rcx + rax*4], xmm1
    
    ; incremening loop counter
    inc eax
    
    ; compare so we can check if we've processed all elements (eax < n)
    cmp eax, edi
    jl .loop  ; Jump back to loop if eax < edi
    
.done:
    ret  ; Return to caller

%else
; if Windows, we use the Microsoft x64 calling convention
;   ecx   = n (number of elements)
;   xmm1  = A (scalar multiplier, already in XMM register)
;   r8    = X (pointer to input array)
;   r9    = Y (pointer to input array)
;   [rsp+40] = Z (pointer to output array, passed on stack)
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