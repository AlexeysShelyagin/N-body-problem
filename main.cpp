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

using namespace std;

int main() {

    sf::RenderWindow window = sf_init_scene(1920, 1080, "test", 1);

    ent_world world = load_simulation("../Simulations/test6.json");

    file simulated("../Simulated/test.bin", "wb");

    while (world.save && world.time < world.end_time && window.isOpen()){
        world = calculate_euler(world);

        if(world.render) {
            sf_window_event(window, world);
            render_scene(window, world);
        }

        if(world.save) simulated.write(world);
    }

    simulated.close();
    return 0;
}