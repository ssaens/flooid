//
// Created by Dillon Yao on 4/23/17.
//

#include "PBFRenderer.h"
#include "utils.h"

using namespace glm;

void PBFRenderer::render() {
//    scene.draw(shader);
    particles.step(0.001);
    particles.render();

//    glEnable(GL_POLYGON_OFFSET_FILL);
//    glPolygonOffset( 1.0, 1.0 );
//
//    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
//    glEnable(GL_COLOR_MATERIAL);
//
//    // Start specifying the polygon.
//    glBegin(GL_POLYGON);

//    // Set the normal of this face.
//    dvec3 normal(0, 0, 1);
//
//    glNormal3dv( &normal.x );
//
//    glNormal3dv( &normal.x );
//    // Draw this vertex.
//    dvec3 position(1, 0, 0);
//    glVertex3dv( &position.x );
//
//    glNormal3dv( &normal.x );
//    // Draw this vertex.
//    position = dvec3(-1, 0, 0);
//    glVertex3dv( &position.x );
//
//    glNormal3dv( &normal.x );
//    // Draw this vertex.
//    position = dvec3(0, 1, 0);
//    glVertex3dv( &position.x );
//
//    // Finish drawing the polygon.
//    glEnd();
}

void PBFRenderer::load_shader(ShaderInfo &info) {
//    shader = Shader(info);
}

void PBFRenderer::load_scene(string path) {
//    scene.load_scene(path);
}

