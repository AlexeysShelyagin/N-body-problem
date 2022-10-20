#ifndef N_BODY_PROBLEM_REPLAY_H
#define N_BODY_PROBLEM_REPLAY_H

#include <vector>

#include "Entities.h"
#include "Files.h"

class Replay{
public:
    ent_world world;
    file world_file;
    std::vector < double > dt_list;
    std::vector < int > frame_density;
    double dt_min = INT_MAX, dt_range;
    int body_num;
    int max_dens;

    int frame = 0, frame_num;
    bool paused = 0;

    Replay();
    Replay(std::string filename);

    bool load_frame(int _frame);

    int next_frame();
};

#endif //N_BODY_PROBLEM_REPLAY_H
