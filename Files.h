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

    template < typename T >
    void write(T text);
    void write(ent_world world);

    double read_double();
};

std::string file_extension(std::string name);

#endif //N_BODY_PROBLEM_FILES_H
