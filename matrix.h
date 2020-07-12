#ifndef OPENGL_SHADERS_MATRIX_H
#define OPENGL_SHADERS_MATRIX_H
#include <array>

class Matrix {
public:
    static Matrix make_identity();
    static Matrix make_translate(float x, float y, float z);
    static Matrix make_rotate_x(float rad);
    static Matrix make_rotate_y(float rad);
    static Matrix make_rotate_z(float rad);
    static Matrix make_scale(float x, float y, float z);
    static Matrix make_perspective(float fov, float aspect, float near, float far);

    Matrix operator+(const Matrix &m);
    Matrix operator-(const Matrix &m);
    Matrix operator*(const Matrix &m);
    Matrix operator%(const Matrix &m);

    float *getData();

private:
    std::array<float, 16> data;
};


#endif//OPENGL_SHADERS_MATRIX_H
