#ifndef ENTITIES_H
#define ENTITIES_H

#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <variant>

#include "Vectors.h"
#include "Analysis.h"

static double PI = acos(-1);

///------------------------Physical body class-----------------------
/*
 *  Class contains:
 *      all physical body properies
 *
 *  pos - body position vector
 *
 *  vel - velocity vector
 *
 *  m - body mass
 *
 *  r - radius of the body
 *
 *  active - flag, if it's false the postition of body is fixed
 *
 *  name - unique body name
 */

class phys_body{
public:
    vec3 pos, vel;
    double m, r;
    bool active;
    std::string name;

    phys_body(vec3 _pos = vec3(), double _mass = 1, bool _active = 1, vec3 _vel = vec3(), double _r = 1, std::string _name = "");

};
///------------------------------------------------------------------

///--------------------Simulation world class------------------------
/*
 *  Class contains:
 *      all bodies to calculate
 *      simulation functions
 *      simulation properties
 *  All simulations and renderings works only with the whole world
 *
 *  bodies - array of bodies to calculate interraction
 *
 *  last_pos - last position of each body in bodies
 *
 *  G - gravity constant
 *
 *  dt - calculation step
 *
 *  time - current world time in seconds from the beginning
 *
 *  end_time - calculation end time in seconds
 *
 *  render - UI rendering flag: true - enabled, false - disabled
 *
 *  save - saving simulated world flag
 *
 *  calc_method - name of used calculation method
 */

class ent_world{
    std::vector < std::variant < Lifetime_checker, Binary_star_checker > > functions;

    std::vector < vec3 > buffer_pos;
    std::vector < vec3 > buffer_vel;
public:
    std::vector < phys_body > bodies;
    std::map < std::string, std::pair < int, int > > body_groups;
    std::vector < vec3 > last_pos;
    std::vector < vec3 > last_vel;

    std::vector < int > function_types;

    double G, dt, time, end_time, dt_max;
    bool render, save, dynamic_dt;
    std::string calc_method;
    double energy = 0, kinetic_e = 0, potential_e = 0, last_dE = 0;

    ent_world(double _G = 0, double _dt = 0);

    void add_body(phys_body b);
    void remove_body(int i);
    int count();
    int count_active(int i_begin, int i_end);
    int find_body(std::string name);
    void set_group(std::string name, int i_begin, int i_end);

    void set_buffer();
    void load_buffer();

    template < class T >
        void add_function(T func, int type_index);
    template < class T >
        T* get_function(int i);
    int func_count();

    double full_energy(double accuracy = 1);
    double avg_velocity();
    vec2 max_velocity();
    vec2 min_velocity();

    double full_mass();
};

///------------------------------------------------------------------


///-----------------------Camera base class--------------------------
/*
 *  Defines properties for all types of cameras
 *
 *  pos - camera position
 *
 *  n - normal to camera view surface (it's direction)
 *
 *  s - horizontal camera view direction (always perpendicular to n)
 *
 *  orient - orientation of the camera by two angles in radians:
 *      orient.x - incline in horizontal surface from Ox
 *      orient.y - incline in vertical surface from Oz
 */

class ent_camera{
public:
    vec3 pos;

    virtual ~ent_camera() = default;
protected:
    vec3 n, s;
    vec2 orient;
};
///------------------------------------------------------------------


///-----------------------Orthographic camera------------------------
/*
 *  Orthographic camera, based on camera class and renders scene in orthographic
 *
 *  scale - projection scale value
 */

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
///------------------------------------------------------------------

#endif //ENTITIES_H
