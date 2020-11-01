/*
 * Problem phy03-0: asm-arm/basics/globalvars
 * 
 * Реализуйте функцию calculate, которая вычисляет значение выражения: 
 * R=(A*B)+(C*D), где A, B, C, и D - это глобальные переменные типа uint32_t, 
 * объявленные во внешнем модуле компиляции,
 * а R, - глобальная переменная типа uint32_t в текущем модуле компиляции.
 */

/* void calculate();
 * R=(A*B)+(C*D)
 */
    .data
    .global R
R:
    .space 4

    .text
    .global calculate
calculate:
    push {r4-r5}
    ldr r0, .Lexterns
    ldr r0, [r0]
    ldr r1, .Lexterns + 4
    ldr r1, [r1]
    ldr r2, .Lexterns + 8
    ldr r2, [r2]
    ldr r3, .Lexterns + 12
    ldr r3, [r3]
    mul r4, r0, r1
    mul r5, r2, r3
    add r4, r4, r5
    ldr r5, .Lexterns + 16
    str r4, [r5]
    pop {r4-r5}
    bx lr
.Lexterns:   // because of .L will not be entered in the placemark table and will be visible during objdump
    .word A
    .word B
    .word C
    .word D
    .word R
