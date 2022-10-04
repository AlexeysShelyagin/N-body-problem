#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

#include "Entities.h"
#include "Config.h"
#include "UI.h"

using namespace sf;

Font font;

base_view top(0);
base_view front(1);
base_view side(2);

window_surface screen;
window_surface surf_top;
window_surface surf_front;
window_surface surf_side;
window_surface surf_3d;

ent_ortho_camera camera_3d(vec3(), vec2(PI / 4, PI / 4), 0.1);

vec2 pressed_pos;
int mouse_window;

int frame_count = 0, last, fps = 0;
time_t current;

RenderWindow sf_init_scene(int w, int h, std::string title, double scale){
    font.loadFromFile("../Fonts/consolas.ttf");

    screen = window_surface(w, h);
    surf_top = window_surface(w / 2, h / 2, w / 2, 0);
    surf_front = window_surface(w / 2, h / 2,0, h / 2);
    surf_side = window_surface(w / 2, h / 2, w / 2, h / 2);
    surf_3d =  window_surface(w / 2, h / 2, 0, 0);

    camera_3d.scale = scale;
    top.scale = scale;
    front.scale = scale;
    side.scale = scale;

    return RenderWindow(VideoMode(w, h), title);
}

void sf_window_event(RenderWindow& window, ent_world& world){
    vec2 mouse_pos(Mouse::getPosition(window).x, Mouse::getPosition(window).y);

    Event event;
    while (window.pollEvent(event)){
        if (event.type == Event::Closed) window.close();
        if (event.type == Event::MouseWheelScrolled){
            if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel)
                if(surf_top.inside(mouse_pos) || surf_front.inside(mouse_pos) || surf_side.inside(mouse_pos)){
                    double scale = top.scale * (1 + event.mouseWheelScroll.delta);
                    top.scale = front.scale = side.scale = scale;
                }
                if(surf_3d.inside(mouse_pos)) camera_3d.scale *= 1 + event.mouseWheelScroll.delta;
            if (event.mouseWheelScroll.wheel == Mouse::HorizontalWheel)
                world.dt *= 1 + event.mouseWheelScroll.delta;
        }
        if (event.type == Event::MouseButtonPressed){
            pressed_pos = mouse_pos;

            if(surf_top.inside(mouse_pos)) mouse_window = 1;
            if(surf_front.inside(mouse_pos)) mouse_window = 2;
            if(surf_side.inside(mouse_pos)) mouse_window = 3;
            if(surf_3d.inside(mouse_pos)) mouse_window = 0;

            std::cout << mouse_pos.x << ' ' << mouse_pos.y << '\n';
        }
        if (Mouse::isButtonPressed(sf::Mouse::Left)){
            vec2 mouse_delta = mouse_pos - pressed_pos;
            pressed_pos = mouse_pos;

            if(mouse_window == 0) {
                camera_3d.change_orientation(vec2(
                    -mouse_delta.x / 100,
                    -mouse_delta.y / 100
                ));
            }
            if(mouse_window == 1) top.pos += mouse_delta;
            if(mouse_window == 2) front.pos += mouse_delta;
            if(mouse_window == 3) side.pos += mouse_delta;
        }
        if(event.type == Event::Resized) {
            View view = window.getDefaultView();
            view.setSize(event.size.width, event.size.height);
            window.setView(view);
        }
    }
}

void draw_circle(RenderWindow& window, window_surface& surface, vec2 pos){
    CircleShape circle(2);
    circle.setFillColor(sf::Color(255, 255, 255));

    double k = 0.04;
    int dc = k*0 / (1 + std::abs(k*0)) + 1;
    circle.setFillColor(sf::Color(255, 128 * dc, 128 * dc));

    int half_w = surface.w / 2, half_h = surface.h / 2;

    if(pos.x > -half_w && pos.y > -half_h && pos.x < half_w && pos.y < half_h)
        circle.setPosition(Vector2f(
            pos.x + surface.x + half_w,
            pos.y + surface.y + half_h
        ));
    window.draw(circle);
}

void draw_text(RenderWindow& window, window_surface& surface, std::string txt, int x = 0, int y = 0){
    Text text;
    text.setFont(font);
    text.setCharacterSize(FONT_HEIGHT);

    text.setString(txt);
    text.setPosition(Vector2f(x + surface.x, y + surface.y));
    window.draw(text);
}

void draw_line(RenderWindow& window, window_surface& surface, int x0, int y0, int x1, int y1){
    Vertex line[] = {
        Vertex(Vector2f(x0 + surface.x, y0 + surface.y)),
        Vertex(Vector2f(x1 + surface.x, y1 + surface.y)),
    };
    window.draw(line, 2, Lines);
}

void render_scene(RenderWindow& window, ent_world world){
    if(time(&current) == last) frame_count++;
    else{
        last = time(&current);
        fps = frame_count;
        frame_count = 0;
    }

    window.clear();

    draw_line(window, screen, 0, screen.h / 2, screen.w, screen.h / 2);
    draw_line(window, screen, screen.w / 2, 0, screen.w / 2, screen.h);

    draw_text(window, surf_top, "x/y:", 0, 0);
    draw_text(window, surf_front, "x/z:", 0, 0);
    draw_text(window, surf_side, "y/z:", 0, 0);
    draw_text(window, surf_3d, "dt = " + std::to_string(world.dt), 0, 0);
    draw_text(window, surf_3d, "fps = " + std::to_string(fps), 0, FONT_HEIGHT + 2);

    for (int i = 0; i < world.count(); i++){
        phys_body b = world.bodies[i];

        draw_circle(window, surf_top, top.coords(b.pos));
        draw_circle(window, surf_front, front.coords(b.pos));
        draw_circle(window, surf_side, side.coords(b.pos));

        draw_circle(window, surf_3d, camera_3d.point_coords(b.pos) * camera_3d.scale);
    }

    window.display();
}