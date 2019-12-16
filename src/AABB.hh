#ifndef AABBHH
#define AABBHH

#include "Ray.hh"

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

        inline bool AABB::hit(const Ray& r, float t_min, float t_max) const {
            for (int a = 0; a < 3; a++) {
                float invD = 1.0f / r.direction()[a];
                float t0 = (min()[a] - r.origin()[a]) * invD;
                float t1 = (max()[a] - r.origin()[a]) * invD;
                if (invD < 0.0f)
                    std::swap(t0, t1);
                t_min = t0 > t_min ? t0 : t_min;
                t_max = t1 < t_max ? t1 : t_max;
                if (t_max <= t_min)
                    return false;
            }
            return true;
        }
};

#endif