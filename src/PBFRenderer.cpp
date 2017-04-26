//
// Created by Dillon Yao on 4/23/17.
//

#include "PBFRenderer.h"
#include "utils.h"

using namespace glm;

void draw_triangle(dvec3 p1, dvec3 p2, dvec3 p3, fvec3 color) {
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset( 1.0, 1.0 );

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(color.r, color.g, color.b);

    glEnable(GL_COLOR_MATERIAL);

    // Start specifying the polygon.
    glBegin(GL_POLYGON);

    // Set the normal of this face.
    dvec3 normal(0, 0, 1);

    glNormal3dv( &normal.x );

    glNormal3dv( &normal.x );
    // Draw this vertex.
    glVertex3dv( &p1.x );

    glNormal3dv( &normal.x );
    // Draw this vertex.
    glVertex3dv( &p2.x );

    glNormal3dv( &normal.x );
    // Draw this vertex.
    glVertex3dv( &p3.x );

    // Finish drawing the polygon.
    glEnd();
}

void PBFRenderer::render() {
//    scene.draw(shader);
    particles.step(0.0083);
    particles.render();

//    draw_triangle(dvec3(-1, 0, 0),
//                    dvec3(1, 0, 0),
//                    dvec3(0, 1, 0),
//                    fvec3(1, 0, 0));
//    draw_triangle(dvec3(-1, 0, 1),
//                  dvec3(1, 0, 1),
//                  dvec3(0, 1, 1),
//                  fvec3(0, 1, 0));
}

void PBFRenderer::load_shader(ShaderInfo &info) {
//    shader = Shader(info);
}

void PBFRenderer::load_scene(string path) {
//    scene.load_scene(path);
}

