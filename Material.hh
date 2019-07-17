#ifndef MATERIALHH
#define MATERIALHH

#include "Hittable.hh"
#include "RandomNumber.hh"

Vec3 random_point_in_unit_sphere();
Vec3 reflect(const Vec3& v, const Vec3& surface_normal);
bool refract(const Vec3& v, const Vec3& surface_normal, float ni_over_nt, Vec3& refracted);
float schlicks_approximation(float cos_theta, float refractive_index);

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

        Lambertian(const Vec3& albedo) : 
            albedo(albedo) { }

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
        float fuzz;

        Metal(const Vec3& albedo, float fuzz = 0.0) : 
            albedo(albedo),
            fuzz(fuzz >= 1.0 ? 1.0 : fuzz <= 0.0 ? 0.0 : fuzz) { }

        virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& r_scattered) const {
            Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.surface_normal);
            r_scattered = Ray(rec.p, reflected + fuzz * random_point_in_unit_sphere());
            attenuation = albedo;
            return dot(r_scattered.direction(), rec.surface_normal) > 0.0;
        };
};

// defines a clear material such as water, glass, and diamonds
// obs.: a dielectric sphere with negative radius will make the surface normal 
//       point inward, so it can be used as a bubble to make a hollow glass sphere
class Dielectric : public Material {
    public:
        float refractive_index;

        Dielectric(float refractive_index) : 
            refractive_index(refractive_index) { }
        
        virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& r_scattered) const {
            Vec3 outward_normal;
            Vec3 reflected = reflect(r_in.direction(), rec.surface_normal);
            
            // attenuation is always 1.0, so the surface absorbs nothing
            // note: attenuation.z() == 0.0 so it kills the blue channel from the background
            attenuation = Vec3(1.0, 1.0, 1.0);

            float ni_over_nt;
            float cosine;
            if (dot(r_in.direction(), rec.surface_normal) > 0.0) {
                outward_normal = -rec.surface_normal;
                ni_over_nt = refractive_index;
                cosine = refractive_index * dot(r_in.direction(), rec.surface_normal) / r_in.direction().length();
            } else {
                outward_normal = rec.surface_normal;
                ni_over_nt = 1.0 / refractive_index;
                cosine = -dot(r_in.direction(), rec.surface_normal) / r_in.direction().length();
            }

            Vec3 refracted;
            float reflect_prob;
            if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
                reflect_prob = schlicks_approximation(cosine, refractive_index);
            } else {
                reflect_prob = 1.0;
                r_scattered = Ray(rec.p, reflected);
                return true;
            }

            if (RandomNumber::in_01inc_1exc() < reflect_prob) {
                r_scattered = Ray(rec.p, reflected);   
            } else {
                r_scattered = Ray(rec.p, refracted);
            }
            return true;
        }
};

// ref.: https://en.wikipedia.org/wiki/Schlick%27s_approximation, https://www.dorian-iten.com/fresnel/
float schlicks_approximation(float cos_theta, float refractive_index) {
    // considers a refractive index of 1.0 for the dielectric media
    float r0 = (1.0 - refractive_index) / (1.0 + refractive_index);
    r0 = r0*r0;
    return r0 + (1.0 - r0) * pow((1.0 - cos_theta), 5);
}

bool refract(const Vec3& v, const Vec3& surface_normal, float ni_over_nt, Vec3& refracted) {
    Vec3 unit_v = unit_vector(v);
    float dt = dot(unit_v, surface_normal); // projection of v onto N
    float discriminant = 1.0 - ni_over_nt*ni_over_nt * (1.0 - dt*dt);
    if (discriminant > 0.0) {
        refracted = ni_over_nt * (unit_v - surface_normal * dt) - surface_normal * sqrt(discriminant);
        return true;
    }

    // note: if a ray tangentiates the material (discriminant == 0.0) it does not refract
    return false;
}

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
