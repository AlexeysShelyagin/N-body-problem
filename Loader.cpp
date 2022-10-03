#include "include/rapidjson/document.h"
#include "include/rapidjson/istreamwrapper.h"
#include <fstream>
#include <time.h>

#include "Entities.h"
#include "Config.h"
#include "Vectors.h"

using namespace rapidjson;

vec3 load_vector(Value::Object vec_file, std::string _key){
    char key[_key.length()];
    strcpy(key, _key.c_str());

    return vec3(
        vec_file[key][0].GetDouble(),
        vec_file[key][1].GetDouble(),
        vec_file[key][2].GetDouble()
    );
}

phys_body load_body (Value::Object body_file){
    return phys_body(
        load_vector(body_file, "position"),
        body_file["mass"].GetDouble(),
        active_list[ body_file["class"].GetString() ],
        load_vector(body_file, "velocity"),
        body_file["radius"].GetDouble(),
        body_file["name"].GetString()
    );
}

phys_body load_body (Value::Object body_file, phys_body relative_body){
    vec3 ang = load_vector(body_file, "angle") * PI / 180;
    vec3 dir = ang_vec3(ang.x, ang.y);

    return phys_body(
        relative_body.pos + dir * body_file["distance"].GetDouble(),
        body_file["mass"].GetDouble(),
        active_list[ body_file["class"].GetString() ],
        load_vector(body_file, "velocity") + relative_body.vel,
        body_file["radius"].GetDouble(),
        body_file["name"].GetString()
    );
}

double random_double(){
    return (double) rand() / RAND_MAX;
}

void load_random_sphere(ent_world &world, Value::Object body_file){
    srand( time(NULL) );

    int n = body_file["number"].GetInt();
    double m = body_file["mass"].GetDouble();
    double dm = body_file["mass_range"].GetDouble();
    double r = body_file["radius"].GetDouble();
    vec3 center = load_vector(body_file, "center_position");

    vec3 pos, vel;
    double current_m, M = m * n;

    for(int i = 0; i < n; ++i){
        do {
            pos.x = (random_double() - 0.5) * 2 * r;
            pos.y = (random_double() - 0.5) * 2 * r;
            pos.z = (random_double() - 0.5) * 2 * r;
        } while (pos.mod() > r);
        current_m = m + (random_double() - 0.5) * 2 * dm;

        vel = ang_vec3(random_double() * 2 * PI, random_double() * PI);
        vel *= sqrt(1.2 * (world.G * M * M) / (r * current_m * n));

        world.add_body(phys_body(
            pos + center,
            current_m,
            1,
            load_vector(body_file, "velocity") + vel
        ));
    }
}

ent_world load_simulation(std::string file_path){
    std::ifstream ifs(file_path);
    IStreamWrapper isw(ifs);
    Document world_file;
    world_file.ParseStream(isw);
    ent_world world = ent_world();

    world.G = world_file["world"]["G"].GetDouble();
    world.dt = world_file["world"]["dt"].GetDouble();
    world.end_time = world_file["world"]["end time"].GetDouble();
    world.render = world_file["world"]["render"].GetBool();
    world.save = world_file["world"]["save"].GetBool();
    world.calc_method = world_file["world"]["method"].GetString();

    int type;

    for (int i = 0; i < world_file["entities"].Size(); i++){
        type = entities_list[ world_file["entities"][i]["class"].GetString() ];
        if (type == 0){
            world.add_body( load_body(world_file["entities"][i].GetObject()) );
        }
        if(type == 1){
            std::string relative = world_file["entities"][i]["relative"].GetString();
            int relative_i = world.find_body(relative);
            world.add_body(
                load_body(world_file["entities"][i].GetObject(), world.bodies[relative_i])
            );
        }
        if(type == 2){
            load_random_sphere(world, world_file["entities"][i].GetObject());
        }
    }

    return world;
}