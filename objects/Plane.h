#ifndef PLANE_H
#define PLANE_H

#include "../Particle.h"
#if defined(__APPLE_CC__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <math.h>
#endif

struct Plane {
	Plane(const glm::dvec3 point, const glm::dvec3 normal, double friction) :
		point(point), normal(normal), friction(friction) {}

	void collide(Particle& p);
	void render();

	glm::dvec3 point;
	glm::dvec3 normal;
	double friction;
};

#endif /* PLANE_H */