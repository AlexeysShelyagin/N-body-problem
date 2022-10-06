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

    if (ext == ".json") {
        cout << "loading SIMULATION_PATH + filename ...\n";
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

        time_t timer = time(0);
        tm* start = localtime(&timer);
        cout << "dt = " << world.dt << '\n';
        cout << "started at " << start -> tm_hour << "." << start -> tm_min << "." << start -> tm_sec << "\n";
        int ready_part, percent = 0;

        int iteration = 0;
        while (sf_window_opened()) {
            if(world.calc_method == "euler") world = calculate_euler(world);

            if (world.render) {
                sf_window_event(window, world);
                render_scene(window, world);
            }

            if (world.save){
                simulated.write(world);
                if(world.time >= world.end_time){
                    sf_close_window(window);
                }

                ready_part = world.time / world.end_time * 100;
                if(iteration % 100 == 0 || ready_part > percent){
                    percent = ready_part;
                    cout << "[";
                    for(int i = 0; i < percent / 10; i++) cout << '*';
                    for(int i = percent / 10; i < 10; i++) cout << '.';
                    cout << "] " << percent << "%    ";
                    cout << world.time << " of " << world.end_time;
                    cout << "\r";

                }
            }
            iteration++;
        }

        simulated.close();

        timer = time(0);
        tm* end = localtime(&timer) ;
        cout << "\nCalculation time: ";
        cout << end -> tm_hour << "h ";
        cout << end -> tm_min << "m ";
        cout << end -> tm_sec << "s ";
        cout << "\nCalculation time: ";
        cout << start -> tm_hour << "h ";
        cout << start -> tm_min << "m ";
        cout << start -> tm_sec << "s ";
    }
    if(ext == ".bin"){
        sf::RenderWindow window = sf_init_scene(1920, 1080, 1, "test", DEFAULT_SCENE_SCALE);

        Replay replay(SIMULATED_PATH + output_name);

        while (window.isOpen()){
            sf_window_event(window, replay.world);
            render_scene(window, replay.world, replay);

            if(replay.frame < replay.frame_num) replay.frame++;
            replay.load_frame(replay.frame);
        }
    }
    return 0;
}