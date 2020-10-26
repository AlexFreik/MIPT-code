/*
https://contest.yandex.ru/contest/20019/run-report/38134933/
D. Количество различных подстрок
 
 Ограничение времени    0.2 секунды
 Ограничение памяти    10Mb
 Ввод    стандартный ввод или input.txt
 Вывод    стандартный вывод или output.txt
 Дана строка длины n. Найти количество ее различных подстрок. Используйте суффиксный массив.
 Построение суффиксного массива выполняйте за O(n log n). Вычисление количества различных подстрок выполняйте за O(n).
 Формат ввода
 Строка.
 Формат вывода
 Число - количество различных подстрок.
 Пример
 Ввод    Вывод
 abab
 7*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <map>

namespace suff_array {
// begin of the namespace
template<typename T>
class suff_array {
    void build_suff_array_preparing();
    void build_suff_array_step(size_t k);
    std::vector<T> eq_class;
    T eq_class_number;

  public:
    explicit suff_array(std::string &&text);
    void kasai();
    size_t substring_number();
    void print_array(bool lcp = false) const;
    std::string text;
    std::vector<T> array;
    std::vector<T> lcp;
};

template<typename T>
void suff_array<T>::build_suff_array_preparing() {
    assert(array.size() == text.size() + 1);
    assert(eq_class.size() == text.size() + 1);
    std::map<char, std::vector<T>> pockets;
    for (T ind = 0; ind < text.size(); ++ind) {
        pockets[text[ind]].push_back(ind);
    }
    eq_class[array.size() - 1] = 0;
    eq_class_number = 1;
    array[0] = static_cast<T>(text.size());
    T ind_cnt = 1;
    for (auto pocket : pockets) {
        for (auto ind : pocket.second) {
            array[ind_cnt] = ind;
            eq_class[ind] = eq_class_number;
            ++ind_cnt;
        }
        ++eq_class_number;
    }
}
template<typename T>
void suff_array<T>::build_suff_array_step(size_t k) {
    assert(k > 0);
    // creates pockets and put suff-s there according to the first part eq_class
    std::vector<std::vector<T>> pockets(eq_class_number);
    T base = static_cast<T>(array.size());
    for (size_t ind = 0; ind < array.size(); ++ind) {
        assert(array[ind] < base && (1 << (k - 1)) < base);
        T array_ind = (array[ind] + base - (1 << (k - 1))) % base;
        pockets[eq_class[array_ind]].push_back(array_ind);
    }
    std::vector<T> temp_eq_classes(eq_class.size());
    eq_class_number = 0;
    T ind_cnt = 0;
    for (auto pocket : pockets) {
        assert(pocket.size() != 0);
        array[ind_cnt] = pocket[0];
        temp_eq_classes[pocket[0]] = eq_class_number;
        ++ind_cnt;
        for (T ind = 1; ind < pocket.size(); ++ind) {
            array[ind_cnt] = pocket[ind];
            assert(pocket[ind] < base && (1 << (k - 1)) < base);
            T second_part_ind = (pocket[ind] + (1 << (k - 1))) % base;
            T prev_2nd_part_ind = (pocket[ind - 1] + (1 << (k - 1))) % base;
            T curr_second_part_class = eq_class[second_part_ind];
            T prev_second_part_class = eq_class[prev_2nd_part_ind];
            assert(curr_second_part_class >= prev_second_part_class);
            if (curr_second_part_class != prev_second_part_class) {
                ++eq_class_number;
            }
            temp_eq_classes[pocket[ind]] = eq_class_number;
            ++ind_cnt;
        }
        ++eq_class_number;
    }
    eq_class = std::move(temp_eq_classes);
}
template<typename T>
suff_array<T>::suff_array(std::string &&text)
                        : text(text),
                        array(std::vector<T>(text.size() + 1)),
                        eq_class(std::vector<T>(text.size() + 1)) {
    build_suff_array_preparing();
    for (size_t k = 1; 1 << (k - 1) < array.size(); ++k) {
        // on each step work with
        build_suff_array_step(k);
        if (eq_class_number == eq_class.size()) {
            break;
        }
    }
}
template<typename T>
void suff_array<T>::print_array(bool lcp) const {
    for (size_t ind = 0; ind < array.size(); ++ind) {
        std::cout << eq_class[array[ind]] << '|' << array[ind] << '|';
        if (lcp && array[ind] != array.size()) {
            std::cout << this->lcp[ind] << '|';
        }
        for (size_t cnt = array[ind]; cnt < text.size(); ++cnt) {
            std::cout << text[cnt];
        }
        std::cout << std::endl;
    }
}
template<typename T>
void suff_array<T>::kasai() {
    lcp.clear();
    if (array.size() == 1) { return; }
    lcp = std::vector<T>(array.size());
    // inv_arr[i] -- number of i suff in arr
    const std::vector<T> &inverse_array = eq_class;
    assert(array[0] == array.size() - 1);
    for (T ind = 0, cnt = inverse_array[0], prev_lcp = 1;
         ind < array.size() - 1;
         prev_lcp = std::max<T>(lcp[cnt], 1), ++ind, cnt = inverse_array[ind]) {
        lcp[cnt] = prev_lcp - 1;
        T prev_suff = array[cnt - 1];
        T max = std::max(prev_suff, ind);
        while (max + lcp[cnt] < text.size() &&
               text[ind + lcp[cnt]] == text[prev_suff + lcp[cnt]]) { ++lcp[cnt]; }
    }
}
template<typename T>
size_t suff_array<T>::substring_number() {
    kasai();
    size_t ret_val = 0;
    for (T ind = 1; ind < lcp.size(); ++ind) {
        ret_val += text.size() - array[ind] - lcp[ind];
    }
    return ret_val;
}
}  // namespace suff_array

int main(int argc, const char * argv[]) {
    // scan date
    std::string text;
    std::cin >> text;
    // funk
    uint64_t ans =
        suff_array::suff_array<uint32_t>(std::move(text)).substring_number();
    // print answer
    std::cout << ans << std::endl;
}
