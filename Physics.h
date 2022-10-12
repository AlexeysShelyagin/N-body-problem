#ifndef PHYSICS_H
#define PHYSICS_H

#include "Entities.h"

ent_world calculate_euler(ent_world world);
ent_world calculate_rungekutte(ent_world world);
ent_world calculate_verlet(ent_world world);

#endif //PHYSICS_H
