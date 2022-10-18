#ifndef N_BODY_PROBLEM_CONSOLE_H
#define N_BODY_PROBLEM_CONSOLE_H

#include <iostream>
#include <ctime>

#include "Config.h"
#include "Entities.h"
#include "Files.h"

class Console_launch{
public:
    std::string filename = DEFAULT_FILE;
    std::string output_name = DEFAULT_OUTPUT;
    double dt, end_time;
    int render, save;
    std::string ext;

    int start_time, ready_part, percent, last_t = 0;

    Console_launch(int argc, char* argv[]);

    void read_parameters(int argc, char* argv[]);

    void loading_file_start();
    void load_result();
    void apply_parameters(ent_world &world);

    void calculation_start_info();
    void update_progress(double now_time, bool rewrite = false);
    void calculation_end_info();
};

#endif //N_BODY_PROBLEM_CONSOLE_H
