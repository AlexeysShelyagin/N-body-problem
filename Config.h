#ifndef CONFIG_H
#define CONFIG_H

#include <map>

#define DEFAULT_FILE "2body.json"

#define SIMULATION_PATH "../Simulations/"
#define SIMULATED_PATH "../Simulated/"

#define OUTPUT_PATH "../Simulated/"
#define DEFAULT_OUTPUT "test6.bin"

#define FONT_HEIGHT 16
#define DEFAULT_CAMERA_SCALE 1e-10
#define DEFAULT_SCENE_SCALE 1e-10
#define TIMELINE_HEIGHT 100
#define TIMELINE_X_INDENT 50
#define TIMELINE_Y_INDENT 50

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
