/* Реализуйте калькулятор выражений над множествами в обратной польской записи.
 * 
 * На стандартном потоке ввода задается последовательность, состоящая из символов-значений, и символов-операций.
 * 
 * Значениями являются цифры от 0 до 9, и буквы английского алфавита (заглавные и строчные). 
 * Последовательности значений кодируют множества из 62 элементов.
 * 
 * Операциями являются:
 * 
 * & - пересечение множеств;
 * | - объединение множеств;
 * ^ - симметрическая разность;
 * ~ - дополнение текущего результата до полного множества.
 * В начальный момент времени, множество-результат является пустым. 
 * При чтении символов со стандартного потока ввода, когда встречается символ операции, 
 * то эта операция применяется к множеству-результату и последнему прочитанному множеству (как в обратной польской записи).
 * 
 * Вывести на экран полученное итоговое значение множества в нормализованной форме: 
 * сначала цифры, затем заглавные буквы, потом - строчные (упорядоченные по алфавиту).
 * 
 * Для хранения множеств использовать, суммарно, не более 16 байт памяти.
 * 
 * Examples
 * 
 * Input
 * 
 * fed|cba|
 * Output
 * 
 * abcdef
 */

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

enum {
    DIG_NUM = 10,
    ALPH_NUM = 26,
    FIRST_DIG = '0',
    FIRST_alph = 'a',
    FIRST_ALPH = 'A'
}; 

void modify_set (uint64_t *last_set, uint64_t *curr_set, int symbol) {
    if (isdigit(symbol)) {
        *curr_set |= 1ULL << (symbol - FIRST_DIG);
    } else if (isalpha(symbol)) {
        if (FIRST_ALPH <= symbol && symbol <= FIRST_ALPH + ALPH_NUM - 1) {
            *curr_set |= 1ULL << (symbol - FIRST_ALPH + DIG_NUM);
        } else {
            *curr_set |= 1ULL << (symbol - FIRST_alph + DIG_NUM + ALPH_NUM);
        }
    } else {
        switch (symbol) {
            case '&':
                *last_set &= *curr_set;
                break;
            case '|':
                *last_set |= *curr_set;
                break;
            case '^':
                *last_set ^= *curr_set;
                break;
            case '~':
                *last_set = ~(*last_set);
                break;
        }
        *curr_set = 0;
    }
} 

void print_set(uint64_t set) {
    for (uint16_t cnt = 0; cnt < DIG_NUM + ALPH_NUM + ALPH_NUM ; ++cnt) {
        if ((set >> cnt) & 1ULL) {
            if (cnt < DIG_NUM) {
                putchar(FIRST_DIG + cnt);
            } else if (cnt < DIG_NUM + ALPH_NUM) {
                putchar(FIRST_ALPH + cnt - DIG_NUM);
            } else {
                putchar(FIRST_alph + cnt - DIG_NUM - ALPH_NUM);
            }
        }
    }
} 
int main() {
    uint64_t last = 0, curr = 0;
    for (int c = getchar(); c != EOF && c != '\n'; c = getchar()) {
        modify_set(&last, &curr, c);
    }
    print_set(last);
    putchar('\n');
    return 0;
}

