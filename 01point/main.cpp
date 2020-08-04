#include <GL/glew.h>
#include <SDL.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "../common/error_handler.h"
#include "../common/sdl_app.h"

const int WIDTH = 600;
const int HEIGHT = 600;

const int numVAOs = 1;


class CubeApp : public SDLApp {
public:
    CubeApp(int width, int height);
    std::string load_shader(const std::string &filename);
    GLuint createShaderProgram();

    void init() override;
    void update(double delta_time) override;

private:
    GLuint renderingProgram;
    GLuint vao[numVAOs];
};

CubeApp::CubeApp(int width, int height) : SDLApp(width, height) {
    init();
}

std::string CubeApp::load_shader(const std::string &filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        std::ostringstream contents;
        contents << in.rdbuf();
        in.close();
        return (contents.str());
    }
    throw(errno);
}

GLuint CubeApp::createShaderProgram() {
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

void CubeApp::init() {
    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

void CubeApp::update(double delta_time) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);
    glPointSize(30.0f);
    glDrawArrays(GL_POINTS, 0, 1);
}

int main() {
    auto *app = new CubeApp(WIDTH, HEIGHT);
    app->run();
    delete app;
    return 0;
}