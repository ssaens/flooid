#ifndef FLOOID2_TRIANGLE_H
#define FLOOID2_TRIANGLE_H

#include <glm/glm.hpp>

using namespace glm;

struct Triangle {
	vec3 v1, v2, v3;
	vec3 n;
};

#endif