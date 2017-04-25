//
// Created by Dillon Yao on 4/23/17.
//

#include "utils.h"
#include <cmath>
#include <eng/eng.h>
#include <vector>

// Static data describing points on a sphere
#define SPHERE_NUM_LAT 10
#define SPHERE_NUM_LON 10

#define SPHERE_NUM_VERTICES ( ( SPHERE_NUM_LAT + 1 ) * ( SPHERE_NUM_LON + 1 ) )
#define SPHERE_NUM_INDICES ( 6 * SPHERE_NUM_LAT * SPHERE_NUM_LON )
#define SINDEX(x,y) ((x) * (SPHERE_NUM_LON + 1) + (y))
#define VERTEX_SIZE 8
#define TCOORD_OFFSET 0
#define NORMAL_OFFSET 2
#define VERTEX_OFFSET 5
#define BUMP_FACTOR 1

static unsigned int Indices[SPHERE_NUM_INDICES];
static double Vertices[VERTEX_SIZE * SPHERE_NUM_VERTICES];
static bool initialized = false;
static double vertices[3 * SPHERE_NUM_VERTICES];
static double normals[3 * SPHERE_NUM_VERTICES];

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

void init_mesh() {
    for ( int i = 0; i <= SPHERE_NUM_LAT; i++ ) {
        for ( int j = 0; j <= SPHERE_NUM_LON; j++ ) {
            double lat = ((double)i) / SPHERE_NUM_LAT;
            double lon = ((double)j) / SPHERE_NUM_LON;
            double *vptr = &Vertices[VERTEX_SIZE * SINDEX(i,j)];

            vptr[TCOORD_OFFSET + 0] = lon;
            vptr[TCOORD_OFFSET + 1] = 1-lat;

            lat *= M_PI;
            lon *= 2 * M_PI;
            double sinlat = sin( lat );

            vptr[NORMAL_OFFSET + 0] = vptr[VERTEX_OFFSET + 0] = sinlat * sin(lon);
            vptr[NORMAL_OFFSET + 1] = vptr[VERTEX_OFFSET + 1] = cos(lat),
                                      vptr[NORMAL_OFFSET + 2] = vptr[VERTEX_OFFSET + 2] = sinlat * cos(lon);
        }
    }

    for ( int i = 0; i < SPHERE_NUM_LAT; i++ ) {
        for ( int j = 0; j < SPHERE_NUM_LON; j++ ) {
            unsigned int* iptr = &Indices[6 * ( SPHERE_NUM_LON * i + j )];

            unsigned int i00 = SINDEX(i,  j  );
            unsigned int i10 = SINDEX(i+1,j  );
            unsigned int i11 = SINDEX(i+1,j+1);
            unsigned int i01 = SINDEX(i,  j+1);

            iptr[0] = i00;
            iptr[1] = i10;
            iptr[2] = i11;
            iptr[3] = i11;
            iptr[4] = i01;
            iptr[5] = i00;
        }
    }
}

void draw_sphere(const glm::vec3& p, double r) {
    if (!initialized) {
        init_mesh();
        initialized = true;
    }

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(p.x, p.y, p.z);
    glScaled(r, r, r);

    for (int i = 0; i < SPHERE_NUM_INDICES; i += 3) {
        glBegin(GL_POLYGON);
        double *vPtr1 = &Vertices[VERTEX_SIZE * Indices[i]];
        double *vPtr2 = &Vertices[VERTEX_SIZE * Indices[i + 1]];
        double *vPtr3 = &Vertices[VERTEX_SIZE * Indices[i + 2]];
        glm::dvec3 n1(vPtr1[NORMAL_OFFSET],
                      vPtr1[NORMAL_OFFSET + 1],
                      vPtr1[NORMAL_OFFSET + 2]);
        glm::dvec3 n2(vPtr2[NORMAL_OFFSET],
                      vPtr2[NORMAL_OFFSET + 1],
                      vPtr2[NORMAL_OFFSET + 2]);
        glm::dvec3 n3(vPtr3[NORMAL_OFFSET],
                      vPtr3[NORMAL_OFFSET + 1],
                      vPtr3[NORMAL_OFFSET + 2]);
        const glm::dvec3 &n = normalize(n1 + n2 + n3);
        glNormal3dv(&n1.x);
        glVertex3dv(vPtr1 + VERTEX_OFFSET);
        glNormal3dv(&n2.x);
        glVertex3dv(vPtr2 + VERTEX_OFFSET);
        glNormal3dv(&n3.x);
        glVertex3dv(vPtr3 + VERTEX_OFFSET);
        glEnd();
    }
    glPopMatrix();
}