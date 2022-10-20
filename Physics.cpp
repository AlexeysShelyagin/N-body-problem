#include "Entities.h"
#include <vector>
#include <iostream>

ent_world calculate_euler(ent_world world){
    std::vector < vec3 > accel(world.count());
    for (int i = 0; i < world.count(); i++){
        if (world.bodies[i].active){
            for (int j = 0; j < world.count(); j++){
                if (i != j){
                    vec3 r = world.bodies[j].pos - world.bodies[i].pos;
                    double r_3 = r.mod();
                    r_3 = r_3 * r_3 * r_3;
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

ent_world calculate_verlet(ent_world world) {
    std::vector<vec3> accel(world.count());
    for (int i = 0; i < world.count(); i++) {
        if (world.bodies[i].active) {
            for (int j = 0; j < world.count(); j++) {
                if (i != j) {
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

vec3 runge_kutta_function(ent_world& world, vec3 r, double m){
    double r_3 = r.mod();
    r_3 = r_3 * r_3 * r_3;
    return -r * (world.G * m / r_3);
}

ent_world calculate_runge_kutta(ent_world world){
    std::vector < vec3 > vel_list(world.count());
    std::vector < vec3 > pos_list(world.count());
    for (int i = 0; i < world.count(); i++){
        if (world.bodies[i].active) {
            for (int j = 0; j < world.count(); j++) {
                if (i != j) {
                    vec3 r = world.bodies[j].pos - world.bodies[i].pos;
                    vec3 vel = world.bodies[i].vel;
                    double m = world.bodies[i].m;

                    vec3 k1 = runge_kutta_function(world, r, m);
                    vec3 q1 = vel * world.dt;
                    vec3 k2 = runge_kutta_function(world, r + q1 / 2, m);
                    vec3 q2 = (vel + k1 / 2) * world.dt;
                    vec3 k3 = runge_kutta_function(world, r + q2 / 2, m);
                    vec3 q3 = (vel + k2 / 2) * world.dt;
                    vec3 k4 = runge_kutta_function(world, r + q3, m);
                    vec3 q4 = (vel + k3) * world.dt;

                    vel_list[i] += (k1 + k2 * 2 + k3 * 2 + k4) / 6.0;
                    pos_list[i] += (q1 + q2 * 2 + q3 * 2 + q4) / 6.0;
                }
            }
        }
    }

    for (int i = 0; i < world.count(); i++){
        world.bodies[i].vel += vel_list[i];
        world.bodies[i].pos += pos_list[i];
    }

    world.time += world.dt;
    return world;
}