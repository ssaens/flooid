#ifndef FLOOID2_TRIANGLE_H
#define FLOOID2_TRIANGLE_H

#include <glm/glm.hpp>
#include "../PBD/Particle.h"
#include "../common.h"

using namespace glm;

class Triangle {
private:
	vec3 v1, v2, v3;
	vec3 n;

public:
	Triangle(vec3 v1, vec3 v2, vec3 v3) :
		v1(v1), v2(v2), v3(v3)
	{
		n = normalize(cross((v2- v1), (v3 - v1)));
	}

	void collide(Particle &p) {
		vec3 d = p.pred_p - p.p;
		vec3 e1 = v2 - v1;
		vec3 e2 = v3 - v1;
		vec3 s = p.p - v1;
		vec3 s1 = cross(d, e2);
		vec3 s2 = cross(s, e1);
		float coeff = 1 / dot(s2, e1);
		
		// Check if on opposite sides of plane
		float pos_dot = glm::dot(p.p - this->v1, this->n);
    	float pred_dot = glm::dot(p.pred_p - this->v1, this->n);
    	if (std::signbit(pos_dot) == std::signbit(pred_dot)) {
    		return;
    	}

    	// Check if inside triangle
		float b1 = coeff * dot(s1, s);
		float b2 = coeff * dot(s2, d);
		if (b1 >= 0 && b2 >= 0 && (1 - b1 - b2) >= 0) {
			vec3 tangent = b1 * v2 + b2 * v3 + (1 - b1 - b2) * v1;
    		if (std::signbit(pred_dot) > 0) {
    			tangent -= n * SURFACE_OFFSET;
    		} else {
    			tangent += n * SURFACE_OFFSET;
    		}
    		p.pred_p = tangent;
    	}
	}
};

#endif