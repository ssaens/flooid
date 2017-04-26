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

struct ShaderInfo {
    std::string vertex_shader;
    std::string frag_shader;
};

struct Shader {
    GLuint PID;

    Shader() {}
    Shader(const ShaderInfo& info);

    void use();
};

#endif //FLOOID_SHADER_H
