#ifndef RANDOMHH
#define RANDOMHH

#include <random>
#include "Vec3.hh"

namespace Random {
    namespace {
        // ref.: https://stackoverflow.com/questions/686353/random-float-number-generation/17798317#17798317
        std::random_device rd;
        std::mt19937 e2(rd()); // engine
        std::uniform_real_distribution<float> rand_dist(0.0, 1.0); // distribution \in [0.0, 1.0)
    }

    // random number \in [0.0, 1.0)
    float number_in_01inc_1exc() {
        return rand_dist(e2);
    }

    Vec3 point_in_unit_sphere() {
        Vec3 p;
        do {
            // we pick a random point in the unit cube with x, y, z \in [-1.0, 1.0]
            // and if it's outside the sphere we reject it and try again
            float px = number_in_01inc_1exc();
            float py = number_in_01inc_1exc();
            float pz = number_in_01inc_1exc();
            // obs.: 0.0 <= px, py, pz < 1.0, so we map [0.0, 1.0) to [-1.0, 1.0)
            p = 2.0 * Vec3(px, py, pz) - Vec3(1.0, 1.0, 1.0);
        } while (p.squared_length() >= 1.0);
        return p;
    }

    Vec3 point_in_unit_disk() {
        Vec3 p;
        do {
            float px = number_in_01inc_1exc();
            float py = number_in_01inc_1exc();
            p = 2.0 * Vec3(px, py, 0.0) - Vec3(1.0, 1.0, 0.0);
        } while (p.squared_length() >= 1.0);
        return p;
    }
};

#endif
