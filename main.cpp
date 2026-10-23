#include <iostream>
#include <new>
#include <algorithm>

void cleanup(int** m, size_t rows) {
    if (!m) return;
    for (size_t i = 0; i < rows; ++i) {
        delete[] m[i];
    }
    delete[] m;
}

int** alloc_matrix(size_t rows, size_t cols) {
    int** m = nullptr;
    try {
        m = new int*[rows];
        for (size_t i = 0; i < rows; ++i) {
            m[i] = nullptr;
        }
        for (size_t i = 0; i < rows; ++i) {
            m[i] = new int[cols];
        }
    } catch (std::bad_alloc&) {
        cleanup(m, rows);
        return nullptr;
    }
    return m;
}

int** concat_rows(const int * const * lhs, size_t n1, size_t m1,
                  const int * const * rhs, size_t n2, size_t m2) {
    size_t max_n = std::max(n1, n2);
    int** res = nullptr;
    
    try {
        res = new int*[max_n];
        for (size_t i = 0; i < max_n; ++i) {
            res[i] = nullptr;
        }
        
        for (size_t i = 0; i < max_n; ++i) {
            bool has_lhs = (i < n1);
            bool has_rhs = (i < n2);
            
            size_t len = (has_lhs ? m1 : 0) + (has_rhs ? m2 : 0);
            res[i] = new int[len];
            
            size_t k = 0;
            if (has_lhs) {
                for (size_t j = 0; j < m1; ++j) {
                    res[i][k++] = lhs[i][j];
                }
            }
            if (has_rhs) {
                for (size_t j = 0; j < m2; ++j) {
                    res[i][k++] = rhs[i][j];
                }
            }
        }
    } catch (std::bad_alloc&) {
        cleanup(res, max_n);
        return nullptr;
    }
    return res;
}

int main() {
    long long r1, c1, r2, c2;
    
    if (!(std::cin >> r1 >> c1 >> r2 >> c2) || r1 < 0 || c1 < 0 || r2 < 0 || c2 < 0) {
        std::cerr << "Error: invalid input\n";
        return 1;
    }

    size_t n1 = r1, m1 = c1, n2 = r2, m2 = c2;

    int** lhs = alloc_matrix(n1, m1);
    if (!lhs) return 2;

    for (size_t i = 0; i < n1; ++i) {
        for (size_t j = 0; j < m1; ++j) {
            if (!(std::cin >> lhs[i][j])) {
                cleanup(lhs, n1);
                return 1;
            }
        }
    }

    int** rhs = alloc_matrix(n2, m2);
    if (!rhs) {
        cleanup(lhs, n1);
        return 2;
    }

    for (size_t i = 0; i < n2; ++i) {
        for (size_t j = 0; j < m2; ++j) {
            if (!(std::cin >> rhs[i][j])) {
                cleanup(lhs, n1);
                cleanup(rhs, n2);
                return 1;
            }
        }
    }

    int** res = concat_rows(lhs, n1, m1, rhs, n2, m2);
    if (!res) {
        cleanup(lhs, n1);
        cleanup(rhs, n2);
        return 2;
    }

    size_t max_n = std::max(n1, n2);
    for (size_t i = 0; i < max_n; ++i) {
        size_t len = (i < n1 ? m1 : 0) + (i < n2 ? m2 : 0);
        for (size_t j = 0; j < len; ++j) {
            std::cout << res[i][j] << " ";
        }
        std::cout << "\n";
    }

    cleanup(lhs, n1);
    cleanup(rhs, n2);
    cleanup(res, max_n);

    return 0;
}
