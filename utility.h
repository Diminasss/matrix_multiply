#pragma  once

#include "matrix.h"

inline void timespec_diff(timespec *start, timespec *stop,
                          timespec *result) {
    if ((stop->tv_nsec - start->tv_nsec) < 0) {
        result->tv_sec = stop->tv_sec - start->tv_sec - 1;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        result->tv_sec = stop->tv_sec - start->tv_sec;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }
}

inline bool basic_test(matrix (*multiply)(matrix const &, matrix const &)) {
    matrix a, b, c;

    auto test = [&]() {
        matrix result = multiply(a, b);
        if (result != c) {
            std::cout << "Matrix multiplication test failed:\n" << a << "\n*\n" << b << "\nmust be equal to\n" << c <<
                    " but got\n" << result << std::endl;
            return false;
        }
        return true;
    };

    a = {{-3, 1}};
    b = {{4}, {8}};
    c = {{-4}};

    if (not test()) {
        return false;
    }

    a = {{1, -4}, {-5, 5}};
    b = {{2, 3, 5}, {-2, -6, -3}};
    c = {
        {10, 27, 17},
        {-20, -45, -40}
    };

    if (not test()) {
        return false;
    }

    a = {{-9, 5, 9}, {2, 10, -5}};
    b = {
        {-6, -4, 1, -10},
        {-2, 8, -9, -2},
        {-3, -10, 5, -8}
    };
    c = {
        {17, -14, -9, 8},
        {-17, 122, -113, 0}
    };

    if (not test()) {
        return false;
    }

    a = {{0, -7, -1}, {5, -8, 0}, {2, 2, 3}};
    b = {{3}, {8}, {9}};
    c = {
        {-65},
        {-49},
        {49}
    };

    if (not test()) {
        return false;
    }

    a = {{-7, -4}, {-6, -3}, {0, 10}};
    b = {{-6}, {-9}};
    c = {
        {78},
        {63},
        {-90}
    };

    if (not test()) {
        return false;
    }

    a = {{1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}};
    b = {{1, 5}, {2, 6}, {3, 7}, {4, 8}, {9, 10}};
    c = {{19, 36}, {19, 36}, {19, 36}, {19, 36}};

    if (not test()) {
        return false;
    }

    return true;
}
