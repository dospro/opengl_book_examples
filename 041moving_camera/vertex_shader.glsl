#version 410

layout (location=0) in vec3 position;

uniform mat4 model_matrix;
uniform mat4 proj_matrix;
uniform float x_rot;
uniform float y_rot;

out vec4 varying_color;

mat4 build_translate(float x, float y, float z)
{
    mat4 trans = mat4(
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    x, y, z, 1.0
    );
    return trans;
}

mat4 build_rotate_x(float rad)
{
    mat4 xrot = mat4(
    1.0, 0.0, 0.0, 0.0,
    0.0, cos(rad), -sin(rad), 0.0,
    0.0, sin(rad), cos(rad), 0.0,
    0.0, 0.0, 0.0, 1.0
    );
    return xrot;
}

mat4 build_rotate_y(float rad)
{
    mat4 yrot = mat4(
    cos(rad), 0.0, sin(rad), 0.0,
    0.0, 1.0, 0.0, 0.0,
    -sin(rad), 0.0, cos(rad), 0.0,
    0.0, 0.0, 0.0, 1.0
    );
    return yrot;
}

mat4 build_rotate_z(float rad)
{
    mat4 zrot = mat4(
    cos(rad), -sin(rad), 0.0, 0.0,
    sin(rad), cos(rad), 0.0, 0.0,
    0.0, 0.0, -1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
    );
    return zrot;
}

mat4 build_scale(float x, float y, float z)
{
    mat4 scale = mat4(
    x, 0.0, 0.0, 0.0,
    0.0, y, 0.0, 0.0,
    0.0, 0.0, z, 0.0,
    0.0, 0.0, 0.0, 1.0
    );
    return scale;
}

void main(void) {
    mat4 local_rot_x = build_rotate_x(x_rot);
    mat4 local_rot_y = build_rotate_y(y_rot);
    mat4 view_matrix = build_translate(0.0, 0.0, -8.0) * local_rot_x * local_rot_y;

    mat4 mv_matrix = view_matrix * model_matrix;

    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
    varying_color = vec4(position, 1.0)*0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}