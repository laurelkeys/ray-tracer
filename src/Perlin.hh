#pragma once
#ifndef PERLINHH
#define PERLINHH

#include "Random.hh"
#include "Vec3.hh"

inline float perlin_interp(Vec3 c[2][2][2], float u, float v, float w) {
    // Hermite cubic to round off the interpolation
    // and minimize the grid features that appear (Mach bands)
    float uu = u * u * (3 - 2 * u);
    float vv = v * v * (3 - 2 * v);
    float ww = w * w * (3 - 2 * w);

    float acc = 0;
    for (int i = 0; i <= 1; ++i)
        for (int j = 0; j <= 1; ++j)
            for (int k = 0; k <= 1; ++k) {
                Vec3 weight_v(u - i, v - j, w - k);
                // trilinear interpolation
                acc += (i * uu + (1 - i) * (1 - uu)) * 
                       (j * vv + (1 - j) * (1 - vv)) * 
                       (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weight_v);
            }
    return acc;
}

class Perlin {
    public:
        static Vec3* ranvec;

        static int* perm_x;
        static int* perm_y;
        static int* perm_z;

        float noise(const Vec3& p) const {
            int i = floor(p.x());
            int j = floor(p.y());
            int k = floor(p.z());

            Vec3 c[2][2][2];
            for (int di = 0; di <= 1; ++di)
                for (int dj = 0; dj <= 1; ++dj)
                    for (int dk = 0; dk <= 1; ++dk)
                        c[di][dj][dk] = ranvec[
                            perm_x[(i + di) & 255] ^ 
                            perm_y[(j + dj) & 255] ^ 
                            perm_z[(k + dk) & 255]
                        ];

            float u = p.x() - i;
            float v = p.y() - j;
            float w = p.z() - k;
            return perlin_interp(c, u, v, w);
        }

        float turbulence(const Vec3& p, int depth = 7) const {
            float acc = 0;
            Vec3 temp_p = p;
            float weight = 1;
            for (int i = 0; i < depth; ++i) {
                acc += weight * noise(temp_p);
                weight *= 0.5;
                temp_p *= 2;
            }
            return fabs(acc);
        }
};

static Vec3* Perlin_generate() {
    Vec3* p = new Vec3[256];
    for (int i = 0; i < 256; ++i) {
        double x_random = 2 * Random::number_ge_0_lt_1() - 1;
        double y_random = 2 * Random::number_ge_0_lt_1() - 1;
        double z_random = 2 * Random::number_ge_0_lt_1() - 1;
        p[i] = unit_vector(Vec3(x_random, y_random, z_random));
    }
    return p;
}

void permute(int* p, int n) {
    for (int i = n - 1; i > 0; --i) {
        int target = int(Random::number_ge_0_lt_1() * (i + 1));
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

static int* Perlin_generate_perm() {
    int* p = new int[256];
    for (int i = 0; i < 256; ++i)
        p[i] = i;
    permute(p, 256);
    return p;
}

Vec3* Perlin::ranvec = Perlin_generate();

int* Perlin::perm_x = Perlin_generate_perm();
int* Perlin::perm_y = Perlin_generate_perm();
int* Perlin::perm_z = Perlin_generate_perm();

#endif