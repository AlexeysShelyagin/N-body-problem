#include <cmath>

#include "Entities.h"
#include "iostream"

//----------------PHYS_BODY---------------------------------
phys_body::phys_body(vec3 _pos, double _mass, bool _active, vec3 _vel, double _r, std::string _name) {
    this->pos = _pos;
    this->m = _mass;
    this->active = _active;
    this->r = _r;
    this->vel = _vel;
    this->name = _name;
}
//----------------------------------------------------------

//-------------------ENT_WORLD------------------------------
ent_world::ent_world(double _G, double _dt) {
    this->G = _G;
    this->dt = _dt;
    this->time = 0;
}

void ent_world::add_body(phys_body b) {
    this->bodies.push_back(b);
    last_pos.push_back(b.pos);
}

int ent_world::count() {
    return this->bodies.size();
}

int ent_world::find_body(std::string name) {
    for (int i = 0; i < this->bodies.size(); i++){
        if (this->bodies[i].name == name) return i;
    }
    return -1;
}
<<<<<<< Updated upstream
//-------------------------------------------------------------
=======

double ent_world::full_energy(double accuracy){
    double energy = 0;
    for(int i = 0; i < count(); i++){
        phys_body body = bodies[i];
        energy += body.m * body.vel.mod() * body.vel.mod() / 2 / accuracy;
        /*
        for(int j = 0; j < count(); j++){
            if(i != j){
                vec3 r = (bodies[j].pos - body.pos);
                energy += G * body.m * bodies[j].m / r.mod();
            }
        }*/
    }
    return energy;
}
///------------------------------------------------------------------
>>>>>>> Stashed changes

//------------------ENT_ORTHO_CAMERA---------------------------

vec3 ent_ortho_camera::set_orientation(vec2 orientation) {
    orient = orientation;
    n = ang_vec3(orient.x, orient.y);
    s = cross(n, vec3(0, 0, 1)).normalize();
    return n;
}

vec3 ent_ortho_camera::change_orientation(vec2 delta_orientation) {
    if (orient.y + delta_orientation.y > 0 && orient.y + delta_orientation.y < PI)
        return set_orientation(orient + delta_orientation);
}

ent_ortho_camera::ent_ortho_camera(vec3 _pos, vec2 orientation, double _scale) {
    pos = _pos;
    scale = _scale;

    orient = orientation;
    set_orientation(orient);
}

vec2 ent_ortho_camera::get_orientation() {
    return orient;
}

vec3 ent_ortho_camera::get_direction() {
    return n;
}

vec2 ent_ortho_camera::point_coords(vec3 point_pos) {
    vec3 rAB = point_pos - pos;
    vec3 k = cross(rAB, n);

    return vec2(
        dot(cross(k, n), s),
        dot(k, s)
    );
}
//-------------------------------------------------------------