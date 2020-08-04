#include <GL/glew.h>
#include <SDL.h>

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../common/error_handler.h"
#include "../common/matrix.h"
#include "../common/sdl_app.h"

const int numVAOs = 1;
const int numVBOs = 2;

class CubesApp : public SDLApp {
public:
    CubesApp(int width, int height);
    void setup_vertices();
    std::string load_shader(const std::string &filename);
    GLuint createShaderProgram();

    void init() override;
    void update(double delta_time) override;

private:
    GLuint renderingProgram;
    GLuint vao[numVAOs];
    GLuint vbo[numVBOs];

    GLuint v_loc, proj_loc;
    float aspect;
    float counter;

    Matrix p_mat, v_mat, m_mat, mv_mat, t_mat, r_mat;
};

CubesApp::CubesApp(int width, int height) : SDLApp(width, height) {
    init();
}


void CubesApp::setup_vertices() {
    // clang-format off
    float vertex_positions[108] = {
            -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
            1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,
            1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f,
            1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f,
            1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,
    };
    // clang-format on
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
}

std::string CubesApp::load_shader(const std::string &filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        std::ostringstream contents;
        contents << in.rdbuf();
        in.close();
        return (contents.str());
    }
    throw(errno);
}


GLuint CubesApp::createShaderProgram() {
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;

    auto vertex_shader_source = load_shader("vertex_shader.glsl");
    auto fragment_shader_source = load_shader("fragment_shader.glsl");

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char *vshaderSource = vertex_shader_source.c_str();
    const char *fshaderSource = fragment_shader_source.c_str();
    glShaderSource(vShader, 1, &vshaderSource, NULL);
    glShaderSource(fShader, 1, &fshaderSource, NULL);

    glCompileShader(vShader);
    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != 1) {
        std::cout << "Vertex compilation failed" << std::endl;
        printShaderLog(vShader);
    }
    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != 1) {
        std::cout << "Vertex compilation failed" << std::endl;
        printShaderLog(fShader);
    }

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);

    checkOpenGLError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != 1) {
        std::cout << "Linking failed" << std::endl;
        printProgramLog(vfProgram);
    }
    return vfProgram;
}

void CubesApp::init() {
    renderingProgram = createShaderProgram();
    setup_vertices();

    v_mat = Matrix::make_translate(0.0, 0.0, -24.0);
    t_mat = Matrix::make_identity();
    r_mat = Matrix::make_identity();
    counter = 0.0;

    aspect = (float) width / (float) height;
    p_mat = Matrix::make_perspective(1.0472, aspect, 0.1f, 1000.0f);
}
void CubesApp::update(double delta_time) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(renderingProgram);

    v_loc = glGetUniformLocation(renderingProgram, "v_matrix");
    glUniformMatrix4fv(v_loc, 1, GL_TRUE, v_mat.getData());

    proj_loc = glGetUniformLocation(renderingProgram, "proj_matrix");
    glUniformMatrix4fv(proj_loc, 1, GL_TRUE, p_mat.getData());

    counter += delta_time;
    GLuint tf_loc = glGetUniformLocation(renderingProgram, "tf");
    glUniform1f(tf_loc, (float) counter);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100000);
}

int main() {
    auto *app = new CubesApp(600, 600);
    app->run();
    return 0;
}
