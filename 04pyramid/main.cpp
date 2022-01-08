#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

#include "../common/sdl_app.h"
#include "../common/error_handler.h"
#include "../common/matrix.h"

const int numVAOs = 1; // Vertex Array object. Un arreglo de objetos. Creo que solo necesitamos uno.
const int numVBOs = 2; // Vertex Buffer object. Necesitamos uno por mesh diferente? (Y si tenemos miles de meshes?)

class PyramidApp : public SDLApp {
public:
    PyramidApp(int width, int height);
    /**
     * Creates the buffers with the cube and pyramid meshes
     */
    void setup_vertices();

    /**
     * Loads shader file and return a string representation of the entire file
     * @param filename
     * @return std::string with the full content of the shader file
     */
    std::string load_shader(const std::string &filename);
    GLuint create_shader_program();

    void init() override;
    void update(double delta_time) override;

private:
    GLuint rendering_program;
    GLuint vao[numVAOs];
    GLuint vbo[numVBOs];

    GLuint v_loc, proj_loc;

    Matrix p_mat, v_mat, m_mat;
};

PyramidApp::PyramidApp(int width, int height) : SDLApp(width, height) {
    init();
}

void PyramidApp::setup_vertices() {
    float cube_vertices[108] = {
            -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f,
    };

    float pyramid_vertices[] = {
            -1.0, -1.0, -1.0, 0.0, 1.0, 0.0, -1.0, -1.0, 1.0,
            -1.0, -1.0, 1.0, 0.0, 1.0, 0.0, 1.0, -1.0, 1.0,
            1.0, -1.0, 1.0, 0.0, 1.0, 0.0, 1.0, -1.0, -1.0,
            1.0, -1.0, -1.0, 0.0, 1.0, 0.0, -1.0, -1.0, -1.0,
            1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0,
            1.0, -1.0, 1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0
    };

    // Creamos el vertex array. Este siempre se necesita crear aunque no lo usemos
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    // Ahora creamos nuestros 2 buffers
    glGenBuffers(numVBOs, vbo);

    // Cargamos los vertices del cubo en el primer buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    // Ahora los vertices de la piramide
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);
}

std::string PyramidApp::load_shader(const std::string &filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        std::ostringstream contents;
        contents << in.rdbuf();
        in.close();
        return (contents.str());
    }
    throw (errno);
}

GLuint PyramidApp::create_shader_program() {
    GLint vertex_shader_compiled;
    GLint fragment_shader_compiled;
    GLint linked_program;

    auto vertex_shader_source = load_shader("vertex_shader.glsl");
    auto fragment_shader_source = load_shader("fragment_shader.glsl");

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    const char *vertex_shader_string = vertex_shader_source.c_str();
    const char *fragment_shader_string = fragment_shader_source.c_str();
    glShaderSource(vertex_shader, 1, &vertex_shader_string, NULL);
    glShaderSource(fragment_shader, 1, &fragment_shader_string, NULL);

    glCompileShader(vertex_shader);
    checkOpenGLError();
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_shader_compiled);
    if (vertex_shader_compiled != 1) {
        std::cout << "Vertex compilation failed\n";
        printShaderLog(vertex_shader);
    }

    glCompileShader(fragment_shader);
    checkOpenGLError();
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_shader_compiled);
    if (fragment_shader_compiled != 1) {
        std::cout << "Fragment compilation failed\n";
        printShaderLog(fragment_shader);
    }

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    checkOpenGLError();
    glGetProgramiv(shader_program, GL_LINK_STATUS, &linked_program);
    if (linked_program != 1) {
        std::cout << "Linking program failed\n";
        printProgramLog(shader_program);
    }

    return shader_program;
}

void PyramidApp::init() {
    rendering_program = create_shader_program();
    setup_vertices();
    // v_mat basicamente es la cámara principal, pero las coordenadas deben ir al revés
    v_mat = Matrix::make_translate(0.0, 0.0, -8.0);
    p_mat = Matrix::make_perspective(1.0472, width / height, 0.1f, 1000.0f);
}

void PyramidApp::update(double delta_time) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(rendering_program);

    // Movemos el cubo un poquito abajo
    m_mat = Matrix::make_translate(0.0, -2.0, 0.0);

    v_loc = glGetUniformLocation(rendering_program, "mv_matrix");
    glUniformMatrix4fv(v_loc, 1, GL_TRUE, (v_mat * m_mat).getData());

    proj_loc = glGetUniformLocation(rendering_program, "proj_matrix");
    glUniformMatrix4fv(proj_loc, 1, GL_TRUE, p_mat.getData());

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    // Creamos el atributo 0. En el shader dice location=0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    // Activamos el atributo 0. Por alguna razón hay que activarlo
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Y si no es instanced?
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Ahora va la piramide

    // Primero la ponemos en algun lugar de la escena:
    m_mat = Matrix::make_translate(2.0f, 2.0f, 0.0f);

    // Lo mandamos al shader
    glUniformMatrix4fv(v_loc, 1, GL_TRUE, (m_mat * v_mat).getData());
    glUniformMatrix4fv(proj_loc, 1, GL_TRUE, p_mat.getData());

    // Ahora si seleccionamos nuestro segundo buffer, donde esta la piramide
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 18);

}


int main() {
    auto *app = new PyramidApp(600, 600);
    app->run();
    return 0;
}