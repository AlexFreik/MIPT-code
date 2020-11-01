/* Problem phy04-3: asm-x86/fpu-sse/vector-dot-product
 * 
 * Реализуйте на языке ассемблера x86 (IA-32) или x86-64 функцию с сигнатурой:
 * 
 * extern float dot_product(int N, const float *A, const float *B);
 * которая вычисляет скалярное произведение векторов A и B.
 * 
 * Гарантируется, что N больше нуля, но не гарантируется, что N кратно четырём.
 * 
 * Значения исходных векторов хранятся в произвольных адресах памяти, поэтому выравнивание для них не гарантируется.
 * 
 * Необходимо минимизировать количество выполняемых вычислительных инструкций, используя векторные вычисления. Доступен набор команд SSE 4.2. Для архитектуры x86-64 также доступен набор команд AVX.
 */

    .global dot_product
    .intel_syntax noprefix

    .text
// extern float dot_product(int N, const float *A, const float *B);
//        xmm0                  rdi             rsi             rdx
dot_product:
    // standart prologue
    push    ebp  
    mov     ebp, esp

    add     esp, 8                  // esp point on the first arg

    // put args in registers
    mov     edi, [esp + 0]          // edi = N
    mov     esi, [esp + 4]          // esi = *A
    mov     edx, [esp + 8]          // edx = *B
    
    mov     ecx, 0                  // ecx = 0
    cvtsi2sd xmm0, ecx               // xmm0 = 0

.Ladd_loop:
    sub     edi, 4
    cmp     edi, 0
    jl      end                     // skip loop if N < 0

    movups  xmm1, [esi + ecx * 4]   // xmm1 = A[ecx, ..., ecx + 3] 
    movups  xmm2, [edx + ecx * 4]   // xmm2 = B[ecx, ..., ecx + 3]

    dpps    xmm1, xmm2, 0xF1        // xmm1 = dot_prod()
    addss   xmm0, xmm1              // xmm0 += dot_prod
     
    add ecx, 4
    jmp     .Ladd_loop

end:
    cmp     edi, -4
    je      loop_end                // skip loop if N < 0
 .Ladd_loop2:
    movups  xmm1, [esi + ecx * 4]   // xmm1 = A[ecx, ..., ecx + 3] 
    movups  xmm2, [edx + ecx * 4]   // xmm2 = B[ecx, ..., ecx + 3]
    mulss   xmm1, xmm2
    addss   xmm0, xmm1              // xmm0 += dot_prod

    dec     edi
    inc     ecx
    cmp     edi, -4
    jne     .Ladd_loop2

loop_end:
    leave
    sub     esp, 4          // выделяем 8 байт на стеке
    movss   [esp], xmm0     // копируем из xmm0 в память
    fld     dword ptr [esp] // загружаем из памяти в стек x87
    add     esp, 4          // освобождаем память на стеке

    ret

