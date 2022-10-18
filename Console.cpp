#include "Console.h"

using namespace std;

Console_launch::Console_launch(int argc, char* argv[]) {
    read_parameters(argc, argv);
}

void Console_launch::read_parameters(int argc, char* argv[]) {
    dt = -1;
    save = render = (argc <= 1) ? -1 : 0;

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
    }
    ext = file_extension(filename);
}

void Console_launch::loading_file_start() {
    cout << "loading " << SIMULATION_PATH + filename << " ...\n";
}

void Console_launch::load_result() {
    cout << "load successfully\n";
}

void Console_launch::apply_parameters(ent_world &world) {
    if(dt != -1) world.dt = dt;
    if(render != -1) world.render = render;
    if(save != -1) world.save = save;

    dt = world.dt;
    end_time = world.end_time;
}

void Console_launch::calculation_start_info() {
    int now = (int) time(0);
    start_time = now;
    time_t timer = time(0);
    tm* start = localtime(&timer);
    cout << "dt = " << dt << '\n';
    cout << "started at " << start -> tm_hour << "." << start -> tm_min << "." << start -> tm_sec << "\n";
}

void Console_launch::update_progress(double now_time, bool rewrite) {
    ready_part = now_time / end_time * 100;
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

void Console_launch::calculation_end_info() {
    int now = (int) time(0);
    cout << "\nCalculating time: ";
    cout << (now - start_time) / 3600 << "h ";
    cout << (now - start_time) / 60 % 60 << "m ";
    cout << (now - start_time) % 60 << "s ";
}