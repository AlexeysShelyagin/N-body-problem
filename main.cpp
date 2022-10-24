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
#include "Console.h"

using namespace std;

int main(int argc, char* argv[]) {

    Console_launch console(argc, argv);

    Event_result event;

    if (console.ext == ".json") {
        console.loading_file_start();
        ent_world world = load_simulation(SIMULATION_PATH + console.filename);
        console.load_result();

        console.apply_parameters(world);

        sf::RenderWindow window = sf_init_scene(1920, 1080, 0, "test", DEFAULT_SCENE_SCALE);
        if(!world.render) window.close();

        file simulated;
        if(world.save) simulated.open(OUTPUT_PATH + console.output_name, "wb");
        simulated.write(world.G);
        simulated.write((double) world.count());

        console.calculation_start_info();

        int iteration = 0;
        while (sf_window_opened()) {
            if(world.calc_method == "euler") world = calculate_euler(world);
            if(world.calc_method == "runge_kutta") world = calculate_runge_kutta(world);
            if(world.calc_method == "verlet"){
                if (iteration == 0)
                    world = calculate_euler(world);
                else
                    world = calculate_verlet(world);
            }
            if(world.calc_method == "adams_bashforth"){
                if (iteration == 0)
                    world = calculate_euler(world);
                else
                    world = calculate_adams_bashforth(world);
            }
            if(world.calc_method == "implicit_runge_kutta"){
                if (iteration == 0)
                    world = calculate_euler(world);
                else
                    world = calculate_implicit_runge_kutta(world);
            }

            if(world.dynamic_dt) recalculate_dt(world);

            if (world.render) {
                event = sf_window_event(window, world);
                event.change_simulation(world);
                render_scene(window, world, console.filename);
            }

            if (world.save){
                simulated.write(world);
                if (!world.render) {
                    if (world.time >= world.end_time) {
                        sf_close_window(window);
                    }

                    console.update_progress(world.time);
                }
            }
            iteration++;
        }

        simulated.close();

        console.update_progress(world.time, true);
        console.calculation_end_info();
    }
    if(console.ext == ".bin"){
        sf::RenderWindow window = sf_init_scene(1920, 1080, 1, "test", DEFAULT_SCENE_SCALE);

        Replay replay(SIMULATED_PATH + console.filename);

        while (window.isOpen()){
            event = sf_window_event(window, replay.world);
            event.change_replay(replay);
            render_scene(window, replay.world, console.filename, replay);

            replay.next_frame();
        }
    }
    return 0;
}