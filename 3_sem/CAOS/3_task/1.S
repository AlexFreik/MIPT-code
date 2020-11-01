/*
 * Problem phy03-1: asm-arm/basics/format_io
 * 
 * Реализуйте программу, которая вычисляет сумму двух целых десятичных чисел. Размер типа равен размеру машинного слова.
 * 
 * Можно использовать функции стандартной библиотеки языка Си scanf и printf.
 * 
 * В качестве точки входа можно считать функцию main.
 */

    .data
scanf_fmt:
    .asciz "%d%d"
printf_fmt:
    .asciz "%d\n"

    .text
    .global main
main:
    push {lr}  
  scan:
    ldr r0, scanf_fmt_ptr
    sub sp, sp, #8
    mov r1, sp
    sub sp, sp, #8
    mov r2, sp
    bl scanf
    ldr r0, [sp]
    ldr r1, [sp, #8]
    add sp, sp, #16

  func:
    add r1, r1, r0

  print:
    ldr r0, printf_fmt_ptr
    bl printf
    pop {lr}
    bx lr

scanf_fmt_ptr:
    .word scanf_fmt
printf_fmt_ptr:
    .word printf_fmt

