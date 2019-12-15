#ifndef MOVINGSPHEREHH
#define MOVINGSPHEREHH

#include "Material.hh"

class MovingSphere : public Hittable {
    public:
        Vec3 center0, center1;
        float time0, time1;
        float radius;
        Material* material_ptr;

        MovingSphere() { }
        MovingSphere(Vec3 cen0, Vec3 cen1, float t0, float t1, 
                     float radius, Material* material_ptr) : 
            center0(cen0), 
            center1(cen1), 
            time0(t0), 
            time1(t1), 
            radius(radius), 
            material_ptr(material_ptr) { }
        
        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;

        Vec3 center(float time) const;
};

bool MovingSphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    // Sphere equation, with center C:
    //   <p(t) - C, p(t) - C> = R*R
    //   -> <A + t*B - C, A + t*B - C> = R*R
    //   -> t*t*<B, B> + 2*t*<B, A - C> + <A - C, A - C> - R*R = 0
    //
    // The sphere is hit by the ray if there is a value of t,
    // with t_min < t < t_max, such that p(t) satisfies the equation

    Vec3 o_c = r.origin() - center(r.time()); // A - C
    float a = dot(r.direction(), r.direction()); // <B, B>
    float b = dot(r.direction(), o_c); // <B, A - C>
    float c = dot(o_c, o_c) - radius*radius; // <A - C, A - C> - R*R
    float discriminant = b*b - a*c;

    if (discriminant > 0.0) { // the ray hits the sphere
        // note: we didn't use 2 factors above since they'd cancel out below
        float t = (-b - sqrt(discriminant)) / a;
        if (t_min < t && t < t_max) {
            rec.t = t;
            rec.p = r.point_at_parameter(t);
            rec.surface_normal = (rec.p - center(r.time())) / radius; // versor
            rec.material_ptr = material_ptr;
            return true;
        }
        
        t = (-b + sqrt(discriminant)) / a;
        if (t_min < t && t < t_max) {
            rec.t = t;
            rec.p = r.point_at_parameter(t);
            rec.surface_normal = (rec.p - center(r.time())) / radius; // versor
            rec.material_ptr = material_ptr;
            return true;
        }
    }
    
    // note: we consider that if a ray tangentiates the sphere it does not hit it
    return false;
}

Vec3 MovingSphere::center(float time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

#endif
