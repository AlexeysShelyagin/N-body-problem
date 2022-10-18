#include <iostream>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <ctime>

#include "Vectors.h"
#include "Entities.h"
#include "Loader.h"
#include "Renderer.h"
#include "Physics.h"
#include "Files.h"
#include "Config.h"
#include "Replay.h"

using namespace std;

int main(int argc, char* argv[]) {

    string filename = DEFAULT_FILE;
    string output_name = DEFAULT_OUTPUT;
    double dt = -1;
    int render = (argc <= 1) ? -1 : 0;
    int save = (argc <= 1) ? -1 : 0;

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
    string ext = file_extension(filename);

    Event_result event;

    if (ext == ".json") {
        cout << "loading " << SIMULATION_PATH + filename << " ...\n";
        ent_world world = load_simulation(SIMULATION_PATH + filename);
        cout << "load successfully\n";

        if(dt != -1) world.dt = dt;
        if(render != -1) world.render = render;
        if(save != -1) world.save = save;

        sf::RenderWindow window = sf_init_scene(1920, 1080, 0, "test", DEFAULT_SCENE_SCALE);

        file simulated;
        if(world.save) simulated.open(OUTPUT_PATH + output_name, "wb");
        simulated.write(world.G);
        simulated.write((double) world.count());

        int now = (int) time(0), st = now;
        time_t timer = time(0);
        tm* start = localtime(&timer);
        cout << "dt = " << world.dt << '\n';
        cout << "started at " << start -> tm_hour << "." << start -> tm_min << "." << start -> tm_sec << "\n";
        int ready_part, percent = 0, last_t = 0;

        int iteration = 0;
        while (sf_window_opened()) {
            if(world.calc_method == "euler") world = calculate_euler(world);
            if(world.calc_method == "rungekutte") world = calculate_rungekutte(world);
            if(world.calc_method == "verlet"){
                if (iteration == 0)
                    world = calculate_euler(world);
                else
                    world = calculate_verlet(world);
            }
            if (world.render) {
                event = sf_window_event(window, world);
                event.change_simulation(world);
                render_scene(window, world, filename);
            }

            if (world.save){
                simulated.write(world);
                if (!world.render) {
                    if (world.time >= world.end_time) {
                        sf_close_window(window);
                    }

                    ready_part = world.time / world.end_time * 100;
                    now = time(0);
                    int remaining_time = (now - st) * (world.end_time / world.time - 1);

                    if (last_t != remaining_time) {
                        percent = ready_part;
                        last_t = remaining_time;

                        cout << "[";
                        for (int i = 0; i < percent / 10; i++) cout << '*';
                        for (int i = percent / 10; i < 10; i++) cout << '.';
                        cout << "] " << percent << "%    ";
                        cout << world.time << " of " << world.end_time;

                        cout << "    time remaining: ";
                        cout << remaining_time / 3600 << "h ";
                        cout << remaining_time / 60 % 60 << "m ";
                        cout << remaining_time % 60 << "s ";
                        cout << "\r";
                    }
                }
            }
            iteration++;
        }

        simulated.close();

        now = time(0);
        cout << "\nCalculating time: ";
        cout << (now - st) / 3600 << "h ";
        cout << (now - st) / 60 % 60 << "m ";
        cout << (now - st) % 60 << "s ";
    }
    if(ext == ".bin"){
        sf::RenderWindow window = sf_init_scene(1920, 1080, 1, "test", DEFAULT_SCENE_SCALE);

        Replay replay(SIMULATED_PATH + filename);

        while (window.isOpen()){
            event = sf_window_event(window, replay.world);
            event.change_replay(replay);
            render_scene(window, replay.world, filename, replay);

            replay.next_frame();
        }
    }
    return 0;
}