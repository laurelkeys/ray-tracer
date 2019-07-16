#ifndef HITTABLEHH
#define HITTABLEHH

#include "Ray.hh"

struct HitRecord {
    float t;
    Vec3 p; // point p(t) where there was a hit
    Vec3 surface_normal; // versor that points out from the surface
};

class Hittable {
    public:
        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
        // note: the pure virtual function makes Hittable an abstract class, so it can't be instantiated,
        //       and a derived class that implements hit() must be used (i.e. "= 0" makes overriding required)
};

#endif
