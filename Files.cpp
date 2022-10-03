#include "Files.h"

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

template < typename T >
void file::write(T text) {
    f.write((char*) &text, sizeof(text));
}

void file::write(ent_world world) {
    write(world.time);
    for(int i = 0; i < world.count(); i++){
        write(world.bodies[i].pos.x);
        write(world.bodies[i].pos.y);
        write(world.bodies[i].pos.z);
    }
}

double file::read_double() {
    char* buffer = new char[sizeof(double)];
    f.read(buffer, sizeof(double));
    return *(double*)buffer;
}

template void file::write <int> (int);
template void file::write <double> (double);
template void file::write <std::string> (std::string);
template void file::write <char const*> (char const*);