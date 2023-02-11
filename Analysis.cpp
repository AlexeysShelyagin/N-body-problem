#include "Analysis.h"

#include <iostream>
#include "Entities.h"

///------------------------Lifetime checker--------------------------

Lifetime_checker::Lifetime_checker(ent_world& world_ptr, int i_begin, int i_end) {
    world = &world_ptr;
    group.beg = i_begin;
    group.end = i_end;
}

void Lifetime_checker::check() {
    for (int i = group.beg; i <= group.end; i++){
        std::cout << world -> bodies[i].m << ' ';
    }
    std::cout << "\n\n";
}

void Lifetime_checker::check_scattering() {

}

void Lifetime_checker::remove_scattered() {

}

///------------------------------------------------------------------

///----------------------Binary star checker-------------------------

Binary_star_checker::Binary_star_checker(ent_world &world_ptr, int i_begin, int i_end) {
    world = &world_ptr;
    group.beg = i_begin;
    group.end = i_end;
}