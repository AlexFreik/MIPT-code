/*
https://contest.yandex.ru/contest/19772/run-report/37321928/
 A. Поиск подстроки
 Язык    Ограничение времени    Ограничение памяти    Ввод    Вывод
 Все языки    0.1 секунда    32Mb    стандартный ввод или input.txt    стандартный вывод или output.txt
 Oracle Java 7    1 секунда    192Mb
 Python 3.7.3    1 секунда    192Mb
 Python 3.6    1 секунда    192Mb
 Oracle Java 8    1 секунда    192Mb
 Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки – n. Время O(n + p), доп. память – O(p).
 p ≤ 30000, n ≤ 300000.
 Использовать один из методов:
 Вариант 1. С помощью префикс-функции;
 Вариант 2. С помощью z-функции.
 Формат ввода
 Шаблон, символ перевода строки, строка.
 Формат вывода
 Позиции вхождения шаблона в строке.
 Пример
 Ввод    Вывод
 a
 aaa
 0 1 2
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

char get_value(const std::string &temp,
                 const std::string &text,
                 size_t index) {
    assert(index < temp.size() + text.size());
    char retVal;
    if (index < temp.size()) {
        retVal = temp[index];
    } else {
        retVal = text[index - temp.size()];
    }
    return retVal;
}

void z_func_for_template_calculation(const std::string &temp,
                                     const std::string &text,
                                     std::vector<size_t> &preff) {
    //  calculate z function for template
    for (size_t left = 0, right = 0, cnt = 1; cnt < temp.size(); ++cnt) {
        if (right >= cnt) {
            preff[cnt] = std::min(preff[cnt - left], right - cnt + 1);
        }
        while (cnt + preff[cnt] < temp.size() + text.size() &&
               get_value(temp, text, preff[cnt]) ==
               get_value(temp, text, cnt + preff[cnt])) {
            ++preff[cnt];
        }
        if (cnt + preff[cnt] - 1 > right) {
            left = cnt;
            right = cnt + preff[cnt] - 1;
        }
    }
}
void template_search(const std::string &temp,
                     const std::string &text) {
    size_t t = temp.size(), n = text.size() + temp.size();
    std::vector<size_t> preff(t);
    z_func_for_template_calculation(temp, text, preff);
    //  calculate z function for text and cout immediately
    for (size_t left = 0, right = 0, cnt = t, cnt_preff = 0; cnt < n; ++cnt) {
        if (right >= cnt) {
            cnt_preff = std::min(preff[cnt - left], right - cnt + 1);
        }
        while (cnt + cnt_preff < n && cnt_preff < t &&
               get_value(temp, text, cnt_preff) ==
               get_value(temp, text, cnt + cnt_preff)) {
            ++cnt_preff;
        }
        if (cnt + cnt_preff - 1 > right) {
            left = cnt;
            right = cnt + cnt_preff - 1;
        }
        if (cnt_preff >= t) {
            std::cout << cnt - t << ' ';
        }
    }
}

int main(int argc, const char * argv[]) {
    // input
    std::string temp, text;
    std::cin >> temp >> text;
    // algorithm
    template_search(temp, text);
    // output
    std::cout << std::endl;
}
