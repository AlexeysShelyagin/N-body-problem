#ifndef N_BODY_PROBLEM_FILES_H
#define N_BODY_PROBLEM_FILES_H

#include <fstream>

#include "Entities.h"

class file{
    std::fstream f;
public:
    std::string name;

    file(std::string _name = "", std::string mode = "r");

    void open(std::string _name, std::string mode = "r");
    void close();
    unsigned long long size();

    template < typename T >
    void write(T text);
    void write(ent_world world);

    void set_cur(unsigned long long cur);

    double read_double(long long cur = -1);
    vec3 read_vec3(long long cur = -1);
    ent_world read_world(int body_num, long long cur = -1);
};

std::string file_extension(std::string name);

#endif //N_BODY_PROBLEM_FILES_H
