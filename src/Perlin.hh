#ifndef PERLINHH
#define PERLINHH

#include "Vec3.hh"
#include "Random.hh"

inline float trilinear_interp(float c[2][2][2], float u, float v, float w) {
    float acc = 0;
    for (int i = 0; i <= 1; ++i)
        for (int j = 0; j <= 1; ++j)
            for (int k = 0; k <= 1; ++k)
                acc += (i * u + (1-i) * (1-u)) *
                       (j * v + (1-j) * (1-v)) *
                       (k * w + (1-k) * (1-w)) * c[i][j][k];
    return acc;
}

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

            float c[2][2][2];
            for (int di = 0; di <= 1; ++di)
                for (int dj = 0; dj <= 1; ++dj)
                    for (int dk = 0; dk <= 1; ++dk)
                        c[di][dj][dk] = ranfloat[
                            perm_x[(i + di) & 255] ^
                            perm_y[(j + dj) & 255] ^
                            perm_z[(k + dk) & 255]
                        ];
            
            float u = p.x() - i;
            float v = p.y() - j;
            float w = p.z() - k;
            // Hermite cubic to round off the interpolation
            // and minimize the grid features that appear (Mach bands)
            u = u*u * (3 - 2*u);
            v = v*v * (3 - 2*v);
            w = w*w * (3 - 2*w);
            return trilinear_interp(c, u, v, w);
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