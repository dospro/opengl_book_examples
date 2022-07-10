#version 410

layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoord;

out vec2 tc;

uniform mat4 view_matrix;
uniform mat4 proj_matrix;

uniform sampler2D samp;

void main(void) {
    gl_Position = proj_matrix * view_matrix * vec4(position, 1.0);
    tc = texCoord;
}