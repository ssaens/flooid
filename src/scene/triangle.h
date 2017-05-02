#ifndef FLOOID2_TRIANGLE_H
#define FLOOID2_TRIANGLE_H

#include <glm/glm.hpp>

struct Triangle {
	glm::vec3 v1, v2, v3;
	glm::vec3 n;
};

#endif