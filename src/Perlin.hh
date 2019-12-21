#ifndef PERLINHH
#define PERLINHH

// #include <iostream>
#include "Random.hh"

class Perlin {
    public:
        static float *ranfloat;
        
        static int *perm_x;
        static int *perm_y;
        static int *perm_z;

        float noise(const Vec3& p) const {
            int i = floor(p.x());
            int j = floor(p.y());
            int k = floor(p.z());
            float u = p.x() - i;
            float v = p.y() - j;
            float w = p.z() - k;
            // FIXME
            i = i >= 0 ? i : -i;
            j = j >= 0 ? j : -j;
            k = k >= 0 ? k : -k;
            // std::cout << "u: " << u << " | i: " << i << " | x: " << perm_x[i] << ", " << perm_x[i % 255] << std::endl;
            // std::cout << "v: " << v << " | j: " << j << " | y: " << perm_y[j] << ", " << perm_y[j % 255] << std::endl;
            // std::cout << "w: " << w << " | k: " << k << " | z: " << perm_x[k] << ", " << perm_x[k % 255] << std::endl;
            // std::cout << (perm_x[i] ^ perm_y[j] ^ perm_z[k]) << ", " << (perm_x[i%255] ^ perm_y[j%255] ^ perm_z[k%255]) << std::endl;
            return ranfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
        }
};

static float* Perlin_generate() {
    float *p = new float[256];
    for (int i = 0; i < 256; ++i)
        p[i] = Random::number_ge_0_lt_1();
    return p;
}

void permute(int *p, int n) {
    for (int i = n-1; i > 0; --i) {
        int target = int(Random::number_ge_0_lt_1() * (i+1));
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

static int* Perlin_generate_perm() {
    int *p = new int[256];
    for (int i = 0; i < 256; ++i)
        p[i] = i;
    permute(p, 256);
    return p;
}

float *Perlin::ranfloat = Perlin_generate();

int *Perlin::perm_x = Perlin_generate_perm();
int *Perlin::perm_y = Perlin_generate_perm();
int *Perlin::perm_z = Perlin_generate_perm();

#endif