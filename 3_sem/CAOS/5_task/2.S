 /* Problem phy05-2: asm-x86/syscalls/reverse-print-lines
 * 
 * Реализуйте на языке ассемблера x86_64 программу, которая читает со стандартного потока ввода текст, после чего выводит все строки текста в обратном порядке.
 * 
 * Использование стандартной библиотеки Си запрещено.
 * 
 * Точка входа в программу - функция _start.
 * 
 * Examples
 * 
 * Input
 * 
 * Мой дядя самых честных правил,
 * Когда не в шутку занемог,
 * Он уважать себя заставил
 * И лучше выдумать не мог.
 *       
 * Output
 * 
 * И лучше выдумать не мог.
 * Он уважать себя заставил
 * Когда не в шутку занемог,
 * Мой дядя самых честных правил,
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

    // get brk(0)
    mov     rax, __NR_brk
    mov     rdi, 0
    syscall                     // brk(0)
    mov     r12, rax            // r12 = brk(0)

    // increase brk break
    mov     rax, __NR_brk
    lea     rdi, [r12 + 0x4000000]
    syscall                     // brk(brk(0) + 0x500)
    mov     r13, 0x0
    
scan_loop:                           // r12 - brk(0); r13 - number of chars
    mov     rax, __NR_read
    mov     rdi, 0x0
    lea     rsi, [r12 + r13]
    mov     rdx, 0x500
    syscall                     // read(stdin, buf =  *rsp, count = 0x500)
    add     r13, rax            // r13 = number of succesfully readed bytes

    cmp     rax, 0x0
    jg      scan_loop

    push    r14
    mov     r14, r13
print_loop:
    sub     r14, 0x1
    cmp     r14, 0x0
    jl      loop_end

    mov     r8 , [r12 + r14]
    cmp     r8b, '\n'
    jne     print_loop

    mov     rax, __NR_write     // ; the (syscall) system call number of "write".
    mov     rdi, 0x1            // ; first parameter: 1, the stdout file descriptor
    add     r14, 0x1
    lea     rsi, [r12 + r14]
    mov     rdx, r13            // ; bytes to write
    sub     rdx, r14
    sub     r14, 0x1
    syscall                     // ; Issue the system call

    mov     r13, r14
    
    mov     rax, __NR_write     // ; the (syscall) system call number of "write".
    mov     rdi, 0x1            // ; first parameter: 1, the stdout file descriptor
    mov     rsi, new_line_symbol_ptr
    mov     rdx, 0x1            // ; bytes to write
    syscall                     // ; Issue the system call

    jmp print_loop

loop_end:
    mov     rax, __NR_write     // ; the (syscall) system call number of "write".
    mov     rdi, 0x1            // ; first parameter: 1, the stdout file descriptor
    mov     rsi, r12
    mov     rdx, r13            // ; bytes to write
    syscall                     // ; Issue the system call

    pop     r14 
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

