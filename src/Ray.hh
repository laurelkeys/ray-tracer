#ifndef RAYHH
#define RAYHH

#include "Vec3.hh"

class Ray {
    public:
        Vec3 A;
        Vec3 B;
        float _time;

        Ray() { }
        Ray(const Vec3& a, const Vec3& b, float ti = 0.0) : 
            A(a), 
            B(b),
            _time(ti) { }

        Vec3 origin() const { return A; }
        Vec3 direction() const { return B; }

        float time() const { return _time; }

        // p(t) = A + t*B
        Vec3 point_at_parameter(float t) const { return A + t*B; }
};

#endif