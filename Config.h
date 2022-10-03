#ifndef CONFIG_H
#define CONFIG_H

#include <map>

#define SIMULATION_PATH "../Simulations/"
#define SIMULATED_PATH "../Simulated/"
#define DEFAULT_FILE "test6.json"

#define OUTPUT_PATH "../Simulated/"
#define DEFAULT_OUTPUT "test.bin"

#define FONT_HEIGHT 16

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
