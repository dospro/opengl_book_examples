#include "matrix.h"

#include <cmath>

Matrix Matrix::make_identity() {
    Matrix m;
    // clang-format off
    m.data = {
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0,
    };
    // clang-format on
    return m;
}
Matrix Matrix::make_translate(float x, float y, float z) {
    Matrix m;
    // clang-format off
    m.data = {
            1.0, 0.0, 0.0, x,
            0.0, 1.0, 0.0, y,
            0.0, 0.0, 1.0, z,
            0.0, 0.0, 0.0, 1.0,
    };
    // clang-format on
    return m;
}
Matrix Matrix::make_rotate_x(float rad) {
    Matrix m;
    // clang-format off
    m.data = {
            1.0, 0.0, 0.0, 0.0,
            0.0, cos(rad), -sin(rad), 0.0,
            0.0, sin(rad), cos(rad), 0.0,
            0.0, 0.0, 0.0, 1.0,
    };
    // clang-format on
    return m;
}
Matrix Matrix::make_rotate_y(float rad) {
    Matrix m;
    // clang-format off
    m.data = {
            cos(rad), 0.0, sin(rad), 0.0,
            0.0, 1.0, 0.0, 0.0,
            -sin(rad), 0.0, cos(rad), 0.0,
            0.0, 0.0, 0.0, 1.0,
    };
    // clang-format on
    return m;
}
Matrix Matrix::make_rotate_z(float rad) {
    Matrix m;
    // clang-format off
    m.data = {
            cos(rad), -sin(rad), 0.0, 0.0,
            sin(rad), cos(rad), 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0,
    };
    // clang-format on
    return m;
}
Matrix Matrix::make_scale(float x, float y, float z) {
    Matrix m;
    // clang-format off
    m.data = {
            x, 0.0, 0.0, 0.0,
            0.0, y, 0.0, 0.0,
            0.0, 0.0, z, 0.0,
            0.0, 0.0, 0.0, 1.0,
    };
    // clang-format on
    return m;
}
Matrix Matrix::make_perspective(float fov, float aspect, float near, float far) {
    Matrix m;
    float q = 1 / (tan(fov / 2.0f));
    float a = q / aspect;
    float b = (near + far) / (near - far);
    float c = (2 * (near * far)) / (near - far);

    // clang-format off
    m.data = {
            a, 0, 0, 0,
            0, q, 0, 0,
            0, 0, b, c,
            0, 0, -1, 0};
    // clang-format on
    return m;
}
Matrix Matrix::operator+(const Matrix& m) {
    Matrix r;
    for (int i = 0; i < 16; ++i) {
        r.data[i] = this->data[i] + m.data[i];
    }
    return r;
}
Matrix Matrix::operator-(const Matrix& m) {
    Matrix r;
    for (int i = 0; i < 16; ++i) {
        r.data[i] = this->data[i] - m.data[i];
    }
    return r;
}
Matrix Matrix::operator*(const Matrix& m) {
    Matrix result;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            float value = 0;
            for (int i = 0; i < 4; ++i) {
                int va = row * 4 + i;
                int vb = i * 4 + col;
                value += this->data[va] * m.data[vb];
            }
            result.data[row * 4 + col] = value;
        }
    }
    return result;
}
Matrix Matrix::operator%(const Matrix& m) {
    return Matrix();
}
float* Matrix::getData() {
    return this->data.data();
}
