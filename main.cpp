#include <GL/glew.h>
#include <SDL.h>
#include <fstream>
#include <iostream>

const int WIDTH = 600;
const int HEIGHT = 600;

const int numVAOs = 1;

GLuint renderingProgram;
GLuint vao[numVAOs];

std::string load_shader(const std::string &filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        return (std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()));
    }
    throw(errno);
}

void printShaderLog(GLuint shader) {
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char *) malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        std::cout << "Shader info Log:" << log << std::endl;
        free(log);
    }
}

void printProgramLog(int prog) {
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    std::cout << "Len: " << len << std::endl;
    if (len > 0) {
        log = (char *) malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        std::cout << "Program info Log:" << log << std::endl;
        free(log);
    }
}

bool checkOpenGLError() {
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        std::cout << "glError: " << glErr << std::endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

GLuint createShaderProgram() {
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

int main() {
    SDL_Window *window;
    SDL_GLContext gl_context;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return -1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow(
            "OpenGL Shaders",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WIDTH,
            HEIGHT,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    gl_context = SDL_GL_CreateContext(window);
    glewInit();

    std::cout << glGetString(GL_VERSION) << std::endl;

    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);

    bool isRunning = true;
    SDL_Event event;
    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;
            }
        }
        glClearColor(1.0, 1.0, 1.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(renderingProgram);
        glPointSize(30.0f);
        glDrawArrays(GL_POINTS, 0, 1);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
