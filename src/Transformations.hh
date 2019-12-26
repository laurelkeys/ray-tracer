#ifndef TRANSFORMATIONSHH
#define TRANSFORMATIONSHH

#include "Vec3.hh"
#include "Ray.hh"
#include "AABB.hh"
#include "Hittable.hh"

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

class Translate : public Hittable {
    public:
        Hittable *ptr;
        Vec3 offset;
        
        Translate(Hittable *p, const Vec3& displacement) : 
            ptr(p), 
            offset(displacement) {}
        
        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
        virtual bool bounding_box(float t0, float t1, AABB& box) const;
};

bool Translate::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    Ray moved_r(r.origin() - offset, r.direction(), r.time());
    if (ptr->hit(moved_r, t_min, t_max, rec)) {
        rec.p += offset;
        return true;
    }
    return false;
}

bool Translate::bounding_box(float t0, float t1, AABB& box) const {
    if (ptr->bounding_box(t0, t1, box)) {
        box = AABB(box.min() + offset, box.max() + offset);
        return true;
    }
    return false;
}

#endif