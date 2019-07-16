#ifndef RANDOMNUMBERHH
#define RANDOMNUMBERHH

#include <random>

namespace RandomNumber {
    namespace {
        // ref.: https://stackoverflow.com/questions/686353/random-float-number-generation/17798317#17798317
        std::random_device rd;
        std::mt19937 e2(rd()); // engine
        std::uniform_real_distribution<float> rand_dist(0.0, 1.0); // distribution \in [0.0, 1.0)
    }

    // random number \in [0.0, 1.0)
    float in_01inc_1exc() {
        return rand_dist(e2);
    }
};

#endif
