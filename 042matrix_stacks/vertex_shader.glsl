#version 410

layout (location=0) in vec3 position;

uniform mat4 model_matrix;
uniform mat4 proj_matrix;

out vec4 varying_color;

void main(void) {
    gl_Position = proj_matrix * model_matrix * vec4(position, 1.0);
    varying_color = vec4(position, 1.0)*0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}