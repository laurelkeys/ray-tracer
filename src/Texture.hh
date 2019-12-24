#ifndef TEXTUREHH
#define TEXTUREHH

#include "Vec3.hh"
#include "Perlin.hh"

class Texture {
    public:
        virtual Vec3 value(float u, float v, const Vec3& p) const = 0;
};

class ConstantTexture : public Texture {
    public:
        Vec3 color;
        
        ConstantTexture() { }
        ConstantTexture(Vec3 c) : color(c) { }
        
        virtual Vec3 value(float u, float v, const Vec3& p) const {
            return color;
        }
};

class CheckerTexture : public Texture {
    public:
        Texture *even;
        Texture *odd;

        CheckerTexture() { }
        CheckerTexture(Texture *t0, Texture *t1) : 
            even(t0), 
            odd(t1) { }
        
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
        
        NoiseTexture() { }
        NoiseTexture(float scale) : 
            scale(scale) { }
        
        virtual Vec3 value(float u, float v, const Vec3& p) const {
            float perlin_noise = noise.noise(scale * p);
            // scale and bias the noise to remap it from [-1, 1] to [0, 1]
            return Vec3(1, 1, 1) * (0.5 * (1 + perlin_noise));
        }
};

#endif