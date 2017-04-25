//
// Created by Dillon Yao on 4/25/17.
//

#ifndef FLOOID_MESH_H
#define FLOOID_MESH_H

#include <glm/vec3.hpp>
#include <eng/eng.h>
#include <vector>
#include "../shader/Shader.h"

using namespace std;

struct Vertex {
    glm::vec3 pos;
    glm::vec3 n;
    glm::vec3 tex;
};

struct Texture {
    GLuint id;
    string type;
};

class Mesh {
private:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    /*  Functions  */
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
    void draw(Shader shader);
public:
/*  Render data  */
    GLuint VAO, VBO, EBO;
    /*  Functions    */
    void init();
};


#endif //FLOOID_MESH_H
