#include <cmath>

#include "Entities.h"
#include "Config.h"

//----------------PHYS_BODY---------------------------------
phys_body::phys_body(vec3 _pos, double _mass, bool _active, vec3 _vel, double _r, std::string _name) {
    pos = _pos;
    m = _mass;
    active = _active;
    r = _r;
    vel = _vel;
    name = _name;
}
//----------------------------------------------------------

//-------------------ENT_WORLD------------------------------
ent_world::ent_world(double _G, double _dt) {
    G = _G;
    dt = _dt;
    time = 0;
}

void ent_world::add_body(phys_body b) {
    bodies.push_back(b);
    last_pos.push_back(b.pos);
    last_vel.push_back(b.vel);

    buffer_pos.push_back(b.pos);
    buffer_vel.push_back(b.vel);
}

void ent_world::remove_body(int i){
    bodies[i].active = false;
    bodies[i].m = 0;
}

int ent_world::count() {
    return bodies.size();
}

int ent_world::count_active(int i_begin, int i_end) {
    int res = 0;
    for(int i = i_begin; i <= i_end; i++){
        res += (int) bodies[i].active;
    }
    return res;
}

int ent_world::find_body(std::string name) {
    for (int i = 0; i < this->bodies.size(); i++){
        if (this->bodies[i].name == name) return i;
    }
    return -1;
}

void ent_world::set_group(std::string name, int i_begin, int i_end) {
    body_groups[name] = std::make_pair(i_begin, i_end);
}

void ent_world::set_buffer() {
    for(int i = 0; i < count(); i++){
        buffer_pos[i] = bodies[i].pos;
        buffer_vel[i] = bodies[i].vel;
    }
}

void ent_world::load_buffer() {
    for(int i = 0; i < count(); i++){
        last_pos[i] = buffer_pos[i];
        last_vel[i] = buffer_vel[i];
    }
}

template < class T >
void ent_world::add_function(T func, int type_index) {
    functions.push_back(func);
    function_types.push_back(type_index);
}

template void ent_world::add_function < Lifetime_checker > (Lifetime_checker, int);
template void ent_world::add_function < Binary_star_checker > (Binary_star_checker, int);

template < class T >
T* ent_world::get_function(int i) {
    return &(std::get < T > (functions[i]));
}

template Lifetime_checker* ent_world::get_function < Lifetime_checker > (int);
template Binary_star_checker* ent_world::get_function < Binary_star_checker > (int);

int ent_world::func_count() {
    return functions.size();
}

double ent_world::full_energy(double accuracy){
    double eP = 0, eK = 0;
    for(int i = 0; i < count(); i++){
        if (!bodies[i].active) continue;

        phys_body body = bodies[i];
        eK += body.m * body.vel.mod() * body.vel.mod() / 2 / accuracy;

        for(int j = i + 1; j < count(); j++){
            if (!bodies[j].active) continue;

            vec3 r = (bodies[j].pos - body.pos);
            eP -= G * body.m * bodies[j].m / r.mod() / accuracy;
        }
    }
    energy = eK + eP;
    kinetic_e = eK;
    potential_e = eP;
    return energy;
}

double ent_world::avg_velocity(){
    double sum = 0;
    int unactive = 0;
    for(int i = 0; i < count(); i++){
        if (!bodies[i].active){
            unactive++;
            continue;
        }

        sum += bodies[i].vel.mod();
    }
    return sum / (count() - unactive);
}

vec2 ent_world::max_velocity(){
    double max_vel = 0, vel, max_i;
    for(int i = 0; i < count(); i++){
        if (!bodies[i].active) continue;

        vel = bodies[i].vel.mod();
        if(vel > max_vel){
            max_vel = vel;
            max_i = i;
        }
    }
    return vec2(max_vel, max_i);
}

vec2 ent_world::min_velocity(){
    double min_vel = VEL_MAX, vel, min_i;
    for(int i = 0; i < count(); i++){
        if (!bodies[i].active) continue;

        vel = bodies[i].vel.mod();
        if(vel < min_vel){
            min_vel = vel;
            min_i = i;
        }
    }
    return vec2(min_vel, min_i);
}

double ent_world::full_mass(){
    double full_m = 0;
    for(int i = 0; i < count(); i++){
        if (!bodies[i].active) continue;

        full_m += bodies[i].m;
    }
    return full_m;
}
///------------------------------------------------------------------


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
    return n;
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