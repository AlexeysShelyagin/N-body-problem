#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

#include "Entities.h"
#include "Config.h"
#include "UI.h"
#include "Replay.h"

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
window_surface surf_timeline;

ent_ortho_camera camera_3d(vec3(), vec2(PI / 4, PI / 4), DEFAULT_CAMERA_SCALE);

vec2 pressed_pos;
int mouse_window;

vec2 window_size;

bool window_opened = false;
int frame_count = 0, last, fps = 0;
time_t current;

RenderWindow sf_init_scene(int w, int h, int mode, std::string title, double scale){
    font.loadFromFile("../Fonts/consolas.ttf");

    screen = window_surface(w, h);
    if(mode == 1) screen.h -= TIMELINE_HEIGHT;
    surf_top = window_surface(screen.w / 2, screen.h / 2, screen.w / 2, 0);
    surf_front = window_surface(screen.w / 2, screen.h / 2,0, screen.h / 2);
    surf_side = window_surface(screen.w / 2, screen.h / 2, screen.w / 2, screen.h / 2);
    surf_3d = window_surface(screen.w / 2, screen.h / 2, 0, 0);
    surf_timeline = window_surface(w, TIMELINE_HEIGHT, 0, screen.h);

    window_size = vec2(w, h);

    camera_3d.scale = scale;
    top.scale = scale;
    front.scale = scale;
    side.scale = scale;

    window_opened = true;
    return RenderWindow(VideoMode(w, h), title);
}

bool sf_window_opened(){
    return window_opened;
}

void sf_close_window(RenderWindow& window){
    window.close();
    window_opened = false;
}

void sf_window_event(RenderWindow& window, ent_world& world){
    vec2 mouse_pos(Mouse::getPosition(window).x, Mouse::getPosition(window).y);

    Event event;
    while (window.pollEvent(event)){
        if (event.type == Event::Closed) sf_close_window(window);
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
        /*
        if(event.type == Event::Resized) {

            View view = window.getDefaultView();
            view.setSize(event.size.width, event.size.height);
            window.setView(view);

            vec2 dsize = vec2(event.size.width, event.size.height) - window_size;\
            window_size += dsize;

            screen.change_size(dsize, -dsize * 0.5);
            surf_3d.change_size(dsize * 0.5, -dsize * 0.5);
            surf_top.change_size(dsize * 0.5, vec2(0, -dsize.y * 0.5));
            surf_front.change_size(dsize * 0.5, vec2(-dsize.x * 0.5, 0));
            surf_side.change_size(dsize * 0.5);

        }
        */
    }
}

void draw_circle(RenderWindow& window, window_surface& surface, vec2 pos){
    CircleShape circle(2);
    circle.setFillColor(sf::Color(255, 255, 255));

    double k = 0.04;
    int dc = k*0 / (1 + std::abs(k*0)) + 1;
    circle.setFillColor(Color(255, 128 * dc, 128 * dc));

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

void draw_line(RenderWindow& window, window_surface& surface, int x0, int y0, int x1, int y1, vec3 color = vec3(255, 255, 255)){
    Vertex line[] = {
        Vertex(Vector2f(x0 + surface.x, y0 + surface.y), Color(color.x, color.y, color.z)),
        Vertex(Vector2f(x1 + surface.x, y1 + surface.y), Color(color.x, color.y, color.z)),
    };
    window.draw(line, 2, Lines);
}

void draw_timeline(RenderWindow& window, Replay& replay){
    draw_line(window, surf_timeline, 0, 0, surf_timeline.w, 0);

    RectangleShape rect;
    double rect_w = (double) (surf_timeline.w - 100) / replay.frame_num;
    rect.setSize(Vector2f( rect_w,  25));
    double val;
    for(int i = 0; i < replay.frame_num; i++){
        val = (replay.dt_list[i] - replay.dt_min) / replay.dt_range;
        rect.setPosition(surf_timeline.x + 50 + rect_w * i, surf_timeline.y + 50);
        rect.setFillColor(Color(val * 200 + 55, 0, 0));

        window.draw(rect);
    }

    draw_line(window, surf_timeline, 50 + rect_w * replay.frame, 25, 50 + rect_w * replay.frame, 85);
}

void render_scene(RenderWindow& window, ent_world& world, bool show){
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

    if(show) window.display();
}

void render_scene(RenderWindow& window, ent_world& world, Replay& replay){
    render_scene(window, world, false);

    draw_timeline(window, replay);
    window.display();
}