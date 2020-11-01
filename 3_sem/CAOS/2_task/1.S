/*
 * Problem phy02-1: asm-arm/basics/memaccess
 * 
 * Реализуйте на языке ассемблера ARM функцию с сигнатурой:
 * 
 * int summ(int x0, int N, int *X)
 *  r0/r1        r0      r1     r2
 * Функция должна вычислять значение x0+∑xi, где 0≤i<N
 */
    .text
    .global summ

summ:
    ldr r3, [r2]  // load value with r2 adress into r3 (r3 = X[0])
    add r0, r0, r3 // x += X[0]
    add r2, r2, #4  // X += 1
    sub r1, r1, #1  // N -=1
    
    cmp r1, #0  // compare N with 0
    bgt summ  // recursively call summ if condition gt (greater than) is true
    
    mov r1, r0  // r1 = summ
    bx lr  // go back from the funktion


