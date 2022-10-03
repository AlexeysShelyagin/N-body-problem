#ifndef RENDERER_H
#define RENDERER_H

sf::RenderWindow sf_init_scene(int w, int h, std::string title = "", double scale = 1);

void sf_window_event(sf::RenderWindow &window, ent_world &world);

void render_scene(sf::RenderWindow& window, ent_world world);

#endif //RENDERER_H
