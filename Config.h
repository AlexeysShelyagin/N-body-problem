#ifndef CONFIG_H
#define CONFIG_H

#include <map>

#define DEFAULT_FILE "test6.bin"

#define SIMULATION_PATH "../Simulations/"
#define SIMULATED_PATH "../Simulated/"

#define OUTPUT_PATH "../Simulated/"
#define DEFAULT_OUTPUT "test6.bin"

#define FONT_HEIGHT 16
#define DEFAULT_CAMERA_SCALE 0.1
#define DEFAULT_SCENE_SCALE 0.1
#define TIMELINE_HEIGHT 100

static std::map < std::string, int > entities_list {
        {"phys_body", 0},
        {"static_body", 0},
        {"relative_body", 1},
        {"random_sphere", 2}
};

static std::map < std::string, bool > active_list {
        {"phys_body", 1},
        {"static_body", 0},
        {"relative_body", 1}
};

#endif //CONFIG_H
