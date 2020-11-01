/* Problem phy04-0: asm-x86/arrays
 * Реализуйте функцию на языке ассемблера x86 (IA-32) или x86-64 с сигнатурой:
 * extern void summ(int N, const int *A, const int *B, int *R);      
 * Функция вычисляет значения Ri=Ai+Bi для всех i от 0 до N.
 * Память для хранения массива-результата уже выделена.
 */
    .global summ
    .intel_syntax noprefix

    .text
summ:
    mov ecx, [esp + 4]

.Lloop:
    dec ecx                   //; ecx = N - 1
    mov esi, [esp + 8]        //; esi = *A
    mov edi, [esi + 4 * ecx]  //; edi = A[N-1] 

    mov esi, [esp + 12]       //; esi = *B
    add edi, [esi + 4 * ecx]  //; edi = A[N-1] + B[N-1] 
    
    mov esi, [esp + 16]       //; esi = *R
    mov [esi + 4 * ecx], edi  //; R[N-1] = edi = A[N-1] + B[N-1]
    
    inc ecx
    loop .Lloop
    
    ret
