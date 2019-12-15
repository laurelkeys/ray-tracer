#ifndef RANDOMHH
#define RANDOMHH

#include <random>
#include "Vec3.hh"

namespace Random {
    namespace {
        // ref.: https://stackoverflow.com/questions/686353/random-float-number-generation/17798317#17798317
        // https://en.cppreference.com/w/cpp/numeric/random/random_device
        // https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine
        // https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution

        std::random_device rd;
        std::mt19937 gen(rd());
        
        // uniform distribution \in [0.0, 1.0)
        std::uniform_real_distribution<float> rand_dist(0.0, 1.0);
    }

    // random number \in [0.0, 1.0), with uniform distribution
    float number_ge_0_lt_1() {
        return rand_dist(gen);
    }

    Vec3 point_in_unit_sphere() {
        Vec3 p;
        do {
            // we pick a random point in the unit cube with x, y, z \in [-1.0, 1.0]
            // and if it's outside the sphere we reject it and try again (acceptance rate is 52%)
            float px = number_ge_0_lt_1();
            float py = number_ge_0_lt_1();
            float pz = number_ge_0_lt_1();
            // obs.: 0.0 <= px, py, pz < 1.0, so we map [0.0, 1.0) to [-1.0, 1.0)
            p = 2.0 * Vec3(px, py, pz) - Vec3(1.0, 1.0, 1.0);
        } while (p.squared_length() >= 1.0);
        return p;
    }

    Vec3 point_in_unit_disk() {
        Vec3 p;
        do {
            float px = number_ge_0_lt_1();
            float py = number_ge_0_lt_1();
            p = 2.0 * Vec3(px, py, 0.0) - Vec3(1.0, 1.0, 0.0);
        } while (p.squared_length() >= 1.0);
        return p;
    }
};

#endif
