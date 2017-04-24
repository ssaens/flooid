//
// Created by Dillon Yao on 4/23/17.
//

#include "utils.h"
#include <cmath>
#include <eng/eng.h>
#include <vector>

void draw_sphere(double radius, unsigned int lats, unsigned int longs) {
    float const R = 1. / (float) (lats - 1);
    float const S = 1. / (float) (longs - 1);
    int r, s;

    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;
    vertices.resize(lats * longs * 3);
    normals.resize(lats * longs * 3);
    texcoords.resize(lats * longs * 2);

    std::vector<GLfloat>::iterator v = vertices.begin();
    std::vector<GLfloat>::iterator n = normals.begin();
    std::vector<GLfloat>::iterator t = texcoords.begin();
    for (r = 0; r < lats; r++)
        for (s = 0; s < longs; s++) {
            float const y = sin(-M_PI_2 + M_PI * r * R);
            float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);
            *t++ = s * S;
            *t++ = r * R;
            *v++ = x * radius;
            *v++ = y * radius;
            *v++ = z * radius;
            *n++ = x;
            *n++ = y;
            *n++ = z;
        }

    indices.resize(lats * longs * 4);
    std::vector<GLushort>::iterator i = indices.begin();
    for (r = 0; r < lats - 1; r++)
        for (s = 0; s < longs - 1; s++) {
            *i++ = r * longs + s;
            *i++ = r * longs + (s + 1);
            *i++ = (r + 1) * longs + (s + 1);
            *i++ = (r + 1) * longs + s;
        }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    glNormalPointer(GL_FLOAT, 0, &normals[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
    glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
    glPopMatrix();
}
