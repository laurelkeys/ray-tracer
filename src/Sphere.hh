#ifndef SPHEREHH
#define SPHEREHH

#include "AABB.hh"
#include "Constants.hh"
#include "Hittable.hh"
#include "Material.hh"
#include "Ray.hh"
#include "Vec3.hh"

void get_sphere_uv(const Vec3& p, float& u, float& v) {
    float phi = atan2(p.z(), p.x());   //   [-π, π]
    float theta = asin(p.y());         // [-π/2, π/2]
    u = 1 - (phi + _PI_) / (2 * _PI_); //   [-1, 1]
    v = (theta + _PI_ / 2) / _PI_;     //   [ 0, 1]
}

class Sphere : public Hittable {
    public:
        Vec3 center;
        float radius;
        Material* material_ptr;

        Sphere() { }
        Sphere(Vec3 center, float radius, Material* material_ptr) :
            center(center),
            radius(radius),
            material_ptr(material_ptr) { }

        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
        virtual bool bounding_box(float t0, float t1, AABB& box) const;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    // Sphere equation, with center C:
    //   <p(t) - C, p(t) - C> = R*R
    //   -> <A + t*B - C, A + t*B - C> = R*R
    //   -> t*t*<B, B> + 2*t*<B, A - C> + <A - C, A - C> - R*R = 0
    //
    // The sphere is hit by the ray if there is a value of t,
    // with t_min < t < t_max, such that p(t) satisfies the equation

    Vec3 o_c = r.origin() - center; // A - C
    float a = dot(r.direction(), r.direction()); // <B, B>
    float b = dot(r.direction(), o_c); // <B, A - C>
    float c = dot(o_c, o_c) - radius * radius; // <A - C, A - C> - R*R
    float discriminant = b * b - a * c;

    if (discriminant > 0.0) { // the ray hits the sphere
        // note: we didn't use 2 factors above since they'd cancel out below
        float t = (-b - sqrt(discriminant)) / a;
        if (t_min < t && t < t_max) {
            rec.t = t;
            rec.p = r.point_at_parameter(t);
            get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
            rec.surface_normal = (rec.p - center) / radius; // versor
            rec.material_ptr = material_ptr;
            return true;
        }

        t = (-b + sqrt(discriminant)) / a;
        if (t_min < t && t < t_max) {
            rec.t = t;
            rec.p = r.point_at_parameter(t);
            get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
            rec.surface_normal = (rec.p - center) / radius; // versor
            rec.material_ptr = material_ptr;
            return true;
        }
    }

    // note: we consider that if a ray tangentiates the sphere it does not hit it
    return false;
}

bool Sphere::bounding_box(float t0, float t1, AABB& box) const {
    box = AABB(center - Vec3(radius, radius, radius),
               center + Vec3(radius, radius, radius));
    return true; // returns a bool because not all primitives have bounding boxes (e.g. infinite planes)
}

#endif