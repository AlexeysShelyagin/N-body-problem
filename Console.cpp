#include "Console.h"

using namespace std;

///-------------------------Constructor------------------------------

Console_launch::Console_launch(int argc, char* argv[]) {
    read_parameters(argc, argv);
}

///------------------------------------------------------------------


///----------------------Reading arguments---------------------------
/*
 *  Function arguments:
 *      argc - number of console arguments
 *      argv - arguments list
 *
 *  Arguments definition:
 *      -n or -name + file name in default folder: name of simulation/replay file
 *      -o or -output + file name in default folder: name of simulated output binary
 *      -dt + real number: redefining simulation step (world.dt)
 *      -r or -render : flag for enable simulation rendering
 *      -d : flag for enable dynamic dt
 *      -m or -method + method name: name of calculating method
 */

void Console_launch::read_parameters(int argc, char* argv[]) {
    dt = -1;
    save = render = (argc <= 1) ? -1 : 0;
    dynamic_dt = 0;
    method = "";

    string arg;
    for(int i = 0; i < argc; i++){
        arg = argv[i];
        if( (arg == "-n") || (arg == "-name") ) filename = argv[++i];
        if( (arg == "-o") || (arg == "-output") ){
            output_name = argv[++i];
            save = 1;
        }
        if( (arg == "-dt") ) dt = stod(argv[++i]);
        if( (arg == "-r") || (arg == "-render") ) render = 1;
        if( (arg == "-d")) dynamic_dt = 1;
        if( (arg == "-m") || (arg == "-method") ) method = argv[++i];
    }
    ext = file_extension(filename);
}
///------------------------------------------------------------------


///--------------------File loading process--------------------------
/*
 *  Prints loading process of selected file
 */

void Console_launch::loading_file_start() {
    cout << "loading " << SIMULATION_PATH + filename << " ...\n";
}
///------------------------------------------------------------------


///-----------------------File load result----------------------------
/*
 *  Printing load result
 */

void Console_launch::load_result() {
    cout << "load successfully\n";
}
///------------------------------------------------------------------


///----------------------Applying parameters-------------------------
/*
 *  Function arguments:
 *      &world - world to apply startup parameters
 *
 *  Applies all given console arguments to calculating world
 */

void Console_launch::apply_parameters(ent_world &world) {
    if(dt != -1) world.dt = dt;
    if(render != -1) world.render = render;
    if(save != -1) world.save = save;
    if(!method.empty()) world.calc_method = method;
    world.dynamic_dt = (dynamic_dt || world.dynamic_dt);

    dt = world.dt;
    end_time = world.end_time;
}
///------------------------------------------------------------------


///------------------------Start info--------------------------------
/*
 *  Prints simulation dt and start time
 */

void Console_launch::calculation_start_info() {
    int now = (int) time(0);
    start_time = now;
    time_t timer = time(0);
    tm* start = localtime(&timer);
    cout << "dt = " << dt << '\n';
    cout << "started at " << start -> tm_hour << "." << start -> tm_min << "." << start -> tm_sec << "\n";
}
///------------------------------------------------------------------

///-----------------------Progress info------------------------------
/*
 *  Function arguments:
 *      now_time - simulation world current time
 *      rewrite - flag to rewrite line without printing delay condition
 *
 *  Prints progress bar, done percent, current world time and calculated remaining time
 */

void Console_launch::update_progress(double now_time, bool rewrite) {
    ready_part = now_time / end_time * 100;
    if(ready_part < 0 || ready_part > 100) return;
    int now = (int) time(0);
    int remaining_time = (now - start_time) * (end_time / now_time - 1);

    if (now > last_t || rewrite) {
        percent = ready_part;
        last_t = now;

        cout << "[";
        for (int i = 0; i < percent / 10; i++) cout << '*';
        for (int i = percent / 10; i < 10; i++) cout << '.';
        cout << "] " << percent << "%    ";
        cout << now_time << " of " << end_time;

        cout << "    time remaining: ";
        cout << remaining_time / 3600 << "h ";
        cout << remaining_time / 60 % 60 << "m ";
        cout << remaining_time % 60 << "s ";
        cout << "\r";
    }
}
///------------------------------------------------------------------

///-------------------------Done info--------------------------------
/*
 *  Prints overall computing time
 */

void Console_launch::calculation_end_info() {
    int now = (int) time(0);
    cout << "\nCalculating time: ";
    cout << (now - start_time) / 3600 << "h ";
    cout << (now - start_time) / 60 % 60 << "m ";
    cout << (now - start_time) % 60 << "s ";
}
///------------------------------------------------------------------