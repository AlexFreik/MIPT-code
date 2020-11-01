/* Problem phy05-1: asm-x86/syscalls/copy-stream
 * 
 * Реализуйте на языке ассемблера x86_64 программу, которая копирует содержимое со стандартного потока ввода на стандартный поток вывода.
 * 
 * Использование стандартной библиотеки Си запрещено.
 * 
 * Точка входа в программу - функция _start.
 * 
*/

#include <asm/unistd_64.h>
        
        .intel_syntax noprefix
        .text
        .global _start
_start:
    mov     rbp, rsp
    
    call my_main
    
    // Do exit
    mov     rax, __NR_exit
    xor     rbx, rbx
    syscall

my_main:
    push    rbx
    push    rbp
    push    r12 
    push    r13

    mov     r12, rsp
    sub     rsp, 0x8 
    
loop:
    mov     rax, __NR_read
    mov     rdi, 0x0
    mov     rsi, r12
    mov     rdx, 0x1
    syscall                     // read(stdin, buf =  *rsp, count = 0x500)
    mov     r13, rax            // r13 = number of succesfully readed bytes

    cmp     rax, 0x0
    jle     loop_end            // stop reading if read return 0 or -1

    mov     rax, __NR_write     // ; the (syscall) system call number of "write".
    mov     rdi, 0x1            // ; first parameter: 1, the stdout file descriptor
    mov     rsi, r12            // ; data to write
    mov     rdx, r13            // ; bytes to write
    syscall                     // ; Issue the system call
    
    jmp loop

loop_end:

    add     rsp, 0x8
    pop     r13
    pop     r12
    pop     rbp
    pop     rbx
    ret

    .data
new_line_symbol:
    .string "\n"
new_line_symbol_ptr:
    .long   new_line_symbol

