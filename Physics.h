#ifndef PHYSICS_H
#define PHYSICS_H

#include "Entities.h"

void recalculate_dt(ent_world &world);

ent_world calculate_euler(ent_world world);
ent_world calculate_runge_kutta(ent_world world);
ent_world calculate_verlet(ent_world world);
ent_world calculate_adams_bashforth(ent_world world);
ent_world calculate_implicit_runge_kutta(ent_world world);

class runge_kutta_values{
public:
    vec3 q = vec3(), k = vec3();
};

#endif //PHYSICS_H
