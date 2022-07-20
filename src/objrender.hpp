#include <gl3w.h>
#include <GLFW/glfw3.h>

// stlib
#include <chrono>
#include <iostream>
#include <vector>

// Matrix/vector classes in a format that we can pass to a shader
#include <glm/ext/vector_int2.hpp>  // ivec2
#include <glm/mat3x3.hpp>           // mat3
#include <glm/mat4x4.hpp>           // mat4
#include <glm/vec2.hpp>             // vec2
#include <glm/vec3.hpp>             // vec3
#include <glm/gtc/type_ptr.hpp>

class Renderer {
public:
    Renderer(char * objPath);
    void initiateRender();
    void updateVertices(std::vector<GLfloat> newVertices);
    void completeRender();

private:
    static const int DISPLAY_WIDTH = 512;
    static const int DISPLAY_HEIGHT = 512;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::vector<GLfloat> texcoords;
    // vao and vbo handle
    GLuint vao, vbo, ibo;
    GLuint uv_vbo;
    GLuint shader_program, vertex_shader, fragment_shader;
    GLFWwindow* window;
    glm::mat4 ModelMatrix = glm::mat4(1.0);
    bool check_shader_compile_status(GLuint obj);
    void generateAndBindBuffer();
    void swapBuffer();
    void drawToFrameBuffer();
    void processShaders();
    void clearRender();

    void outputPixelValue() const;

    void substituteBufferData(std::vector<GLfloat> &newVertices) const;
    static void error_callback(int error, const char *msg) {
        std::cerr << "GLWT error " << error << ": " << msg << std::endl;
    }
    bool check_program_link_status(GLuint obj);
    const std::string vertex_source =
            "#version 330\n"
            "layout(location = 0) in vec3 vposition;\n"
            "layout(location = 1) in vec2 texCoords;\n"
            "uniform mat4 MVP;\n"
            "out vec2 uv;\n"
            "void main() {\n"
            "   gl_Position = MVP * vec4(vposition, 1.0);\n"
            "   uv = texCoords;\n"
            "}\n";

    const std::string fragment_source =
            "#version 330\n"
            "uniform mat4 MVP;\n"
            "in vec2 uv;\n"
            "layout(location = 0) out vec4 FragColor;\n"
            "void main() {\n"
            "   FragColor = vec4(uv.x, uv.y, 0.0, 1.0);\n"
            "}\n";

    void initializeWindow();
    void compileShaders();
};