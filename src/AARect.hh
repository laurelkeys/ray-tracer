#ifndef AARECTHH
#define AARECTHH

#include "AABB.hh"
#include "Hittable.hh"
#include "Material.hh"
#include "Ray.hh"
#include "Vec3.hh"

// Axis-Aligned Rectangles

class XYRect : public Hittable {
    public:
        float x0, x1, y0, y1, k; // rect in the plane z(t) = k
        Material* material_ptr;

        XYRect() { }
        XYRect(float x0, float x1, float y0, float y1, float k, Material* mat) :
            x0(x0), x1(x1), 
            y0(y0), y1(y1), 
            k(k), material_ptr(mat) { }

        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;

        virtual bool bounding_box(float t0, float t1, AABB& box) const {
            box = AABB(Vec3(x0, y0, k - 0.0001), Vec3(x1, y1, k + 0.0001));
            return true;
        }
};

class XZRect : public Hittable {
    public:
        float x0, x1, z0, z1, k; // rect in the plane y(t) = k
        Material* material_ptr;

        XZRect() { }
        XZRect(float x0, float x1, float z0, float z1, float k, Material* mat) :
            x0(x0), x1(x1), 
            z0(z0), z1(z1), 
            k(k), material_ptr(mat) { }

        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;

        virtual bool bounding_box(float t0, float t1, AABB& box) const {
            box = AABB(Vec3(x0, k - 0.0001, z0), Vec3(x1, k + 0.0001, z1));
            return true;
        }
};

class YZRect : public Hittable {
    public:
        float y0, y1, z0, z1, k; // rect in the plane x(t) = k
        Material* material_ptr;

        YZRect() { }
        YZRect(float y0, float y1, float z0, float z1, float k, Material* mat) :
            y0(y0), y1(y1), 
            z0(z0), z1(z1), 
            k(k), material_ptr(mat) { }

        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;

        virtual bool bounding_box(float t0, float t1, AABB& box) const {
            box = AABB(Vec3(k - 0.0001, y0, z0), Vec3(k + 0.0001, y1, z1));
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

bool XZRect::hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
    float t = (k - r.origin().y()) / r.direction().y();
    if (t < t0 || t > t1)
        return false;

    float x = r.origin().x() + t * r.direction().x();
    float z = r.origin().z() + t * r.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.material_ptr = material_ptr;
    rec.p = r.point_at_parameter(t);
    rec.surface_normal = Vec3(0, 1, 0); // y
    return true;
}

bool YZRect::hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
    float t = (k - r.origin().x()) / r.direction().x();
    if (t < t0 || t > t1)
        return false;

    float y = r.origin().y() + t * r.direction().y();
    float z = r.origin().z() + t * r.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;

    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.material_ptr = material_ptr;
    rec.p = r.point_at_parameter(t);
    rec.surface_normal = Vec3(1, 0, 0); // x
    return true;
}

#endif