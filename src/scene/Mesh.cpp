//
// Created by Dillon Yao on 4/25/17.
//

#include "Mesh.h"
#include "../utils.h"

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    init();
}

void Mesh::init() {
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
                 &this->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
                 &this->indices[0], GL_STATIC_DRAW);

    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, n));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, tex));

    glBindVertexArray(0);
}

void Mesh::draw(Shader &shader) {
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for(GLuint i = 0; i < this->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
        // Retrieve texture number (the N in diffuse_textureN)
        stringstream ss;
        string number;
        string name = this->textures[i].type;
        if(name == "texture_diffuse")
            ss << diffuseNr++; // Transfer GLuint to stream
        else if(name == "texture_specular")
            ss << specularNr++; // Transfer GLuint to stream
        number = ss.str();

        glUniform1f(glGetUniformLocation(shader.PID, ("material." + name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // Draw mesh
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::draw() {
//    glUseProgram(0);
//    glBindVertexArray(this->VAO);
//    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
//    glBindVertexArray(0);

    for (auto i = this->indices.begin(); i != this->indices.end();) {
        dvec3 face[3];
        dvec3 normal;
        for (int v = 0; v < 3; ++v) {
            face[v] = this->vertices[*i].pos;
            normal += this->vertices[*i].n;
            ++i;
        }
        normal = normalize(normal);
        fvec3 color(0, 1, 0);
        draw_triangle(face[0], face[1], face[2], normal, color);
    }

//    draw_triangle(dvec3(-1, 0, 0),
//                dvec3(1, 0, 0),
//                dvec3(0, 1, 0),
//                fvec3(1, 0, 0));
//    draw_triangle(dvec3(-1, 0, 1),
//                  dvec3(1, 0, 1),
//                  dvec3(0, 1, 1),
//                  fvec3(0, 1, 0));
}