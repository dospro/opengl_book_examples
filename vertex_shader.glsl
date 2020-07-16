#version 410

layout (location=0) in vec3 position;

uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform float tf;

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
    float i = gl_InstanceID + tf;
    float a = sin(203.0 * i / 8000.0)*403.0;
    float b = sin(301.0 * i / 4004.0)*401.0;
    float c = sin(400.0 * i / 6003.0)*405.0;

    mat4 local_rot_x = build_rotate_x(i);
    mat4 local_rot_y = build_rotate_y(i);
    mat4 local_rot_z = build_rotate_z(i);
    mat4 local_trans = build_translate(a, b, c);

    mat4 new_m = local_trans *local_rot_x * local_rot_y * local_rot_z;
    mat4 mv_matrix = v_matrix * new_m;

    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
    varying_color = vec4(position, 1.0)*0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}