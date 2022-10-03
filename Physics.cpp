#include <vector>
#include "Entities.h"

ent_world calculate_euler(ent_world world){
    std::vector < vec3 > accel(world.count());
    for (int i = 0; i < world.count(); i++){
        if (world.bodies[i].active){
            for (int j = 0; j < world.count(); j++){
                if (i != j){
                    vec3 r = world.bodies[j].pos - world.bodies[i].pos;
                    double r_3 = r.mod() * r.mod() * r.mod();
                    accel[i] += r * (world.G * world.bodies[j].m / r_3);
                }
            }
        }
    }

    for (int i = 0; i < world.count(); i++){
        world.bodies[i].vel += accel[i] * world.dt;
        world.bodies[i].pos += world.bodies[i].vel * world.dt;
    }

    world.time += world.dt;
    return world;
}