#include <fstream>
#include <sstream>
#include <iostream>
#include <stack>

#include <GL/glew.h>

#include "../common/sdl_app.h"
#include "../common/error_handler.h"
#include "../common/matrix.h"

const int numVAOs = 1; // Vertex Array object. Un arreglo de objetos. Creo que solo necesitamos uno.
const int numVBOs = 2; // Vertex Buffer object. Necesitamos uno por mesh diferente? (Y si tenemos miles de meshes?)

class MatrixStacks : public SDLApp {
public:
    MatrixStacks(int width, int height);
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

    GLuint proj_loc;

    Matrix p_mat, v_mat;
    std::stack<Matrix> m_stack;
    double counter;
};

MatrixStacks::MatrixStacks(int width, int height) : SDLApp(width, height), m_stack{}, counter{} {
    init();
}

void MatrixStacks::setup_vertices() {
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

std::string MatrixStacks::load_shader(const std::string &filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        std::ostringstream contents;
        contents << in.rdbuf();
        in.close();
        return (contents.str());
    }
    throw (errno);
}

GLuint MatrixStacks::create_shader_program() {
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

void MatrixStacks::init() {
    rendering_program = create_shader_program();
    setup_vertices();
    p_mat = Matrix::make_perspective(1.0472, width / height, 0.1f, 1000.0f);
}

void MatrixStacks::update(double delta_time) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(rendering_program);

    // Ahora empecewmos mandando la matriz de proyeccion primero
    proj_loc = glGetUniformLocation(rendering_program, "proj_matrix");
    glUniformMatrix4fv(proj_loc, 1, GL_TRUE, p_mat.getData());

    // Primero vamos a guardar la matriz del view
    v_mat = Matrix::make_translate(0.0f, 0.0f, -16.0f);
    m_stack.push(v_mat);

    // Ahora vamos a dibujar a la piramide como centro
    // Primero, copiamos el view, y luego le agregamos la transformación
    m_stack.push(m_stack.top() * Matrix::make_translate(0.0f, 0.0f, 0.0f));

    // Ahora giramos el centro
    m_stack.push(m_stack.top() * Matrix::make_rotate_y(counter));

    // Mandamos la matriz al shader
    GLint loc_model_matrix = glGetUniformLocation(rendering_program, "model_matrix");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_TRUE, m_stack.top().getData());
    // Y dibujamos la piramide
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 18);
    // Como no queremos propagar la rotación, la sacamos del stack:
    m_stack.pop();

    // Ahora vamos a dibujar el primer planeta/cubo

    // En realidad este translate es el que hace la traslación (rotación al rededor del sol)
    // El valor dentro de sin/cos indica que tan rapido hace la traslación
    // La amplitud (el 5.0) nos indica que tan lejos se está del centro
    m_stack.push(m_stack.top() * Matrix::make_translate(sin(2.0 * counter) * 5.0, 0.0, cos(2.0 * counter) * 5.0));
    m_stack.push(m_stack.top() * Matrix::make_rotate_y(5.0f * counter));

    // De nuevo, mandamos al shader y luego dibujamos
    glUniformMatrix4fv(loc_model_matrix, 1, GL_TRUE, m_stack.top().getData());
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // Ahora es el cubo
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    m_stack.pop();

    // Y finalmente la luna (otro cubo) que tambien vamos a hacer mas chiquito
    m_stack.push(m_stack.top()
                 * Matrix::make_translate(0.0f, sin(5 * counter) * 3.0, cos(5 * counter) * 3.0)
                 * Matrix::make_rotate_z(4.0 * counter)
                 * Matrix::make_scale(0.25f, 0.25f, 0.25f)
    );
    glUniformMatrix4fv(loc_model_matrix, 1, GL_TRUE, m_stack.top().getData());
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // Ahora es el cubo
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //m_stack.pop(); // Sacamos el translate de la luna
    m_stack.pop(); // Sacamos el translate del planeta
    m_stack.pop(); // Sacamos el translate del sol
    m_stack.pop(); // Sacamos el view
    counter += 0.01;

}


int main() {
    auto *app = new MatrixStacks(600, 600);
    app->run();
    return 0;
}