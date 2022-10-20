#ifndef ENTITIES_H
#define ENTITIES_H

#include <string>
#include <vector>
#include <cmath>

#include "Vectors.h"

static double PI = acos(-1);

class phys_body{
public:
    vec3 pos, vel;
    double m, r;
    bool active;
    std::string name;

    phys_body(vec3 _pos = vec3(), double _mass = 1, bool _active = 1, vec3 _vel = vec3(), double _r = 1, std::string _name = "");

};

class ent_world{
public:
    std::vector < phys_body > bodies;
    std::vector < vec3 > last_pos;
    double G, dt, time, end_time;
    bool render, save;
    std::string calc_method;

    ent_world(double _G = 0, double _dt = 0);

    void add_body(phys_body b);
    int count();
    int find_body(std::string name);

    double full_energy(double accuracy = 1);
};

class ent_camera{
public:
    vec3 pos;

    virtual ~ent_camera() = default;
protected:
    vec3 n, s;
    vec2 orient;
};

class ent_ortho_camera: public ent_camera{
public:
    double scale;

    ent_ortho_camera(vec3 pos = vec3(), vec2 orientation = vec2(), double _scale = 1);
    vec3 set_orientation(vec2 orientation);
    vec3 change_orientation(vec2 delta_orientation);
    vec2 get_orientation();
    vec3 get_direction();
    vec2 point_coords(vec3 point_pos);
};
#endif //ENTITIES_H
