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
    int i = 0;
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
