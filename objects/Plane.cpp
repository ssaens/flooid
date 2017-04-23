#include "Plane.h"
#include "../utils.h"

void Plane::collide(Particle& p) {
	double pos_dot = glm::dot(p.pos, this->normal);
	glm::dvec3 final_pos = p.pred_pos + p.delta_pos;
	double pred_dot = glm::dot(final_pos, this->normal);
	if (std::signbit(pos_dot) != std::signbit(pred_dot)) {
		glm::dvec3 d = glm::normalize(final_pos - p.pos);
		double t = glm::dot(this->point - p.pos, this->normal) / glm::dot(d, this->normal);
		glm::dvec3 tangent = p.pos + t * d;
		glm::dvec3 correction = (tangent - SURFACE_OFFSET * d) - p.delta_pos;
		p.pred_pos = correction;

		p.vel = glm::reflect(p.vel, this->normal) * 0.9;
		// if (glm::length(p.vel) < 0.001) {
		// 	p.vel = glm::dvec3();
		// }
	 }
}

void Plane::render() {
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 10.0, 10.0, 10.0, 0.0 };
    glShadeModel(GL_SMOOTH);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glColor3f(0.5, 0.5, 0.5);
    glColorMaterial(GL_FRONT, GL_SPECULAR);
    glColor3f(0.9, 0.9, 0.9);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor3f(0.2, 0.5, 0.8);

	glBegin(GL_QUADS); //Begin quadrilateral coordinates
	//Trapezoid
	glVertex3f(-1.f, -RADIUS, 1.f);
	glVertex3f(-1.f, -RADIUS, -1.f);
	glVertex3f(1.f, -RADIUS, -1.f);
	glVertex3f(1.f, -RADIUS, 1.f);
	glEnd(); //End quadrilateral coordinates

	glPushMatrix();
    glTranslatef(this->point.x, this->point.y, this->point.z);
    glutSolidSphere(0.025, 10, 10);
    glPopMatrix();
}