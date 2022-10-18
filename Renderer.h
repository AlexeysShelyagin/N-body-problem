#ifndef RENDERER_H
#define RENDERER_H

#include "Replay.h"
#include "UI.h"

sf::RenderWindow sf_init_scene(int w, int h, int mode = 0, std::string title = "", double scale = 1);
bool sf_window_opened();
void sf_close_window(sf::RenderWindow& window);

Event_result sf_window_event(sf::RenderWindow &window, ent_world &world);

void render_scene(sf::RenderWindow& window, ent_world& world, std::string &filename, bool show = true);
void render_scene(sf::RenderWindow& window, ent_world& world, std::string &filename, Replay& replay);

#endif //RENDERER_H
