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
vec3 accelCalc(ent_world& world, vec3 r, double m){
    vec3 accel;
    double r_3 = r.mod() * r.mod() * r.mod();
    accel += r * (world.G * m / r_3);

    return accel;
}
//ent_world calculate_runge_kutta(ent_world world){
//    for (int i = 0; i < world.count(); i++){
//        if (world.bodies[i].active) {
//            for (int j = 0; j < world.count(); j++) {
//                if (i != j) {
//                    vec3 r = world.bodies[i].pos - world.bodies[j].pos;
//                    vec3 k1 = accelCalc(world, r, world.bodies[j].m) * world.dt;
//                    vec3 q1 = world.bodies[i - 1].vel * world.dt;
//                    vec3 k2 = (accelCalc(world, r + q1 / 2, world.bodies[j].m)) * world.dt;
//                    vec3 q2 = (world.bodies[i - 1].vel + k1 / 2) * world.dt;
//                    vec3 k3 = (accelCalc(world, r + q2 / 2, world.bodies[j].m)) * world.dt;
//                    vec3 q3 = (world.bodies[i - 1].vel + k2 / 2) * world.dt;
//                    vec3 k4 = (accelCalc(world, r + q3, world.bodies[j].m)) * world.dt;
//                    vec3 q4 = (world.bodies[i - 1].vel + k3) * world.dt;
//                    world.bodies[i].vel += (k1 + k2 * 2.0 + k3 * 2.0 + k4) * (1.0 / 6);
//                    world.bodies[i].pos += (q1 + q2 * 2.0 + q3 * 2.0 + q4) * (1.0 / 6);
//                }
//            }
//        }
//    }
//
//    world.time += world.dt;
//    return world;
//}

ent_world calculate_verlet(ent_world world){
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
        vec3 temp_pos = world.bodies[i].pos;
        world.bodies[i].vel += accel[i] * world.dt;
        world.bodies[i].pos = world.bodies[i].pos * 2 - world.last_pos[i] + accel[i] * world.dt * world.dt;
        world.last_pos[i] = temp_pos;
    }

    world.time += world.dt;
    return world;
}
ent_world calculate_adams_bashforth(ent_world world){
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
        vec3 temp_pos = world.bodies[i].pos;
        vec3 temp_vel = world.bodies[i].vel;
        world.bodies[i].vel += accel[i] * world.dt;
        world.bodies[i].pos +=  world.bodies[i].vel * world.dt * (3.0 / 2)  - world.last_vel[i] * world.dt * (1.0 / 2);
        world.last_pos[i] = temp_pos;
        world.last_vel[i] = temp_vel;
    }

    world.time += world.dt;
    return world;
}
ent_world calculate_implicit_runge_kutta(ent_world world){
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
        vec3 temp_pos = world.bodies[i].pos;
        vec3 temp_vel = world.bodies[i].vel;
        world.bodies[i].vel += accel[i] * world.dt;
        world.bodies[i].pos += (world.bodies[i].vel + world.last_vel[i]) * world.dt * (1.0 / 2);
        world.last_pos[i] = temp_pos;
        world.last_vel[i] = temp_vel;
    }

    world.time += world.dt;
    return world;
}
ent_world calculate_runge_kutta(ent_world world){
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
        vec3 k1 = world.bodies[i].vel * world.dt;
        vec3 k2 = (world.bodies[i].vel + k1 * (1.0 / 2))* world.dt ;
        vec3 k3 = (world.bodies[i].vel + k2  * (1.0 / 2)) * world.dt;
        vec3 k4 = (world.bodies[i].vel + k3) * world.dt;
        world.bodies[i].pos += (k1 + k2 * 2.0 + k3 * 2.0 + k4) * (1.0 / 6);

    }

    world.time += world.dt;
    return world;
}