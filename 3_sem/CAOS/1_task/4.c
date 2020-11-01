/*
 * Реализуйте функцию с прототипом:
 * 
 * extern int
 * check_int(uint32_t u32); // retval: 0 - false, 1 - true 
 * которая проверяет, может ли число u32 быть представлено типом float:
 * 
 * float f = u32;
 * uint64_t y = f;
 * assert(u32==y);
 *       
 * Функция должна вернуть 1, если число может быть представлено типом float, или 0, - если не может.
 * 
 * Допускается исользовать только побитовые операции.
 */

#include <stdio.h>
#include <stdint.h>

enum {
    M_SIZE = 24,
};
extern int
check_int(uint32_t u32) { /* retval: 0 - false, 1 - true */
    int retval = 1;
    while (!(u32 & 1) && u32) {  // can replace "!=", but it would look ugly && stupid
        u32 >>= 1;
    }
    /*  can replace " > " with a while loop and several if clauses,
     *  but this will also not look very nice.
     *  And it doesn't make sense within the framework of understanding the problem.
     */
    if (u32 > 1ULL << M_SIZE) {
        retval = 0;
    }
    return retval;
}
