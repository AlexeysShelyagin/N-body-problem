#ifndef CONFIG_H
#define CONFIG_H

#include <map>

#define DEFAULT_FILE "Cluster_large.json"

#define SIMULATION_PATH "../Simulations/"
#define SIMULATED_PATH "../Simulated/"

#define OUTPUT_PATH "../Simulated/"
#define DEFAULT_OUTPUT "Cluster_large.bin"

#define FONT_HEIGHT 16
#define DEFAULT_CAMERA_SCALE 1e-15
#define DEFAULT_SCENE_SCALE 1e-15
#define TIMELINE_HEIGHT 100
#define TIMELINE_X_INDENT 50
#define TIMELINE_Y_INDENT 50

#define R_R0_FACTOR 50
#define VEL_MAX 1e20
#define SCATTER_VELOCITY_FACTOR 2.2
#define SCATTER_VELOCITY_DEVIATION 0.05
#define SCATTER_WAIT_STEPS 3
#define SCATTER_ACCEPT_STEPS 20

static std::map < std::string, int > entities_list {
        {"phys_body", 0},
        {"static_body", 0},
        {"relative_body", 1},
        {"star_cluster", 2},
        {"func_lifetime_checker", 3}
};

static std::map < std::string, bool > active_list {
        {"phys_body", 1},
        {"static_body", 0},
        {"relative_body", 1}
};

static std::map < std::string, int > functions_list {
        {"func_lifetime_checker", 0},
        {"func_binary_star_checker", 1}
};

#endif //CONFIG_H
