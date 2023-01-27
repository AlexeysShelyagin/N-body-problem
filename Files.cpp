#include "Files.h"

#include <iostream>

file::file(std::string _name, std::string mode) {
    open(_name, mode);
}

void file::open(std::string _name, std::string mode) {
    name = _name;
    if (mode == "rb") f.open(name, std::ios::in | std::ios::binary);
    if (mode == "wb") f.open(name, std::ios::out | std::ios::binary);
}

void file::close() {
    f.close();
}

unsigned long long file::size() {
    long long pos = f.tellg();
    f.seekg(0, std::ios::end);
    long long size = f.tellg();
    f.seekg(pos);
    return size;
}

template < typename T >
void file::write(T text) {
    f.write((char*) &text, sizeof(text));
}

void file::write(ent_world world) {
    write(world.time);
    write(world.potential_e);
    write(world.kinetic_e);
    for(int i = 0; i < world.count(); i++){
        write(world.bodies[i].pos.x);
        write(world.bodies[i].pos.y);
        write(world.bodies[i].pos.z);
        write(world.bodies[i].m);
    }
}

template void file::write <int> (int);
template void file::write <double> (double);
template void file::write <std::string> (std::string);
template void file::write <char const*> (char const*);

void file::set_cur(unsigned long long cur) {
    f.seekg(cur, std::ios::beg);
}

double file::read_double(long long cur) {
    if(cur != -1) set_cur(cur);

    char* buffer = new char[sizeof(double)];
    f.read(buffer, sizeof(double));
    return *(double*)buffer;
}

vec3 file::read_vec3(long long cur){
    if(cur != -1) set_cur(cur);

    double x, y, z;
    x = read_double();
    y = read_double();
    z = read_double();
    return vec3(x, y, z);
}

ent_world file::read_world(int body_num, long long cur) {
    if(cur != -1) set_cur(cur);

    ent_world world;
    world.time = read_double();
    world.potential_e = read_double();
    world.kinetic_e = read_double();

    vec3 pos;
    double m;
    for(int i = 0; i < body_num; i++){
        pos = read_vec3();
        m = read_double();
        world.add_body(phys_body(pos, m));
    }

    return world;
}

std::string file_extension(std::string name){
    int i = name.length() - 1;
    while(name[i] != '.') i--;
    std::string ext;
    for(;i < name.length(); i++) ext += name[i];
    return ext;
}