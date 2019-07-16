#ifndef MATERIALHH
#define MATERIALHH

#include "Hittable.hh"
#include "RandomNumber.hh"

Vec3 random_point_in_unit_sphere();
Vec3 reflect(const Vec3& v, const Vec3& surface_normal);

class Material {
    public:
        virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& r_scattered) const = 0;
        // note: the pure virtual function makes Material an abstract class, so it can't be instantiated,
        //       and a derived class that implements scatter() must be used (i.e. "= 0" makes overriding required)
};

// defines a matte material (a diffusely reflecting surface)
class Lambertian : public Material {
    public:
        Vec3 albedo; // "whiteness", the measure of diffuse reflection from 0 to 1:
                     // 0: black body that absorbs all incident radiation
                     // 1: a body that reflects all incident radiation

        Lambertian(const Vec3& albedo) : albedo(albedo) { }

        virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& r_scattered) const {
            // simulating a matte object reflection by choosing a 
            // random point in the unit sphere with center p(t) + N
            Vec3 target = rec.p + rec.surface_normal + random_point_in_unit_sphere();
            r_scattered = Ray(rec.p, target - rec.p);
            attenuation = albedo;
            return true;
            // note: we could also only scatter with some probability P, and have attenuation = (1/P)*albedo
        };
};

class Metal : public Material {
    public:
        Vec3 albedo; // "whiteness", the measure of diffuse reflection from 0 to 1:
                     // 0: black body that absorbs all incident radiation
                     // 1: a body that reflects all incident radiation

        Metal(const Vec3& albedo) : albedo(albedo) { }

        virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& r_scattered) const {
            Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.surface_normal);
            r_scattered = Ray(rec.p, reflected);
            attenuation = albedo;
            return dot(r_scattered.direction(), rec.surface_normal) > 0.0;
        };
};

Vec3 reflect(const Vec3& v, const Vec3& surface_normal) {
    return v - 2.0 * dot(v, surface_normal) * surface_normal;
}

Vec3 random_point_in_unit_sphere() {
    Vec3 p;
    do {
        // we pick a random point in the unit cube with x, y, z \in [-1.0, 1.0]
        // and if it's outside the sphere we reject it and try again
        float px = RandomNumber::in_01inc_1exc();
        float py = RandomNumber::in_01inc_1exc();
        float pz = RandomNumber::in_01inc_1exc();
        // obs.: 0.0 <= px, py, pz < 1.0, so we map [0.0, 1.0) to [-1.0, 1.0)
        p = 2.0 * Vec3(px, py, pz) - Vec3(1.0, 1.0, 1.0);
    } while (p.squared_length() >= 1.0);
    return p;
}

#endif
