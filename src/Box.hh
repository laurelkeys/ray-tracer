#pragma once
#ifndef BoxHH
#define BoxHH

#include "AABB.hh"
#include "AARect.hh"
#include "Hittable.hh"
#include "HittableList.hh"
#include "Material.hh"
#include "Ray.hh"
#include "Transformations.hh"
#include "Vec3.hh"

class Box : public Hittable {
    public:
        Vec3 pmin, pmax;
        Hittable* list_ptr;

        Box() {}
        Box(const Vec3& p0, const Vec3& p1, Material* ptr);

        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;

        virtual bool bounding_box(float t0, float t1, AABB& box) const {
            box = AABB(pmin, pmax);
            return true;
        }
};

Box::Box(const Vec3& p0, const Vec3& p1, Material* ptr) {
    pmin = p0;
    pmax = p1;
    Hittable** list = new Hittable*[6];

    // planes z = p1.z and z = p0.z
    list[0] = new XYRect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr);
    list[1] = new FlipNormals(
        new XYRect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));
    // planes y = p1.y and y = p0.y
    list[2] = new XZRect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr);
    list[3] = new FlipNormals(
        new XZRect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));
    // planes x = p1.x and x = p0.x
    list[4] = new YZRect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr);
    list[5] = new FlipNormals(
        new YZRect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));

    list_ptr = new HittableList(list, 6);
}

bool Box::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    return list_ptr->hit(r, t_min, t_max, rec);
}

#endif