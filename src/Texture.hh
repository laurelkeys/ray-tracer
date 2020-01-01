#pragma once
#ifndef TEXTUREHH
#define TEXTUREHH

#include "Perlin.hh"
#include "Vec3.hh"

class Texture {
    public:
        virtual Vec3 value(float u, float v, const Vec3& p) const = 0;
};

class ConstantTexture : public Texture {
    public:
        Vec3 color;

        ConstantTexture() {}
        ConstantTexture(Vec3 c) :
            color(c) {}

        virtual Vec3 value(float u, float v, const Vec3& p) const {
            return color;
        }
};

class CheckerTexture : public Texture {
    public:
        Texture* even;
        Texture* odd;

        CheckerTexture() {}
        CheckerTexture(Texture* t0, Texture* t1) :
            even(t0),
            odd(t1) {}

        virtual Vec3 value(float u, float v, const Vec3& p) const {
            float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
            if (sines < 0)
                return odd->value(u, v, p);
            else
                return even->value(u, v, p);
        }
};

class NoiseTexture : public Texture {
    public:
        Perlin noise;

        float scale = 1.0;

        NoiseTexture() {}
        NoiseTexture(float scale) :
            scale(scale) {}

        virtual Vec3 value(float u, float v, const Vec3& p) const {
            float perlin_noise = sin(scale * p.z() + 10 * noise.turbulence(p));
            // scale and bias the noise to remap it from [-1, 1] to [0, 1]
            return Vec3(1, 1, 1) * (0.5 * (1 + perlin_noise));
        }
};

class ImageTexture : public Texture {
    public:
        int nx, ny;
        unsigned char* data;

        ImageTexture() {}
        ImageTexture(unsigned char* pixels, int A, int B) :
            data(pixels),
            nx(A),
            ny(B) {}

        virtual Vec3 value(float u, float v, const Vec3& p) const;
};

Vec3 ImageTexture::value(float u, float v, const Vec3& p) const {
    int i = u * nx;
    int j = (1 - v) * ny - 0.001;
    i = (i < 0) ? 0 : (i > nx - 1) ? nx - 1 : i; // [0, nx-1]
    j = (j < 0) ? 0 : (j > ny - 1) ? ny - 1 : j; // [0, ny-1]

    int idx = 3 * i + 3 * nx * j;
    float r = int(data[idx]) / 255.0;
    float g = int(data[idx + 1]) / 255.0;
    float b = int(data[idx + 2]) / 255.0;

    return Vec3(r, g, b);
}

#endif