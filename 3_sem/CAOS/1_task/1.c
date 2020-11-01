/* Problem phy01-1: c/ints/satsum
 *
 * Реализовать функцию с прототипом:
 * uint16_t satsum(uint16_t x, uint16_t y);
 * которая выполняет сложение с насыщением, то есть, в случае целочисленного 
 * переполнения возвращает максимально возможное целое число для данного типа данных.
 *
 * В коде программы запрещено использовать константы, отличные от 0 и 1.
 * Запрещено использовать платформо-зависимые или компиляторо-зависимые функции, 
 * типы данных с повышенной разрядностью или вещественные типы даннх.
 * */

#include <stdint.h>

uint16_t satsum(uint16_t x, uint16_t y) {
    uint16_t sum = x + y, zero = 0;
    if (sum < x) {
          sum = zero - 1;
    }
    return sum;
}
