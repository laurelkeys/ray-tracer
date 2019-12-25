#ifndef AARECTHH
#define AARECTHH

#include "Vec3.hh"
#include "Ray.hh"
#include "AABB.hh"
#include "Material.hh"
#include "Hittable.hh"

// Axis-Aligned Rectangles

class XYRect: public Hittable {
    public:
        Material *material_ptr;
        float x0, x1, y0, y1, k; // rect in the plane z(t) = k

        XYRect() { }
        XYRect(float x0, float x1, float y0, float y1, float k, Material *mat) : 
            x0(x0), x1(x1), 
            y0(y0), y1(y1), 
            k(k), 
            material_ptr(mat) { }
        
        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
        
        virtual bool bounding_box(float t0, float t1, AABB& box) const {
            box = AABB(Vec3(x0, y0, k - 0.0001), Vec3(x1, y1, k + 0.0001));
            return true;
        }
};

bool XYRect::hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
    float t = (k - r.origin().z()) / r.direction().z();
    if (t < t0 || t > t1)
        return false;

    float x = r.origin().x() + t * r.direction().x();
    float y = r.origin().y() + t * r.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    rec.material_ptr = material_ptr;
    rec.p = r.point_at_parameter(t);
    rec.surface_normal = Vec3(0, 0, 1); // z
    return true;
}

#endif