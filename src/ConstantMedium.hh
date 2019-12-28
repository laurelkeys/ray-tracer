#ifndef CONSTANTMEDIUMHH
#define CONSTANTMEDIUMHH

#include <float.h>

#include "Ray.hh"
#include "Random.hh"
#include "Texture.hh"
#include "Hittable.hh"
#include "Material.hh"

class ConstantMedium : public Hittable {
    public:
        Hittable *boundary;
        Material *phase_function;
        float density; // C
        // note: the probability that a ray scatters in a small distance δL
        //       when passing through a constant volume is equal to (C ⋅ δL)

        ConstantMedium(Hittable *b, float d, Texture *a) :
            boundary(b),
            density(d) {
            phase_function = new Isotropic(a);
        }
        
        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
        
        virtual bool bounding_box(float t0, float t1, AABB& box) const {
            return boundary->bounding_box(t0, t1, box);
        }
};

bool ConstantMedium::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    // Print occasional samples when debugging. To enable, set enableDebug true.
    const bool enableDebug = true;
    bool debugging = enableDebug && Random::number_ge_0_lt_1() < 0.00001;

    HitRecord rec1, rec2;
    if (boundary->hit(r, -FLT_MAX, FLT_MAX, rec1)) {
        if (boundary->hit(r, rec1.t + 0.0001, FLT_MAX, rec2)) {
            if (debugging) std::cerr << "\nt0 t1 " << rec1.t << " " << rec2.t << '\n';

            if (rec1.t < t_min)
                rec1.t = t_min;

            if (rec2.t > t_max)
                rec2.t = t_max;

            if (rec1.t >= rec2.t)
                return false;

            if (rec1.t < 0)
                rec1.t = 0;

            float distance_inside_boundary = (rec2.t - rec1.t) * r.direction().length();
            float hit_distance = -(1/density) * log(Random::number_ge_0_lt_1());

            if (hit_distance < distance_inside_boundary) {
                rec.t = rec1.t + hit_distance / r.direction().length();
                rec.p = r.point_at_parameter(rec.t);

                if (debugging) {
                    std::cerr << "hit_distance = " <<  hit_distance << '\n'
                              << "rec.t = " <<  rec.t << '\n'
                              << "rec.p = " <<  rec.p << '\n';
                }

                rec.surface_normal = Vec3(1, 0, 0); // arbitrary
                rec.material_ptr = phase_function;
                return true;
            }
        }
    }
    return false;
}

#endif