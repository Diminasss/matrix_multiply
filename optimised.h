#pragma once

#include "matrix.h"

// Используется блочное умножение матриц
inline matrix optimised_multiply(matrix const &a, matrix const &b) {
    // Получен размер матрицы A: количество столбцов и строк
    auto [a_cols, a_rows] = a.size();
    // Получен размер матрицы B: количество столбцов и строк
    auto [b_cols, b_rows] = b.size();
    // Создаётся нулевая матрица размером (строки A) * (столбцы B)
    matrix result(a_rows, b_cols, true);
    // Матрица В транспонируется, чтобы столбцы стали строками - улучшается доступ по памяти и
    //удобство использования
    matrix b_T = b.T();


    constexpr size_t BLOCK_SIZE = 64; // Размер блока. Подбирается по размеру L1-кэша (пример: 64 — это 64×64×4 = 16КБ)
    // В И5 10400 кэш L1 - 64КБ на ядро - по полам L1d, L1i, то есть 32КБ на ядро
    // Какая-то часть будет занята программой, рассчитывается, что в L1 будет 16КБ памяти
    // размер блока: 64×64 (влезает в L1) так как 64 * 64 * 4(float = 4Б) = 16384 Б = 16 КБ


    // Внешний тройной цикл: матрицы разбиваются на блоки
    for (size_t ii = 0; ii < a_rows; ii += BLOCK_SIZE) {
        for (size_t jj = 0; jj < b_cols; jj += BLOCK_SIZE) {
            for (size_t kk = 0; kk < a_cols; kk += BLOCK_SIZE) {

                // Внутренние циклы: перемножение блоков
                for (size_t i = ii; i < std::min(ii + BLOCK_SIZE, a_rows); ++i) {

                    // Берётся строка i из A
                    std::span<float const> row_a = a[i];
                    // Берётся строка i из результата
                    std::span<float> row_result = result[i];

                    for (size_t j = jj; j < std::min(jj + BLOCK_SIZE, b_cols); ++j) {
                        // Берётся строка j из транспонированной B (столбец из B)
                        std::span<float const> row_bT = b_T[j];
                        // Получаем текущую сумму в ячейке [i][j] (в начале — 0)
                        float sum = row_result[j];

                        for (size_t k = kk; k < std::min(kk + BLOCK_SIZE, a_cols); ++k) {
                            // Скалярное произведение двух строк (A[i] и B^T[j])
                            sum += row_a[k] * row_bT[k];
                        }
                        // Записывает сумму в пределах блока в соответствующую ячейку результата
                        row_result[j] = sum;
                    }
                }
            }
        }
    }

    return result; // Возвращает готовую перемноженную матрицу
}