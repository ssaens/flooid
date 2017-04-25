//
// Created by Dillon Yao on 4/23/17.
//

#ifndef FLOOID_PARTICLEMANAGER_H
#define FLOOID_PARTICLEMANAGER_H

#include <eng/eng.h>
#include <glm/glm.hpp>

#include <vector>
#include <map>
#include "particle.h"
#include "PBDSolver.h"
#include "AppConfig.h"

using namespace glm;

class ParticleManager {
private:
    std::vector<Particle> particles;
    ivec3 bin(Particle &p);
    int hash_bin(glm::ivec3 pos);
    std::vector<Particle *> neighborhood(Particle& p);

    std::map<int, std::vector<Particle *> *> spacial_map;

    PBDSolver solver;

    int solver_iters;
    float particle_radius;
    float particle_mass;

public:
    ParticleManager(AppConfig &config);
    ~ParticleManager() { spacial_map.clear(); }
    void render() const;
    void step(float dt);
};


#endif //FLOOID_PARTICLEMANAGER_H
