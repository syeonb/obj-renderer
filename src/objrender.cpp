#include <objrender.hpp>
#include <objloader.hpp>

void clearRender();

Renderer::Renderer(char *objPath) {
    bool res = loadOBJ(objPath, vertices, indices, texcoords);
    if (res == false) {
        printf("obj could not be processed properly\n");
    }
}

void Renderer::initiateRender() {
    initializeWindow();
    compileShaders();
    generateAndBindBuffer();
    glm::vec3 scale = glm::vec3(10.f, 10.f, 10.f);
    ModelMatrix = glm::scale(ModelMatrix, scale);
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        clearRender();
        processShaders();
        drawToFrameBuffer();
        // swapBuffer();
    }
    completeRender();
}

void Renderer::compileShaders() {// we need these to properly pass the strings
    const char *source;
    int length;

    // create and compiler vertex shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    source = vertex_source.c_str();
    length = vertex_source.size();
    glShaderSource(vertex_shader, 1, &source, &length);
    glCompileShader(vertex_shader);
    if(!check_shader_compile_status(vertex_shader)) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    // create and compiler fragment shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    source = fragment_source.c_str();
    length = fragment_source.size();
    glShaderSource(fragment_shader, 1, &source, &length);
    glCompileShader(fragment_shader);
    if(!check_shader_compile_status(fragment_shader)) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    // create program
    shader_program = glCreateProgram();

    // attach shaders
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    // link the program and check for errors
    glLinkProgram(shader_program);
    if(!check_program_link_status(shader_program)) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

void Renderer::initializeWindow() {
    glfwSetErrorCallback(error_callback);

    if(glfwInit() == GL_FALSE) {
        std::cerr << "failed to init GLFW" << std::endl;
    } else {
        printf("GLFW initialized\n");
    }
    // GLFW / OGL Initialization, needs to be set before glfwCreateWindow
    // Core Opengl 3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, 0);

    // create a window
    if((window = glfwCreateWindow(DISPLAY_WIDTH, DISPLAY_HEIGHT, "Teleview-UVRender", 0, 0)) == 0) {
        std::cerr << "failed to open window" << std::endl;
        glfwTerminate();
    } else {
        printf("opened window successfully\n");
    }
    glfwMakeContextCurrent(window);
    gl3w_init();
}

void Renderer::updateVertices(std::vector<GLfloat> newVertices) {
    clearRender();
    processShaders();
    substituteBufferData(newVertices);
    drawToFrameBuffer();
    swapBuffer();
    outputPixelValue();
    // TODO: RETURN THE LIST OF VERTEX COLOUR VALUES
}

void Renderer::substituteBufferData(std::vector<GLfloat> &newVertices) const {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), newVertices.data());
}

void Renderer::outputPixelValue() const {
    GLubyte data[DISPLAY_WIDTH * DISPLAY_HEIGHT * 3];
    printf("reading pixel data\n");
    glReadPixels(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, data);
    printf("pixel data read\n");
}

void Renderer::completeRender() {
    // delete the created objects
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &uv_vbo);
    glDetachShader(shader_program, vertex_shader);
    glDetachShader(shader_program, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteProgram(shader_program);
    GLenum error = glGetError();
    if(error == GL_NO_ERROR) {
        std::cout << "No error, closing window" << std::endl;
    } else {
        std::cerr << error << std::endl;
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Renderer::check_shader_compile_status(GLuint obj) {
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
bool Renderer::check_program_link_status(GLuint obj) {
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

void Renderer::generateAndBindBuffer() {
    // generate and bind the vao
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // generate and bind the index buffer
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        // fill with data
        // generate and bind the buffer object
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (char*)0 + 0 * sizeof(GLfloat));

        // generate and bind the uv coordinates
        glGenBuffers(1, &uv_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
        glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(GLfloat), texcoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (char*)0 + 0 * sizeof(GLfloat));
}

void Renderer::swapBuffer() {// finally swap buffers
    glfwSwapBuffers(window);
}

void Renderer::drawToFrameBuffer() {// draw
    glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);

    // check for errors
    GLenum error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cerr << error << std::endl;
    }
}

void Renderer::processShaders() {// use the shader program
    glUseProgram(shader_program);
    // blend colours
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    // add the uniforms 
    GLint MVP_loc = glGetUniformLocation(shader_program, "MVP");
    glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
}

void Renderer::clearRender() {
    glClearDepth(0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
