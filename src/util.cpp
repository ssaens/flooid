//
// Created by Dillon Yao on 4/28/17.
//

#include "util.h"

Mesh generate_sphere_mesh(float radius, unsigned int lats, unsigned int longs) {
    float const R = 1.f / (lats - 1);
    float const S = 1.f / (longs - 1);

    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<GLuint> indices;
    for (int r = 0; r < lats; ++r) {
        for (int s = 0; s < longs; ++s) {
            float theta = M_PI * r * R;
            float phi = 2 * M_PI * s * S;
            float y = sin(M_PI_2 + theta);
            float x = cos(phi) * sin(theta);
            float z = sin(phi) * sin(theta);
            Vertex v;
            v.n = glm::vec3(x, y, z);
            v.pos = radius * v.n;
            v.tex = glm::vec2(s * S, r * R);
            vertices.push_back(v);
        }
    }

    for (int r = 0; r < lats - 1; ++r) {
        for (int s = 0; s < longs; ++s) {
            int next_s = (s + 1) % longs;
            int next_r = (r + 1) % lats;
            indices.push_back(r * longs + s);            //  1 --- 2   5
            indices.push_back(r * longs + next_s);       //        |   |
            indices.push_back(next_r * longs + next_s);  //        3   7 --- 6
            indices.push_back(r * longs + s);
            indices.push_back(next_r * longs + next_s);
            indices.push_back(next_r * longs + s);
        }
    }

    return Mesh(vertices, indices, textures);
}

Mesh generate_cube_mesh(float side) {
    glm::vec3 points[8] = {
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.f, 0.f, 1.f),
        glm::vec3(0.f, 1.f, 0.f),
        glm::vec3(0.f, 1.f, 1.f),
        glm::vec3(1.f, 0.f, 0.f),
        glm::vec3(1.f, 0.f, 1.f),
        glm::vec3(1.f, 1.f, 0.f),
        glm::vec3(1.f, 1.f, 1.f),
    };

    std::vector<Vertex> vertices;
    std::vector<Texture> textures;

    for (int i = 0; i < 8; i++) {
        Vertex v;
        v.pos = points[i] * (side / 2);
        v.n = glm::normalize(points[i]);
        v.tex = glm::vec2(i / 4, i % 4);
        vertices.push_back(v);
    }

    std::vector<GLuint> indices = {
            0, 4, 6,
            0, 6, 2,
            4, 5, 7,
            4, 7, 6,
            5, 3, 7,
            5, 1, 3,
            1, 0, 2,
            1, 2, 3,
            3, 2, 6,
            3, 6, 7,
            1, 4, 0,
            1, 5, 4,
    };
    return Mesh(vertices, indices, textures);
}