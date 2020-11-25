// https://contest.yandex.ru/contest/19772/run-report/42569502/
//
//  С_1task.cpp
//  Strings
//
//  Created by Alex Freik on 25.11.2020.
//


/*
C. Шаблон с ?
Все языки    make2
Ограничение времени    10 секунд    80 секунд
Ограничение памяти    20Mb    400Mb
Ввод    стандартный ввод или input.txt
Вывод    стандартный вывод или output.txt
Шаблон поиска задан строкой длины m, в которой кроме обычных символов
 могут встречаться символы “?”. Найти позиции всех вхождений шаблона в
 тексте длины n. Каждое вхождение шаблона предполагает, что все обычные
 символы совпадают с соответствующими из текста, а вместо символа “?” в
 тексте встречается произвольный символ. Гарантируется, что сам “?” в тексте
 не встречается.
Время работы - O(n + m + Z), где Z - общее число вхождений подстрок шаблона
 “между вопросиками” в исходном тексте. m ≤ 5000, n ≤ 2000000.
Пример 1
Ввод    Вывод
ab??aba
ababacaba
2
Пример 2
Ввод    Вывод
aa??bab?cbaa?
aabbbabbcbaabaabbbabbcbaab
0 13
 */

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <map>
#include <utility>


namespace string_func {
struct suffix_tree {
    struct Node {
        struct id_type {
            id_type(size_t id, size_t size) : id_number(id), patt_size(size) {}
            size_t id_number, patt_size;
        };
        /// variables
        Node *parent = nullptr;
        char char_to_parent;
        std::map<char, Node*> childs;
        Node *suff_link = nullptr;
        Node *short_suff_link = nullptr;
        std::map<char, Node*> go;
        bool is_terminate = false;
        std::vector<id_type> id;
        /// functions
        ~Node();
    };
    suffix_tree(const std::string &pat, const std::string &text,
                size_t &get_patt_number);
    Node* get_suff_link(Node *node);
    Node* get_link(Node *node, char c);
    Node* get_short_suff_link(Node *node);
    void add_pattern(const std::string &patt, size_t id_number);
    /// variables
    Node *root;
    ~suffix_tree() { delete root; }
};
void extract_patterns(const std::string &pat,
                      std::vector<std::string> &patterns,
                      std::vector<size_t> &patt_places) {
    patterns.clear();
    patt_places.clear();
    for (size_t cnt = 0, new_word_key = true; cnt < pat.size(); ++cnt) {
        if (pat[cnt] == '?') {
            new_word_key = true;
        } else {
            if (new_word_key) {
                patterns.emplace_back();
                patt_places.push_back(cnt);
                new_word_key = false;
            }
            patterns.back().push_back(pat[cnt]);
        }
    }
}
suffix_tree::suffix_tree(const std::string &pat, const std::string &text,
                         size_t &get_patt_number) {
    root = new Node;
    std::vector<std::string> patterns;
    std::vector<size_t> patt_places;
    /// extract patterns without "?" from template
    extract_patterns(pat, patterns, patt_places);
    for (size_t cnt = 0; cnt < patterns.size(); ++cnt) {
        add_pattern(patterns[cnt], patt_places[cnt]);
    }
    get_patt_number = patterns.size();
}
suffix_tree::Node::~Node() {
    for (auto kv : childs) {
        delete kv.second;
    }
}
suffix_tree::Node* suffix_tree::get_suff_link(Node *node) {
    if (node->suff_link == nullptr) {
        if (node->parent == nullptr) {
            node->suff_link = node;
        } else if (node->parent->parent == nullptr) {
            node->suff_link = node->parent;
        } else {
            node->suff_link =
                get_link(get_suff_link(node->parent), node->char_to_parent);
        }
    }
    return node->suff_link;
}

suffix_tree::Node* suffix_tree::get_link(Node *node, char c) {
    if (node->go.find(c) == node->go.end()) {
        if (node->childs.find(c) != node->childs.end()) {
            node->go[c] = node->childs[c];
        } else if (!node->parent) {  // if this == root
            node->go[c] = node;
        } else {
            node->go[c] = get_link(get_suff_link(node), c);
        }
    }
    return node->go[c];
}
suffix_tree::Node* suffix_tree::get_short_suff_link(Node *node) {
    if (!node->short_suff_link) {
        if (get_suff_link(node)->is_terminate) {
            node->short_suff_link = get_suff_link(node);
        } else if (!node->parent) {
            node->short_suff_link = node;
        } else {
            node->short_suff_link = get_short_suff_link(get_suff_link(node));
        }
    }
    return node->short_suff_link;
}
void suffix_tree::add_pattern(const std::string &patt, size_t id_number) {
    assert(root->parent == nullptr);  // check that this is root
    suffix_tree::Node *curr = root;
    for (auto c : patt) {
        if (curr->childs.find(c) == curr->childs.end()) {
            curr->childs[c] = new suffix_tree::Node;
            curr->childs[c]->parent = curr;
            curr->childs[c]->char_to_parent = c;
        }
        curr = curr->childs[c];
    }
    curr->is_terminate = true;
    assert(patt.size() > 0);
    curr->id.emplace_back(id_number, patt.size());
}

void aho_corasick(const std::string &pat,
                  const std::string &text,
                  std::vector<uint32_t> &ans) {
    // extract patterns without "?" from template ans start aho corasick
    size_t patterns_number;
    suffix_tree suff_tree(pat, text, patterns_number);
    suffix_tree::Node *curr_state = suff_tree.root, *node_cnt;
    std::vector<uint32_t> patt_cnt(text.size());
    for (size_t cnt = 0; cnt < text.size(); ++cnt) {
        curr_state = suff_tree.get_link(curr_state, text[cnt]);
        for (node_cnt = (curr_state->is_terminate ? curr_state :
                         suff_tree.get_short_suff_link(curr_state));
             node_cnt != suff_tree.root;
             node_cnt = suff_tree.get_short_suff_link(node_cnt)) {
            for (auto s : node_cnt->id) {
                if (cnt + 1 >= s.patt_size + s.id_number) {
                    ++patt_cnt[cnt + 1 - s.patt_size - s.id_number];
                }
            }
        }
    }
    ans.clear();
    for (uint32_t cnt = 0; cnt < patt_cnt.size(); ++cnt) {
        if (patt_cnt[cnt] == patterns_number &&
            cnt + pat.size() <= text.size()) {
            /// check because "?" can be at the end of template
            ans.push_back(cnt);
        }
    }
}
}


int main(int argc, const char * argv[]) {
    /// input processing
    std::string pat, text;
    std::cin >> pat >> text;
    /// answer calculation
    std::vector<uint32_t> ans;
    string_func::aho_corasick(pat, text, ans);
    /// output processing
    for (auto uint32 : ans) {
        std::cout << uint32 << ' ';
    }
    std::cout << std::endl;
}
