#include "Replay.h"
#include <iostream>

Replay::Replay() {
}

Replay::Replay(std::string filename) {
    world_file.open(filename, "rb");

    double G = world_file.read_double();
    body_num = (int) world_file.read_double();
    world = world_file.read_world(body_num);
    world.G = G;

    frame_num = (world_file.size() / sizeof(double) - 2) / 4 / body_num;
    dt_list.resize(frame_num);

    double last_t = 0, T, dt_max = 0;
    for(int i = 0; i < frame_num; i++){
        T = world_file.read_double(sizeof(double) * (2 + (4 * body_num + 1) * i));
        dt_list[i] = T - last_t;
        last_t = T;

        dt_max = std::max(dt_max, dt_list[i]);
        dt_min = std::min(dt_min, dt_list[i]);
    }
    dt_range = dt_max - dt_min;
}

bool Replay::load_frame(int _frame) {
    frame = _frame;
    if(frame < frame_num) world = world_file.read_world(body_num, sizeof(double) * (2 + (4 * body_num + 1) * frame));
    return (frame < frame_num);
}