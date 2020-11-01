/* Problem phy05-0: asm-x86/syscalls/hello-world
 *
 * Реализуйте на языке ассемблера x86 или x86_64 программу, которая выводит "Hello, World!".
 * 
 * Использование стандартной библиотеки Си запрещено.
 *
 * Точка входа в программу - функция _start.
 */

#include <asm/unistd_64.h>
        
        .intel_syntax noprefix
        .text
        .global _start
_start:
    // Print Hello
    mov     rax, __NR_write             // ; the (syscall) system call number of "write".
    mov     rdi, 1                      // ; first parameter: 1, the stdout file descriptor
    mov     rsi, hello_world_ptr        // ; data to write
    mov     rdx, 14                     // ; bytes to write
    syscall                         // ; Issue the system call
    
    // Do exit
    mov     rax, __NR_exit
    xor     rbx, 0
    syscall
    
    .data
hello_world:
    .string "Hello, World!\n"
hello_world_ptr:
    .long   hello_world
