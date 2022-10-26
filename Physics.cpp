#include "Physics.h"
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
        world.bodies[i].pos +=  world.bodies[i].vel * world.dt * (3.0 / 2) - world.last_vel[i] * world.dt * (1.0 / 2);
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

vec3 runge_kutta_function(double G, vec3 r, double m){
    double r_3 = r.mod();
    r_3 = r_3 * r_3 * r_3;
    return r * (G * m / r_3);
}

std::vector < runge_kutta_values > runge_kutta_iteration(ent_world& world, std::vector < runge_kutta_values > last_iter_val = std::vector < runge_kutta_values > (), double k = 1){
    std::vector < runge_kutta_values > values(world.count());
    if (last_iter_val.empty()) last_iter_val.resize(world.count());

    for(int i = 0; i < world.count(); i++){
        vec3 accel = vec3();
        for(int j = 0; j < world.count(); j++){
            if(i != j){
                vec3 r = (world.bodies[j].pos + last_iter_val[j].q * k) - (world.bodies[i].pos + last_iter_val[i].q * k);
                accel += runge_kutta_function(world.G, r, world.bodies[j].m);
            }
        }
        values[i].k = accel * world.dt;
        values[i].q = (world.bodies[i].vel + last_iter_val[i].k * k) * world.dt;
    }

    return values;
}

ent_world calculate_runge_kutta(ent_world world){
    std::vector < runge_kutta_values > iter1;
    std::vector < runge_kutta_values > iter2;
    std::vector < runge_kutta_values > iter3;
    std::vector < runge_kutta_values > iter4;

    iter1 = runge_kutta_iteration(world);
    iter2 = runge_kutta_iteration(world, iter1, 0.5);
    iter3 = runge_kutta_iteration(world, iter2, 0.5);
    iter4 = runge_kutta_iteration(world, iter3);

    for(int i = 0; i < world.count(); i++){
        world.bodies[i].pos += (iter1[i].q + iter2[i].q * 2 + iter3[i].q * 2 + iter4[i].q) / 6.0;
        world.bodies[i].vel += (iter1[i].k + iter2[i].k * 2 + iter3[i].k * 2 + iter4[i].k) / 6.0;
    }

    world.time += world.dt;
    return world;
}