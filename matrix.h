#pragma once

#include <memory>
#include <span>

struct matrix {
    matrix() = default;

    matrix(size_t rows, size_t cols, bool zero = false) : rows(rows), cols(cols) {
        if (not zero) {
            data.reset(new float[rows * cols]);
        } else {
            data.reset(new float[rows * cols]{});
        }
    }

    matrix(matrix const &other) {
        *this = other;
    }

    matrix(matrix &&other) noexcept {
        *this = std::move(other);
    }

    matrix(std::initializer_list<std::initializer_list<int> > const &other) {
        *this = other;
    }

    matrix &operator=(matrix const &other) {
        rows = other.rows;
        cols = other.cols;
        data.reset(new float[rows * cols]);
        std::copy(other.data.get(), other.data.get() + rows * cols, data.get());
        return *this;
    }

    matrix &operator=(matrix &&other) noexcept {
        cols = other.cols;
        rows = other.rows;
        data = std::move(other.data);
        other.cols = 0;
        other.rows = 0;
        return *this;
    }

    matrix &operator=(std::initializer_list<std::initializer_list<int> > const &other) {
        rows = other.size();
        cols = other.begin()->size();
        data.reset(new float[rows * cols]);
        size_t i = 0;
        for (auto const &row: other) {
            for (auto const &elem: row) {
                data[i++] = elem;
            }
        }
        return *this;
    }

    matrix T() const {
        matrix result(cols, rows);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[j][i] = (*this)[i][j];
            }
        }
        return result;
    }

    std::pair<size_t, size_t> size() const {
        return std::make_pair(cols, rows);
    }

    std::span<float> operator[](size_t i) {
        return {&data[cols * i], cols};
    }

    std::span<float> const operator[](size_t i) const {
        return {&data[cols * i], cols};
    }

    bool operator==(const matrix &other) const {
        if (other.cols != cols or other.rows != rows) {
            return false;
        }
        for (size_t i = 0; i < other.rows; ++i) {
            for (size_t j = 0; j < other.cols; ++j) {
                if ((*this)[i][j] != other[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

private:
    friend std::ostream &operator<<(std::ostream &os, matrix const &matrix);

    size_t rows = 0;
    size_t cols = 0;
    std::unique_ptr<float[]> data;
};

inline std::ostream &operator<<(std::ostream &os, matrix const &matrix) {
    if (matrix.rows == 0) {
        os << "{{}}";
        return os;
    }
    os << '{';
    for (size_t i = 0; i < matrix.rows; ++i) {
        os << '{';
        for (size_t j = 0; j < matrix.cols; ++j) {
            os << matrix[i][j];
            if (j != matrix.cols - 1) {
                os << ", ";
            }
        }
        os << "}";
        if (i != matrix.rows - 1) {
            os << ",\n";
        }
    }
    os << '}';
    return os;
}
