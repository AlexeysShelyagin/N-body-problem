#ifndef RENDERER_H
#define RENDERER_H

#include "Replay.h"

sf::RenderWindow sf_init_scene(int w, int h, int mode = 0, std::string title = "", double scale = 1);

void sf_window_event(sf::RenderWindow &window, ent_world &world);

void render_scene(sf::RenderWindow& window, ent_world& world, bool show = true);
void render_scene(sf::RenderWindow& window, ent_world& world, Replay& replay);

#endif //RENDERER_H
