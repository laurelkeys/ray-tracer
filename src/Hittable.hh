#ifndef HITTABLEHH
#define HITTABLEHH

#include "Vec3.hh"
#include "Ray.hh"
#include "AABB.hh"

class Material; // forward declaration (so Material and Hittable can refer to each other)

struct HitRecord {
    float t;
    float u, v; // UV mapping
    Vec3 p; // point p(t) where there was a hit
    Vec3 surface_normal; // versor that points out from the surface
    Material* material_ptr;
};

class Hittable {
    public:
        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
        virtual bool bounding_box(float t0, float t1, AABB& box) const = 0;
        // note: the pure virtual function makes Hittable an abstract class, so it can't be instantiated,
        //       and a derived class that implements hit() must be used (i.e. "= 0" makes overriding required)
};

class FlipNormals : public Hittable {
    public:
        Hittable *ptr;

        FlipNormals(Hittable *p) : 
            ptr(p) { }

        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
            if (ptr->hit(r, t_min, t_max, rec)) {
                rec.surface_normal = -rec.surface_normal;
                return true;
            }
            return false;
        }
        
        virtual bool bounding_box(float t0, float t1, AABB& box) const {
            return ptr->bounding_box(t0, t1, box);
        }
};

#endif