/*
https://contest.yandex.ru/contest/20019/run-report/36987099/
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
    struct ring {
        explicit ring(size_t base) : base(base) {}
        size_t base;
        T sub(T value, T subb) const;
        T add(T value, T add) const;
    };
    void build_suff_array_first_step();
    void build_suff_array_next_step(size_t k);
    std::vector<T> eq_class;
    T eq_class_number;
    ring R;

  public:
    explicit suff_array(std::string &&text);
    void kasai();
    size_t substring_number();
    void print_array(bool lcp = false) const;
    std::string text;
    std::vector<T> array;
    std::vector<T> lcp;
};
// ring nested class funktions
template<typename T>
T suff_array<T>::ring::sub(T value, T subb) const {
    assert(value < base && subb < base);
    if (value >= subb) {
        value -= subb;
    } else {
        value +=  base - subb;
    }
    value = value - static_cast<T>((value / base) * base);
    if (value < 0) {
        value += base;
    }
    return value;
}
template<typename T>
T suff_array<T>::ring::add(T value, T add) const {
    assert(add < base && value < base);
    value += add;
    if (value > base) {
        value -= base;
    }
    return value;
}
// ring nested class funktions

template<typename T>
void suff_array<T>::build_suff_array_first_step() {
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
void suff_array<T>::build_suff_array_next_step(size_t k) {
    // create pockets and put suff-s there according to the first part eq_class
    std::vector<std::vector<T>> pockets(eq_class_number);
    for (size_t ind = 0; ind < array.size(); ++ind) {
        T array_ind = R.sub(array[ind], 1 << (k-1));
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
            T curr_second_part_class = eq_class[R.add(pocket[ind], 1 << (k-1))],
            prev_second_part_class = eq_class[R.add(pocket[ind - 1], 1 << (k-1))];
            assert(curr_second_part_class >= prev_second_part_class);
            if (curr_second_part_class != prev_second_part_class) {
                ++eq_class_number;
            }
            temp_eq_classes[pocket[ind]] = eq_class_number;
            ++ind_cnt;
        }
        ++eq_class_number;
    }
    eq_class = temp_eq_classes;
}
template<typename T>
suff_array<T>::suff_array(std::string &&text)
                        : text(text),
                        array(std::vector<T>(text.size() + 1)),
                        eq_class(std::vector<T>(text.size() + 1)),
                        R(text.size() + 1) {
    build_suff_array_first_step();
    for (size_t k = 0; 1 << k < array.size(); ++k) {
        build_suff_array_next_step(k + 1);
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
T diff(T a, T b) {
    if (a >= b) return a - b;
    return b - a;
}
template<typename T>
void suff_array<T>::kasai() {
    lcp.clear();
    if (array.size() == 1) { return; }
    lcp = std::vector<T>(array.size());
    // inv_arr[i] -- number of i suff in arr
    std::vector<T> inverse_array(array.size());
    for (T ind = 0; ind < array.size(); ++ind) {
        inverse_array[array[ind]] = ind;
    }
    assert(array[0] == array.size() - 1);
    T cnt = inverse_array[0];
    for (size_t ind2 = array[cnt - 1]; ind2 < text.size(); ++ind2) {
        if (text[ind2] == text[ind2 - array[cnt - 1] + 0]) {
            ++lcp[cnt];
        } else {
            break;
        }
    }
    for (T ind = 1; ind < array.size() - 1; ++ind) {
        T cnt = inverse_array[ind], prev_lcp = lcp[inverse_array[ind - 1]];
        lcp[cnt] = (prev_lcp > 1 ? prev_lcp - 1 : 0);
        T dif = diff(ind, array[cnt - 1]), min = std::min(array[cnt - 1], ind);
        for (size_t ind2 = min + dif + lcp[cnt]; ind2 < text.size(); ++ind2) {
            if (text[ind2] == text[ind2 - dif]) {
                ++lcp[cnt];
            } else {
                break;
            }
        }
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
