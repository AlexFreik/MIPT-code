/*
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

void template_search(const std::string &temp,
                    std::string &text) {
    size_t t = temp.size(), n = text.size() + temp.size();
    text = temp + text;
    std::vector<size_t> preff(t);
    //  calculate z function for template
    for (size_t left = 0, right = 0, cnt = 1; cnt < t; ++cnt) {
        if (right >= cnt) {
            preff[cnt] = std::min(preff[cnt - left], right - cnt + 1);
        }
        while (cnt + preff[cnt] < n &&
               text[preff[cnt]] == text[cnt + preff[cnt]]) {
            ++preff[cnt];
        }
        if (cnt + preff[cnt] - 1 > right) {
            left = cnt;
            right = cnt + preff[cnt] - 1;
        }
    }
    //  calculate z function for text and cout immediately
    for (size_t left = 0, right = 0, cnt = t, cnt_preff = 0; cnt < n; ++cnt) {
        if (right >= cnt) {
            cnt_preff = std::min(preff[cnt - left], right - cnt + 1);
        }
        while (cnt + cnt_preff < n && cnt_preff < t &&
               text[cnt_preff] == text[cnt + cnt_preff]) {
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
