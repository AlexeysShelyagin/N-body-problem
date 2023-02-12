#ifndef N_BODY_PROBLEM_ANALYSIS_H
#define N_BODY_PROBLEM_ANALYSIS_H

#include <vector>
#include <map>

class ent_world;

class World_function{
protected:
    ent_world* world;
public:
    virtual ~World_function() = default;
};

class Group_checker : public World_function{
protected:
    struct Range{
        int beg, end;
    };
    Range group;
};

///------------------------------------------------------------------

///------------------------Lifetime checker--------------------------

class Lifetime_checker : public Group_checker{
    std::map < int, int > scatter_possible;
public:

    Lifetime_checker() {};
    Lifetime_checker(ent_world& world_ptr, int i_begin, int i_end);

    void check_scattering();
};

///------------------------------------------------------------------

///----------------------Binary star checker-------------------------

class Binary_star_checker : public Group_checker{
    std::vector < std::pair < int, int > > binary_possible;
public:
    Binary_star_checker() {};
    Binary_star_checker(ent_world& world_ptr, int i_begin, int i_end);
};

#endif //N_BODY_PROBLEM_ANALYSIS_H
