//
// Created by Dillon Yao on 4/29/17.
//

#include <soil/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Skybox.h"

Skybox::Skybox() {}

Skybox::~Skybox() {}

void Skybox::init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
    glBindVertexArray(0);

    shader.load("src/shaders/skybox.vert", "src/shaders/skybox.frag");
}

void Skybox::load_cube_map(std::vector<std::string> faces) {
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);

    int w, h;
    unsigned char *image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for (GLuint i = 0; i < faces.size(); ++i) {
        image = SOIL_load_image(faces[i].c_str(), &w, &h, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                     GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    init();
}

void Skybox::render(glm::mat4 &view, glm::mat4 &projection) {
    shader.use();
    glm::mat4 stripped_view = glm::mat4(glm::mat3(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(stripped_view));
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader.program, "skybox"), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}