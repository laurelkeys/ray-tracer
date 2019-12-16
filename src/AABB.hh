#ifndef AABBHH
#define AABBHH

#include "Ray.hh"

// note: by not worrying about NaN and other exceptions these 
//       are a bit faster than the built-in fmin() and fmax()
inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class AABB { // Axis-Aligned Bounding Box
    public:
        Vec3 _min;
        Vec3 _max;

        AABB() {}
        AABB(const Vec3& a, const Vec3& b) : 
            _min(a), 
            _max(b) { }

        Vec3 min() const { return _min; }
        Vec3 max() const { return _max; }

        bool hit(const Ray& r, float t_min, float t_max) const {
            for (int a = 0; a < 3; ++a) {
                float t0 = ffmin((_min[a] - r.origin()[a]) / r.direction()[a],
                                 (_max[a] - r.origin()[a]) / r.direction()[a]);
                float t1 = ffmax((_min[a] - r.origin()[a]) / r.direction()[a],
                                 (_max[a] - r.origin()[a]) / r.direction()[a]);
                
                t_min = ffmax(t0, t_min);
                t_max = ffmin(t1, t_max);

                if (t_max <= t_min)
                    return false;
            }

            return true;
        }
};

#endif