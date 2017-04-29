//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_MESH_H
#define FLOOID2_MESH_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <string>
#include "Shader.h"
#include <assimp/scene.h>

using namespace std;

struct Vertex {
    glm::vec3 pos;
    glm::vec3 n;
    glm::vec2 tex;
};

struct Texture {
    GLuint id;
    string type;
    aiString path;
};


class Mesh {
private:
    /*  Functions    */
    void init();

public:
    /*  Render data  */
    GLuint VAO, VBO, EBO;

    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    /*  Functions  */
    Mesh() {}
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);

    void render(Shader &shader);
};


#endif //FLOOID2_MESH_H