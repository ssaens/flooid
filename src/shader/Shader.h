//
// Created by Dillon Yao on 4/25/17.
//

#ifndef FLOOID_SHADER_H
#define FLOOID_SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

struct Shader {
    GLuint PID;

    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

    void use();
};

#endif //FLOOID_SHADER_H
