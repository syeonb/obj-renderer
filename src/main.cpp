// stlib
#include <iostream>
#include <vector>
static const int DISPLAY_WIDTH = 512;
static const int DISPLAY_HEIGHT = 512;
#define GL3W_IMPLEMENTATION
#include <gl3w.h>
#include <GLFW/glfw3.h>

#include "objloader.hpp"
#include "objrender.hpp"

bool spiked = false;

bool check_shader_compile_status(GLuint obj) {
    GLint status;
    glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        GLint length;
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetShaderInfoLog(obj, length, &length, &log[0]);
        std::cerr << &log[0];
        return false;
    }
    return true;
}

// helper to check and display for shader linker error
bool check_program_link_status(GLuint obj) {
    GLint status;
    glGetProgramiv(obj, GL_LINK_STATUS, &status);
    if(status == GL_FALSE) {
        GLint length;
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetProgramInfoLog(obj, length, &length, &log[0]);
        std::cerr << &log[0];
        return false;
    }
    return true;
}

static void error_callback(int error, const char *msg) {
    std::cerr << "GLWT error " << error << ": " << msg << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_J && action == GLFW_PRESS) {
        spiked = true;
        std::cout << "spike" << std::endl;
    } else if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        spiked = false;
        std::cout << "normal" << std::endl;
    }
}

int main() {
    // Modify obj path
     Renderer * r = new Renderer("");
     r->initiateRender();
    return 0;
}