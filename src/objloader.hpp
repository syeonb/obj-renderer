#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <gl3w.h>
#include <GLFW/glfw3.h>

// Matrix/vector classes in a format that we can pass to a shader
#include <glm/ext/vector_int2.hpp>  // ivec2
#include <glm/mat3x3.hpp>           // mat3
#include <glm/mat4x4.hpp>           // mat4
#include <glm/vec2.hpp>             // vec2
#include <glm/vec3.hpp>             // vec3
#include <glm/gtc/type_ptr.hpp>

// stlib
#include <chrono>
#include <iostream>
#include <vector>

bool loadOBJ(
        const char * path,
        std::vector<GLfloat> & vertices,
        std::vector<GLuint> & indices,
        std::vector<GLfloat> & texcoords
        );

#endif