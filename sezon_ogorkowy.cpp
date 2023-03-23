#include <iostream>
#include <algorithm>
#include <utility>

int board[1000007];

void create_subtree(int n, int m, int x_pos, int x_from, int x_to , int y_pos, int y_from, int y_to, std::pair<int, int>** tree) {    
    if (y_from == y_to && x_from == x_to) {
        tree[x_pos][y_pos] = std::make_pair(board[y_from + m * x_from], board[y_from + m * x_from]);
    }
    else if (y_from == y_to) {
        auto first = tree[2 * x_pos][y_pos];
        auto second = tree[2 * x_pos + 1][y_pos];
        tree[x_pos][y_pos] = std::make_pair(std::min(first.first, second.first), std::max(first.second, second.second));
    }
    else {
        int y_mid = (y_from + y_to) / 2;
        create_subtree(n, m, x_pos, x_from, x_to, 2 * y_pos, y_from, y_mid, tree);
        create_subtree(n, m, x_pos, x_from, x_to, 2 * y_pos + 1, y_mid + 1, y_to, tree);
        auto first = tree[x_pos][2 * y_pos];
        auto second = tree[x_pos][2 * y_pos + 1];
        
        tree[x_pos][y_pos] = std::make_pair(std::min(first.first, second.first), std::max(first.second, second.second));
    }
}

void create_tree(int n, int m, int x_pos, int x_from, int x_to, std::pair<int, int>** tree) {
    if (x_from != x_to) {
        int x_mid = (x_from + x_to) / 2;
        create_tree(n, m, 2 * x_pos, x_from, x_mid, tree);
        create_tree(n, m, 2 * x_pos + 1, x_mid + 1, x_to, tree);
    }
    create_subtree(n, m, x_pos, x_from, x_to, 1, 0, m - 1, tree);
}

std::pair<int, int> justice_subtree(int n, int m, int x_pos, int y_pos, int max_y_from, int max_y_to, int y_from, int y_to, std::pair<int, int>** tree) {
    if (y_from > y_to) {
        return { INT32_MAX, INT32_MIN };
    }
    else if (max_y_from == y_from && max_y_to == y_to) {
        auto val = tree[x_pos][y_pos];
        return val;
    }
    else {
        int max_y_mid = (max_y_from + max_y_to) / 2;
        std::pair<int, int> first = justice_subtree(n, m, x_pos, 2 * y_pos, max_y_from, max_y_mid, y_from, std::min(y_to, max_y_mid), tree);
        std::pair<int, int> second = justice_subtree(n, m, x_pos, 2 * y_pos + 1, max_y_mid + 1, max_y_to, std::max(y_from, max_y_mid + 1), y_to, tree);
        if (first == std::make_pair(-1, -1)) {
            return second;
        }
        else {
            return std::make_pair(std::min(first.first, second.first), std::max(first.second, second.second));
        }
    }
}

std::pair<int, int> justice_tree(int n, int m, int x_pos, int max_x_from, int max_x_to, int x_from, int x_to, int y_from, int y_to, std::pair<int, int>** tree) {
    if (x_from > x_to) {
        return { INT32_MAX, INT32_MIN };
    }
    else if (max_x_from == x_from && max_x_to == x_to) {
        return justice_subtree(n, m, x_pos, 1, 0 , m - 1, y_from, y_to, tree);
    }
    else {
        int max_x_mid = (max_x_from + max_x_to) / 2;
        std::pair<int, int> first = justice_tree(n, m, 2 * x_pos, max_x_from, max_x_mid, x_from, std::min(x_to, max_x_mid), y_from, y_to, tree);
        std::pair<int, int> second = justice_tree(n, m, 2 * x_pos + 1, max_x_mid + 1, max_x_to, std::max(x_from, max_x_mid + 1), x_to, y_from, y_to, tree);
        if (first == std::make_pair(INT32_MAX, INT32_MIN)) {
            return second;
        }
        else {
            return std::make_pair(std::min(first.first, second.first), std::max(first.second, second.second));
        }
    }
}

int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    int n, m, k;
    std::cin >> n >> m >> k;

    std::pair<int, int>** tree = new std::pair<int, int>*[4 * n];

    for (int i = 0; i < 4 * n; ++i) {
        tree[i] = new std::pair<int, int>[4 * m];
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            std::cin >> board[j + m * i];
        }
    }

    create_tree(n, m, 1, 0, n - 1, tree);
    
    int x_a, y_a, x_b, y_b;

    for (int i = 0; i < k; ++i) {
        std::cin >> x_a >> y_a >> x_b >> y_b;
        auto res = justice_tree(n, m, 1, 0, n - 1, x_a, x_b, y_a, y_b, tree);
        std::cout << res.second - res.first << std::endl;
    }
    
    for (int i = 0; i < 4 * n; ++i) {
        delete[] tree[i];
    }
    delete[] tree;
}
