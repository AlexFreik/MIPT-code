template<typename T>
struct AVL_node {
    AVL_node *left, *right;
    T key;
    int height;
    int children;
    AVL_node(const T &_key) : key(_key) {
        left = right = nullptr;
        height = 1;
        children = 0;
    }
    ~AVL_node() {
        delete left;
        delete right;
    }
    int getBalanceFactor() const {
        int r = right == nullptr ? 0 : right->height;
        int l = left == nullptr ? 0 : left->height;
        return r - l;
    }
    void fix() {
        int r = right == nullptr ? 0 : right->height;
        int l = left == nullptr ? 0 : left->height;
        height = (r > l ? r : l) + 1;
        r = right == nullptr ? -1 : right->children;
        l = left == nullptr ? -1 : left->children;
        children = r + l + 2;
    }
    AVL_node *balance() {
        fix();
        switch (getBalanceFactor()) {
        case -2:
            if (left->getBalanceFactor() > 0) left = left->rotateLeft();
            return rotateRight();
        case 2:
            if (right->getBalanceFactor() < 0) right = right->rotateRight();
            return rotateLeft();
        default:
            return this;
        }
    }
    AVL_node *rotateRight() {
        auto result = left;
        left = result->right;
        result->right = this;
        fix();
        result->fix();
        return result;
    }
    AVL_node *rotateLeft() {
        auto result = right;
        right = result->left;
        result->left = this;
        fix();
        result->fix();
        return result;
    }
    AVL_node *insert(const T &_key, bool(*lesser)(const T &, const T &)) {
        if (lesser(_key, key))
           left = left == nullptr ? new AVL_node(_key) :
                                                left->insert(_key, lesser);
        else
           right = right == nullptr ? new AVL_node(_key) :
                                                right->insert(_key, lesser);
        return balance();
    }
    AVL_node *findMinimum() {
       return left != nullptr ? left->findMinimum() : this;
    }
    AVL_node *removeMinimum() {
        if (left == nullptr)    return right;
        left = left->removeMinimum();
        return balance();
    }
    static AVL_node *remove(AVL_node *p, const T &_key,
                                    bool(*lesser)(const T &, const T &)) {
        if (p == nullptr) return nullptr;
        if (lesser(_key, p->key)) {
            p->left = remove(p->left, _key, lesser);
            return p->balance();
        } else if (lesser(p->key, _key)) {
            p->right = remove(p->right, _key, lesser);
            return p->balance();
        } else {
            AVL_node *l = p->left;
            AVL_node *r = p->right;
            p->left = p->right = nullptr;
            delete p;
            if (r == nullptr) return l;
            AVL_node *min = r->findMinimum();
            min->right = r->removeMinimum();
            min->left = l;
            return min->balance();
        }
    }
    T &k_por_stat(int k) {
        int l = left == nullptr ? 0 : (left->children + 1);
        if (l > k) return left->k_por_stat(k);
        if (l == k) return this->key;
        return right->k_por_stat(k - l - 1);
    }
    void print(int level) const {
        if (this == nullptr) return;
        right->print(level + 1);
        for (int i = 0; i < level; i++) std::cout << "  ";
        std::cout << key << ";" << children << std::endl;
        left->print(level+1);
    }
};

template<typename T>
struct AVLTree {
    AVL_node<T> *root;
    bool (*comp)(const T &, const T &);
    AVLTree(bool (*islesser)(const T &, const T &)) : comp(islesser) {
        root = nullptr;
    }
    ~AVLTree() {
        delete root;
    }
    void print() const {
        if (root != nullptr) root->print(0);
    }

    bool insert(const T &_key) {
        if (root == nullptr) root = new AVL_node<T>(_key);
        else
            root = root->insert(_key, comp);
        return true;
    }
    bool remove(const T &_key) {
        root = AVL_node<int>::remove(root, _key, comp);
        return true;
    }
    T &k_por_stat(int k) {
        return root->k_por_stat(k);
    }
};
