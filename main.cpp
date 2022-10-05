#include <iostream>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include <windows.h>

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

    string arg;
    for(int i = 0; i < argc; i++){
        arg = argv[i];
        if( (arg == "-n") || (arg == "-name") ) filename = argv[++i];
        if( (arg == "-o") || (arg == "-output") ) output_name = argv[++i];
        if( (arg == "-dt") ) dt = stod(argv[++i]);
    }
    string ext = file_extension(filename);

    if (ext == ".json") {
        ent_world world = load_simulation(SIMULATION_PATH + filename);
        if(dt != -1) world.dt = dt;

        sf::RenderWindow window = sf_init_scene(1920, 1080, 0, "test", DEFAULT_SCENE_SCALE);

        file simulated;
        if(world.save) simulated.open(OUTPUT_PATH + output_name, "wb");
        simulated.write(world.G);
        simulated.write((double) world.count());

        while (world.save && world.time < world.end_time && window.isOpen()) {
            if(world.calc_method == "euler") world = calculate_euler(world);

            if (world.render) {
                sf_window_event(window, world);
                render_scene(window, world);
            }

            if (world.save) simulated.write(world);
        }

        simulated.close();
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