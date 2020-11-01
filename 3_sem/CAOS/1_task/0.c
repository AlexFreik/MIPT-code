/*  Problem phy01-0: c/generic/sum-numbers
 *  На стандартном потоке ввода подается последовательно вещественное число x и целое число в 16-ричной записи y.
 *  Единственным аргументов программы является целое число z в 27-ричной системе исчисления.
 *  Необходимо вывести вычислить значение x+y+z и вывести его на стандартный поток вывода с точностью до 3-го знака после точки.
 *  Language:	gcc - Plain C, 64 bit, using -std=c11 or -std=gnu11
 * */
#include <stdio.h> 
#include <stdlib.h>

enum {
    BASE = 27
};

int main(int argc, char* argv[]) {
    double x;
    int y;
    scanf("%lf %x", &x, &y);
    printf("%.3lf\n", x + y + strtol(argv[1], NULL, BASE));
    return 0;
}
