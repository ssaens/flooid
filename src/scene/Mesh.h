//
// Created by Dillon Yao on 4/25/17.
//

#ifndef FLOOID_MESH_H
#define FLOOID_MESH_H

#include <glm/glm.hpp>
#include <eng/eng.h>
#include <vector>
#include <assimp/types.h>
#include "../shader/Shader.h"

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
/*  Render data  */
    GLuint VAO, VBO, EBO;
    /*  Functions    */
    void init();

public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    /*  Functions  */
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
    void draw(Shader &shader);
};


#endif //FLOOID_MESH_H
