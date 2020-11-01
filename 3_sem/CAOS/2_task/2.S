/*
 * Problem phy02-2: asm-arm/basics/eqsolution
 * 
 * Реализуйте функцию solve, с прототипом
 * 
 * int solve(int A, int B, int C, int D);
 *       
 * которая находит минимальное значение x ∈[0,254], которое является одним из корней уравнения: Ax3+Bx2+Cx+D=0.
 * 
 * Все аргументы и значение выражения представимы 32-разрядными знаковыми целыми числами.
 * 
 * Не забывайте про сохраняемые регистры; можно использовать для этого память на стеке.
 * 
 * Гарантируется, что на тестовых данных существует целочисленное решение.
 */

/*  int solve(int A, int B, int C, int D);
 *  r0/r1         r0     r1    r2     r3 
 */
    .text
    .global solve

solve:
    push {r4-r7}
    mov r4, #0  // r4 = 0
helper:
    mul r5, r4, r0  // r5 = Ax
    mul r5, r4, r5  // r5 = Ax^2
    mul r5, r4, r5  // r5 = Ax^3

    mul r6, r4, r1  // r6 = Bx
    mul r6, r4, r6  // r6 = Bx^2

    mul r7, r4, r2  // r6 = Cx

    add r5, r6, r5  // r5 = Ax^3 + Bx^2 
    add r5, r7, r5  // r5 = Ax^3 + Bx^2 + Cx
    add r5, r3, r5  // r5 = Ax^3 + Bx^2 + Cx + D
    add r4, r4, #1  // r4 +=1

    cmp r5, #0
    bne helper

    sub r4, r4, #1
    mov r0, r4
    mov r1, r4
    pop {r4-r7}
    bx lr

