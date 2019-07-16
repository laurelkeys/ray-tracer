#ifndef LAMBERTIANHH
#define LAMBERTIANHH

#include "Material.hh"

// defines a matte material (a diffusely reflecting surface)
class Lambertian : public Material {
    public:
        Vec3 albedo; // "whiteness", the measure of diffuse reflection from 0 to 1:
                     // 0: black body that absorbs all incident radiation
                     // 1: a body that reflects all incident radiation

        Lambertian(const Vec3& albedo) : albedo(albedo) { }

        virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& r_scattered) const {
            
            float reflectance = 0.5;
            // simulating a matte object reflection by choosing a 
            // random point in the unit sphere with center p(t) + N
            Vec3 target = rec.p + rec.surface_normal + random_point_in_unit_sphere();
            return reflectance * visible_color(Ray(rec.p, target - rec.p), world);
        };
};

#endif
