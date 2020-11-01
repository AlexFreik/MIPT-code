/*
https://contest.yandex.ru/contest/19772/run-report/37323276/
 B. Палиндромы
 
 Ограничение времени    1 секунда
 Ограничение памяти    10Mb
 Ввод    стандартный ввод или input.txt
 Вывод    стандартный вывод или output.txt
 Строка называется палиндромом, если она одинаково читается как слева направо,
 так и справа налево. Например, «abba» – палиндром, а «roman» – нет.
 Для заданной строки s длины n (1 ≤ n ≤ 105) требуется подсчитать число
 пар (i, j), 1 ≤ i < j ≤ n, таких что подстрока s[i..j] является палиндромом.
 Формат ввода
 
 Входной файл содержит одну строку s длины n, состоящюю
 из маленьких латинских букв.
 Формат вывода
 
 В выходной файл выведите искомое число подстрок-палиндромов.
 Пример 1
 
 Ввод    Вывод
 aaa
 3
 Пример 2
 
 Ввод    Вывод
 abba
 2
 Пример 3
 
 Ввод    Вывод
 roman
 0
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

// T is unsigned value that large enough for text.size()
namespace string_functions {
template<typename T>
std::vector<T> palindromes_search(const std::string &text, bool is_even) {
    std::vector<T> pal(text.size(), 0);
    for (T left = 0, right = 0, cnt = 1; cnt < text.size(); ++cnt) {
        if (right >= cnt) {
            pal[cnt] = std::min(right - cnt + is_even,
                                pal[left + (right - cnt) + is_even]);
        }
        while (0 < cnt - pal[cnt] &&
               cnt + pal[cnt] + !is_even < text.size() &&
               text[cnt - pal[cnt] - 1] == text[cnt + pal[cnt] + !is_even]) {
            ++pal[cnt];
        }
        if (cnt + pal[cnt] - is_even > right) {
            right = cnt + pal[cnt] - is_even;
            left = cnt - pal[cnt];
        }
    }
    return pal;
}
template<typename T>
uint64_t manacher(const std::string &text) {
    std::vector<T> odd_pal(palindromes_search<T>(text, false));
    std::vector<T> even_pal(palindromes_search<T>(text, true));
    uint64_t ans = 0;
    for (auto num : odd_pal) {
        if (num) {
            ans += num;
        }
    }
    for (auto num : even_pal) {
        if (num) {
            ans += num;
        }
    }
    return ans;
}
}  // namespace string_functions

int main(int argc, const char * argv[]) {
    // input
    std::string text;
    std::cin >> text;
    assert(text.size() <= 100000);
    // algorithm
    uint64_t ans = string_functions::manacher<uint32_t>(text);
    // output
    std::cout << ans << std::endl;
}
