#ifndef PHYSICS_H
#define PHYSICS_H

#include "Entities.h"

ent_world calculate_euler(ent_world world);
ent_world calculate_runge_kutta(ent_world world);
ent_world calculate_verlet(ent_world world);
ent_world calculate_adams_bashforth(ent_world world);
ent_world calculate_implicit_runge_kutta(ent_world world);

#endif //PHYSICS_H
