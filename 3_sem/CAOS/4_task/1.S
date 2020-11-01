/* Problem phy04-1: asm-x86/fpu-sse/trig-macloren-series
 * 
 * Реализуйте на языке ассемблера x86 (IA-32) или x86-64 функцию с сигнатурой:
 * 
 * extern double my_sin(double x)
 * которая вычисляет значение sin(x).
 * 
 * Запрещено использовать встроенные тригонометрические инструкции.
 * 
 * Для вычислений используйте известный вам из курса Математического анализа способ разложения функции в ряд. 
 * Точность результата должна быть маскимально возможной для типа данных double.
 */

    .global my_sin 
    .intel_syntax noprefix

    .text
my_sin:
    movsd xmm0, [esp + 4]       // xmm0 = x

    mov edi, -1                 
    cvtsi2sd xmm1, edi          // xmm1 = -1
    movsd xmm2, [esp + 4]       // xmm2 = x
    movsd xmm3, [esp + 4]       
    mulsd xmm3, xmm2            // xmm3 = x^2
    mov edi, 1
    cvtsi2sd xmm4, edi          // xmm4 = 1
    cvtsi2sd xmm5, edi          // xmm5 = 1
    
    
.Ladd_loop:
    mulsd xmm2, xmm1            // *= -1
    mulsd xmm2, xmm3            // *= x^2

    addsd xmm5, xmm4 
    divsd xmm2, xmm5    
    addsd xmm5, xmm4 
    divsd xmm2, xmm5            // xmm2 /= n*(n+1) 

    movsd xmm7, xmm0
    addsd xmm0, xmm2
    
    comisd xmm7, xmm0
    jne .Ladd_loop

    sub     esp, 8          // выделяем 8 байт на стеке
    movsd   [esp], xmm0     // копируем из xmm0 в память
    fld     qword ptr [esp] // загружаем из памяти в стек x87
    add     esp, 8          // освобождаем память на стеке
    ret

