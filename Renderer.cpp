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

std::vector < double > vel_q;
std::vector < double > max_vel_q;

Color HSV(double H, double S, double V){
    S /= 100;
    V /= 100;

    double f, p, t, q;
    double r, g, b;

    if(H >= 360) H = 0;
    H = H / 60;
    int i = (int) trunc(H);
    f = H - i;

    p = V * (1.0 - S);
    q = V * (1.0 - (S * f));
    t = V * (1.0 - (S * (1.0 - f)));

    switch (i) {
        case 0:
            r = V;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = V;
            b = p;
            break;
        case 2:
            r = p;
            g = V;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = V;
            break;
        case 4:
            r = t;
            g = p;
            b = V;
            break;
        default:
            r = V;
            g = p;
            b = q;
    }

    int R = r * 255;
    int G = g * 255;
    int B = b * 255;

    return Color(R, G, B);
}

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

Event_result sf_window_event(RenderWindow& window, ent_world& world){
    Event_result res;
    vec2 mouse_pos(Mouse::getPosition(window).x, Mouse::getPosition(window).y);

    Event event;
    while (window.pollEvent(event)){
        if (event.type == Event::Closed) sf_close_window(window);

        if (event.type == Event::MouseWheelScrolled){
            if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {
                if (surf_top.inside(mouse_pos) || surf_front.inside(mouse_pos) || surf_side.inside(mouse_pos)) {
                    double scale = top.scale * (1 + event.mouseWheelScroll.delta);
                    top.scale = front.scale = side.scale = scale;
                }
                if (surf_3d.inside(mouse_pos)) {
                    camera_3d.scale *= 1 + event.mouseWheelScroll.delta;
                }
                res.type = "v_w";   //vertical_wheel
                res.value = event.mouseWheelScroll.delta;
            }
            if (event.mouseWheelScroll.wheel == Mouse::HorizontalWheel){
                res.type = "h_w";   //horizontal_wheel
                res.value = event.mouseWheelScroll.delta;
            }
        }

        if(event.type == Event::KeyPressed){
            if(event.key.code == Keyboard::Space){
                res.type = "space"; //space key
            }
        }

        if (event.type == Event::MouseButtonPressed){
            pressed_pos = mouse_pos;

            if(surf_top.inside(mouse_pos)) mouse_window = 1;
            if(surf_front.inside(mouse_pos)) mouse_window = 2;
            if(surf_side.inside(mouse_pos)) mouse_window = 3;
            if(surf_3d.inside(mouse_pos)) mouse_window = 0;
            if(surf_timeline.inside(mouse_pos)) mouse_window = 4;

            res.type = "m_p";   //mouse_pressed
            res.value = mouse_window;
        }
        if (event.type == Event::MouseButtonReleased){
            mouse_window = -1;
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
            if(mouse_window == 4){
                res.type = "c_f";    //change_frame
                res.value = (mouse_pos.x - TIMELINE_X_INDENT) / (surf_timeline.w - 2 * TIMELINE_X_INDENT);
            }
        }
    }

    return res;
}

void draw_circle(RenderWindow& window, window_surface& surface, vec2 pos, std::vector < std::vector < int > > &body_buff){
    CircleShape circle(1);

    int half_w = surface.w / 2, half_h = surface.h / 2;

    int screen_x = 0, screen_y = 0, num;
    if(pos.x > -half_w && pos.y > -half_h && pos.x < half_w && pos.y < half_h) {
        screen_x = pos.x + surface.x + half_w;
        screen_y = pos.y + surface.y + half_h;
        circle.setPosition(Vector2f(screen_x, screen_y));
        num = body_buff[screen_x / 2][screen_y / 2]++;
    }
    circle.setFillColor(Color(HSV(5, int(55 * exp(-num * 0.5)) + 10, 95)));

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
        rect.setPosition(surf_timeline.x + TIMELINE_X_INDENT + rect_w * i, surf_timeline.y + TIMELINE_Y_INDENT);
        rect.setFillColor(Color(HSV(180 - val * 180, 80, 100)));

        window.draw(rect);
    }

    draw_line(window, surf_timeline, TIMELINE_X_INDENT + rect_w * replay.frame, TIMELINE_Y_INDENT - 25, TIMELINE_X_INDENT + rect_w * replay.frame, TIMELINE_Y_INDENT + 35);
}

void draw_diagram(RenderWindow& window, window_surface surface, ent_world& world){
    int w = 800, h = 200;

    vel_q.push_back(world.avg_velocity());
    if (vel_q.size() > w) vel_q.erase(vel_q.begin());
    max_vel_q.push_back(world.max_velocity().y);
    if (max_vel_q.size() > w) max_vel_q.erase(max_vel_q.begin());

    surface.x += 10;
    surface.y += 140;

    draw_line(window, surface, 0, 0, w, 0);
    draw_line(window, surface, 0, h, w, h);
    draw_line(window, surface, 0, 0, 0, h);
    draw_line(window, surface, w, 0, w, h);

    double max_vel = 0, min_vel = INT_MAX, scale;
    double m_max_vel = 0, m_min_vel = INT_MAX, m_scale;
    for(int i = 0; i < vel_q.size(); i++){
        if(vel_q[i] > max_vel) max_vel = vel_q[i];
        if(vel_q[i] < min_vel) min_vel = vel_q[i];

        if(max_vel_q[i] > m_max_vel) m_max_vel = max_vel_q[i];
        if(max_vel_q[i] < m_min_vel) m_min_vel = max_vel_q[i];
    }
    scale = h / (max_vel - min_vel);
    m_scale = h / (m_max_vel - m_min_vel);

    int val, m_val;
    for(int i = 0; i < vel_q.size(); i++){
        val = (vel_q[i] - min_vel) * scale;
        m_val = (max_vel_q[i] - m_min_vel) * m_scale;

        draw_line(window, surface, i, h - val, i + 1, h - val, vec3(227, 134, 34));
        draw_line(window, surface, i, h - m_val, i + 1, h - m_val, vec3(34, 147, 227));
    }
}

void draw_diagram_2(RenderWindow& window, window_surface surface, ent_world& world){
    int w = world.count(), h = 150;

    surface.x += 10;
    surface.y += 350;

    draw_line(window, surface, 0, 0, w, 0);
    draw_line(window, surface, 0, h, w, h);
    draw_line(window, surface, 0, 0, 0, h);
    draw_line(window, surface, w, 0, w, h);

    double min_vel = world.min_velocity().x;
    double max_vel = world.max_velocity().x;
    double avg_vel = world.avg_velocity();

    double scale = (h-5) / (max_vel - min_vel);
    int val;
    for(int i = 0; i < world.count(); i++){
        vec3 color = vec3(255, 255, 255);
        if (!world.bodies[i].active) color = vec3(255, 0, 0);
        val = (world.bodies[i].vel.mod() - min_vel) * scale;
        draw_line(window, surface, i, h - val, i, h - val - 1, color);
    }
    int avg_h = (avg_vel - min_vel) * scale;
    draw_line(window, surface, 0, h - avg_h, w, h - avg_h, vec3(227, 134, 34));
}

void render_scene(RenderWindow& window, ent_world& world, std::string &filename, bool show){
    if(time(&current) == last) frame_count++;
    else{
        last = time(&current);
        fps = frame_count;
        frame_count = 0;
    }

    std::vector < std::vector < int > > body_buff(screen.w / 2, std::vector < int > (screen.h / 2));

    window.clear();

    draw_line(window, screen, 0, screen.h / 2, screen.w, screen.h / 2);
    draw_line(window, screen, screen.w / 2, 0, screen.w / 2, screen.h);

    draw_text(window, surf_top, "x/y:", 0, 0);
    draw_text(window, surf_front, "x/z:", 0, 0);
    draw_text(window, surf_side, "y/z:", 0, 0);

    draw_text(window, surf_3d, filename, 0, 0);
    draw_text(window, surf_3d, world.calc_method, 0, FONT_HEIGHT + 2);
    draw_text(window, surf_3d, "dt = " + std::to_string(world.dt), 0, (FONT_HEIGHT + 2) * 2);
    draw_text(window, surf_3d, "time = " + std::to_string((long long) world.time), 0, (FONT_HEIGHT + 2) * 3);
    draw_text(window, surf_3d, "bodies = " + std::to_string(world.count()), 0, (FONT_HEIGHT + 2) * 4);
    draw_text(window, surf_3d, std::to_string((long long) (world.energy / 1e3)) + " J", 0, (FONT_HEIGHT + 2) * 5);
    draw_text(window, surf_3d, std::to_string(fps) + " fps", 0, (FONT_HEIGHT + 2) * 6);

    draw_diagram(window, surf_3d, world);
    draw_diagram_2(window, surf_3d, world);

    for (int i = 0; i < world.count(); i++){
        phys_body b = world.bodies[i];
        if (!b.active) continue;

        draw_circle(window, surf_top, top.coords(b.pos), body_buff);
        draw_circle(window, surf_front, front.coords(b.pos), body_buff);
        draw_circle(window, surf_side, side.coords(b.pos), body_buff);

        draw_circle(window, surf_3d, camera_3d.point_coords(b.pos) * camera_3d.scale, body_buff);
    }

    if(show) window.display();
}

void render_scene(RenderWindow& window, ent_world& world, std::string &filename, Replay& replay){
    render_scene(window, world, filename, false);

    draw_timeline(window, replay);
    window.display();
}