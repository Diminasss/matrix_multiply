#pragma once

#include "matrix.h"

inline matrix basic_multiply(matrix const &a, matrix const &b) {
    auto [a_cols, a_rows] = a.size();
    auto [b_cols, b_rows] = b.size();
    matrix result(a_rows, b_cols);

    for (size_t i = 0; i < a_rows; ++i) {
        for (size_t j = 0; j < b_cols; ++j) {
            result[i][j] = 0;
            for (size_t k = 0; k < b_rows; ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return result;
}