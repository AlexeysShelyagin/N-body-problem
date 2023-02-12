#include "Analysis.h"

#include <iostream>
#include <cmath>

#include "Entities.h"
#include "Config.h"

///------------------------Lifetime checker--------------------------

Lifetime_checker::Lifetime_checker(ent_world& world_ptr, int i_begin, int i_end) {
    world = &world_ptr;
    group.beg = i_begin;
    group.end = i_end;
}

void Lifetime_checker::check_scattering() {
    double threshold_vel = world -> avg_velocity() * SCATTER_VELOCITY_FACTOR;
    for (int i = group.beg; i <= group.end; i++){
        if (world -> bodies[i].vel.mod() > threshold_vel){
            scatter_possible[i]++;
        }
    }
    for (auto i : scatter_possible){
        if (i.second >= SCATTER_ACCEPT_STEPS){
            world -> remove_body(i.first);
            scatter_possible.erase(i.first);
            continue;
        }
        if (i.second <= SCATTER_WAIT_STEPS) continue;

        double last_vel = world -> last_vel[i.first].mod();
        double vel = world -> bodies[i.first].vel.mod();
        if (std::abs(vel - last_vel) > SCATTER_VELOCITY_DEVIATION * vel){
            scatter_possible.erase(i.first);
        }
    }
}

///------------------------------------------------------------------

///----------------------Binary star checker-------------------------

Binary_star_checker::Binary_star_checker(ent_world &world_ptr, int i_begin, int i_end) {
    world = &world_ptr;
    group.beg = i_begin;
    group.end = i_end;
}