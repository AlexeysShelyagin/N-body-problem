#ifndef N_BODY_PROBLEM_CONSOLE_H
#define N_BODY_PROBLEM_CONSOLE_H

#include <iostream>
#include <ctime>

#include "Config.h"
#include "Entities.h"
#include "Files.h"

///--------------------Console interface class-----------------------
/*
 *  filename - name of file which program loads
 *
 *  output_name - name of simulated output binary file
 *
 *  dt - delta time, gets from console or world
 *
 *  end_time - end time of simulation, gets from world
 *
 *  render - render mode flag
 *      -1 - use default file mode
 *      0 - disable rendering
 *      1 - enable rendering
 *
 *  save - output mode flag
 *      -1 - use default file mode
 *      0 - don't save anything
 *      1 - save simulated file
 *
 *  ext - opened file extension
 */

class Console_launch{
public:
    std::string filename = DEFAULT_FILE;
    std::string output_name = DEFAULT_OUTPUT;
    double dt, end_time;
    int render, save, dynamic_dt;
    std::string method;
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
///-------------------------------------------------------------------

#endif //N_BODY_PROBLEM_CONSOLE_H
