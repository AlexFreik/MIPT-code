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
                patterns.push_back(std::string());
                patt_places.push_back(cnt);
                new_word_key = false;
            }
            patterns.rbegin()->append(1, pat[cnt]);
        }
    }
}
struct Node {
    // variables
    Node *parent = nullptr;
    char char_to_parent;
    std::map<char, Node*> childs;
    Node *suff_link = nullptr;
    Node *short_suff_link = nullptr;
    std::map<char, Node*> go;
    bool is_terminate = false;
    std::vector<std::pair<size_t, size_t>> id;
    // functions
    Node* get_suff_link();
    Node* get_link(char c);
    Node* get_short_suff_link();
    void add_pattern(const std::string &patt, size_t id_number);
    ~Node();
};
Node::~Node() {
    for (auto kv : childs) {
        delete kv.second;
    }
}
Node* Node::get_suff_link() {
    if (this->suff_link == nullptr) {
        if (this->parent == nullptr) {
            this->suff_link = this;
        } else if (this->parent->parent == nullptr) {
            this->suff_link = this->parent;
        } else {
            this->suff_link =
                this->parent->get_suff_link()->get_link(this->char_to_parent);
        }
    }
    return this->suff_link;
}

Node* Node::get_link(char c) {
    if (this->go.find(c) == this->go.end()) {
        if (this->childs.find(c) != this->childs.end()) {
            this->go[c] = this->childs[c];
        } else if (!this->parent) {  // if this == root
            this->go[c] = this;
        } else {
            go[c] = this->get_suff_link()->get_link(c);
        }
    }
    return this->go[c];
}
Node* Node::get_short_suff_link() {
    if (!this->short_suff_link) {
        if (this->get_suff_link()->is_terminate) {
            this->short_suff_link = this->get_suff_link();
        } else if (!this->parent) {
            this->short_suff_link = this;
        } else {
            this->short_suff_link =
                this->get_suff_link()->get_short_suff_link();
        }
    }
    return this->short_suff_link;
}
void Node::add_pattern(const std::string &patt, size_t id_number) {
    assert(this->parent == nullptr);  // check that this is root
    Node *curr = this;
    for (auto c : patt) {
        if (curr->childs.find(c) == curr->childs.end()) {
            curr->childs[c] = new Node;
            curr->childs[c]->parent = curr;
            curr->childs[c]->char_to_parent = c;
        }
        curr = curr->childs[c];
    }
    curr->is_terminate = true;
    assert(patt.size() > 0);
    curr->id.push_back({id_number, patt.size()});
}

void aho_corasick(const std::string &pat,
                  const std::string &text,
                  std::vector<uint32_t> &ans) {
    std::vector<std::string> patterns;
    std::vector<size_t> patt_places;
    // extract patterns without "?" from template
    extract_patterns(pat, patterns, patt_places);
    // start aho_corasick
    Node root;
    for (size_t cnt = 0; cnt < patterns.size(); ++cnt) {
        root.add_pattern(patterns[cnt], patt_places[cnt]);
    }
    Node *curr_state = &root, *node_cnt;
    std::vector<uint32_t> patt_cnt(text.size());
    for (size_t cnt = 0; cnt < text.size(); ++cnt) {
        curr_state = curr_state->get_link(text[cnt]);
        for (node_cnt = (curr_state->is_terminate ? curr_state :
                    curr_state->get_short_suff_link());
             node_cnt != &root;
             node_cnt = node_cnt->get_short_suff_link()) {
            for (auto s : node_cnt->id) {
                if (cnt + 1 >= s.second + s.first) {
                    ++patt_cnt[cnt + 1 - s.second - s.first];
                }
            }
        }
    }
    ans.clear();
    for (uint32_t cnt = 0; cnt < patt_cnt.size(); ++cnt) {
        if (patt_cnt[cnt] == patterns.size() &&
            cnt + pat.size() <= text.size()) {  // check because "?" can be
            ans.push_back(cnt);                 // at the end of template
        }
    }
}

int main(int argc, const char * argv[]) {
    // input
    std::string pat, text;
    std::cin >> pat >> text;
    // algorythm
    std::vector<uint32_t> ans;
    aho_corasick(pat, text, ans);
    // output
    for (auto uint32 : ans) {
        std::cout << uint32 << ' ';
    }
    std::cout << std::endl;
}
