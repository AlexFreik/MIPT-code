/* Problem phy03-2: asm-arm/basics/char-filter-digits
 * 
 * Напишите программу, которая читает со стандартного потока ввода последовательность символов, 
 * и выводит только те из них, которые являются десятичными цифрами от 0 до 9.
 * Можно использовать функции стандартной библиотеки языка Си: fgetc и fputc.
 * В качестве точки входа можно считать функцию main.
 */
     .text
    .global main
main:
    push {lr}
   
loop:
    bl getchar  // function is equivalent to getc(stdin)
    cmp r0, #0
    blt end_of_loop

    cmp r0, #'0'
    blt not_digit
    cmp r0, #'9'
    bgt not_digit

    bl putchar

not_digit:
    b loop
end_of_loop:
    mov r0, #'\n'
    bl putchar
    pop {lr}
    bx lr
