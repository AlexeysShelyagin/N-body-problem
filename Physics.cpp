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
vec3 accelCalc(ent_world& world, vec3 x, int i){
    vec3 accel;
            for (int j = 0; j < world.count(); j++){
                if (i != j){
                    vec3 r = world.bodies[j].pos - x;
                    double r_3 = r.mod() * r.mod() * r.mod();
                    accel += r * (world.G * world.bodies[j].m / r_3);
                }
            }

    return accel;
}
ent_world calculate_rungekutte(ent_world world){
    std::vector < vec3 > accel(world.count());
    for (int i = 0; i < world.count(); i++){
        vec3 k1 = (accelCalc(world, vec3(), i)) * world.dt;
        vec3 q1 = world.bodies[i-1].vel * world.dt;
        vec3 k2 = (accelCalc(world, q1/2, i)) * world.dt;
        vec3 q2 = (world.bodies[i-1].vel + k1 / 2) * world.dt;
        vec3 k3 = (accelCalc(world, q2/2, i)) * world.dt;
        vec3 q3 = (world.bodies[i-1].vel + k2 / 2) * world.dt;
        vec3 k4 = (accelCalc(world, q3, i)) * world.dt;
        vec3 q4 = (world.bodies[i-1].vel + k3) * world.dt;
        world.bodies[i].vel +=(k1 + k2 * 2.0 + k3 * 2.0 + k4) * (1 / 6);
        world.bodies[i].pos +=(q1 + q2 * 2.0 + q3 * 2.0 + q4) * (1 / 6);
    }

    world.time += world.dt;
    return world;
}