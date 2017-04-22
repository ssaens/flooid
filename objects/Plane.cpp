#include "Plane.h"
#include "../utils.h"

void Plane::collide(Particle& p) {
	double pos_dot = glm::dot(p.pos, this->normal);
	double pred_dot = glm::dot(p.pred_pos, this->normal);
	if (std::signbit(pos_dot) != std::signbit(pred_dot)) {
		glm::dvec3 d = glm::normalize(p.pred_pos - p.pos);
		double t = glm::dot(this->point - p.pos, this->normal) / glm::dot(d, this->normal);
		glm::dvec3 tangent = p.pos + t * d;
		glm::dvec3 correction = (tangent - SURFACE_OFFSET * d) - p.pos;
		p.pred_pos = p.pos + correction * (1 - this->friction);

		p.vel = glm::reflect(p.vel, this->normal) * 0.9;
		if (glm::length(p.vel) < 0.1) {
			p.vel = glm::dvec3();
		}
	 }
}

void Plane::render() {
	glBegin(GL_QUADS); //Begin quadrilateral coordinates
	//Trapezoid
	glVertex3f(-1.f, -RADIUS, 1.f);
	glVertex3f(-1.f, -RADIUS, -1.f);
	glVertex3f(1.f, -RADIUS, -1.f);
	glVertex3f(1.f, -RADIUS, 1.f);
	glEnd(); //End quadrilateral coordinates
}