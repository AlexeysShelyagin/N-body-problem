#include "include/rapidjson/document.h"
#include "include/rapidjson/istreamwrapper.h"
#include <fstream>
#include <time.h>
#include <iostream>
#include <math.h>
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
    double current_m, M = m * n, probability, dist;

    std::vector < double > mass_list(n);
    double full_mass = 0;
    for(int i = 0; i < n; i++){
        mass_list[i] = pow((m * 26.6675 - m * 26.6614 * random_double()), -1/1.3);
        full_mass += mass_list[i];
    }
    std::cout << ">>>" << full_mass << '\n';

    for(int i = 0; i < n; ++i){
        double teta = 2 * PI * random_double();
        double phi = asin(2 * random_double() - 1);

        double radius_x = r * (2 * random_double() - 1);
        double radius_y = r * (2 * random_double() - 1);
        double radius_z = r * (2 * random_double() - 1);
        double radius = sqrt(radius_x * radius_x + radius_y * radius_y + radius_z * radius_z);
        if (random_double() + 5.37 * (radius * radius / r) * pow(1 + (radius * radius / r), -5 / 2) < 1){
            i--;
            continue;
        }

        world.add_body(phys_body(
            ang_vec3(teta, PI/2 - phi) * radius + center,
            current_m,
            1,
            load_vector(body_file, "velocity")
        ));
    }
    double p_e = 0;

    for(int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (i != j && i < j) {
                double r = (world.bodies[i].pos - world.bodies[j].pos).mod();
                p_e += -world.G * mass_list[i] * mass_list[j] / r;
            }
        }
    }

    for(int i = 0; i < n; ++i){
        double Vrms  = sqrt(-p_e / 3 / full_mass);
        std::cout << Vrms << std::endl;
        vel.x = 3.4 * Vrms * (random_double() - 0.5);
        vel.y = 3.4 * Vrms * (random_double() - 0.5);
        vel.z = 3.4 * Vrms * (random_double() - 0.5);
        world.bodies[i].vel += vel;
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
    world.dynamic_dt = world_file["world"]["dynamic_dt"].GetBool();
    world.dt_max = world_file["world"]["dt_max"].GetDouble();
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