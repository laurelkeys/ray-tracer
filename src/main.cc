#include <iostream>
#include <float.h>

#include "Vec3.hh"
#include "Ray.hh"
#include "Random.hh"
#include "Camera.hh"
#include "Hittable.hh"
#include "Scene.hh"

using namespace std;

Vec3 visible_color(const Ray& r, Hittable* world, int depth);

int MAX_DEPTH = 50; // maximum amount of calculated ray reflections

int main() {
    // int nx = 200;
    // int ny = 100;
    // int ns = 10; // number of samples per pixel
    int nx = 1200;
    int ny = 800;
    int ns = 220;
    
    cout << "P3\n" << nx << " " << ny << "\n255\n";

    Hittable* world = Scene::final();

    // Vec3 look_from(13.0, 2.0, 3.0);
    // Vec3 look_at(0.0, 0.0, 0.0);
    Vec3 look_from(378, 278, -700);
    Vec3 look_at(278, 278, 0);

    float vfov = 40.0;
    float lens_aperture = 0.0;
    float dist_to_focus = 10.0;
    
    Camera cam(lens_aperture, dist_to_focus, 
               look_from, look_at, Vec3(0, 1, 0), 
               vfov, float(nx) / float(ny), 0.0, 1.0);
    
    for (int j = ny-1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            Vec3 color(0.0, 0.0, 0.0);
            for (int s = 0; s < ns; ++s) {
                // pixel sampling for antialiasing
                float u = float(i + Random::number_ge_0_lt_1()) / float(nx);
                float v = float(j + Random::number_ge_0_lt_1()) / float(ny);
                Ray r = cam.get_ray(u, v);
                color += visible_color(r, world, 0);
            }
            color /= float(ns); // averages the sampled colors
            color = Vec3(sqrt(color.r()), sqrt(color.g()), sqrt(color.b())); // gamma 2

            // mapping [0.0, 1.0] to [0, 255]
            int ir = int(255.99*color.r());
            int ig = int(255.99*color.g());
            int ib = int(255.99*color.b());
            cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}

Vec3 sky_gradient(const Ray& r) {
    Vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    // blends white and blue based on the ray's y coordinate
    return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}

Vec3 visible_color(const Ray& r, Hittable* world, int depth) {
    HitRecord rec;    
    float t_min = 0.001; // decrease "shadow acne"
    if (world->hit(r, t_min, FLT_MAX, rec)) {
        Ray r_scattered;
        Vec3 attenuation;
        Vec3 emitted = rec.material_ptr->emitted(rec.u, rec.v, rec.p);
        if (depth < MAX_DEPTH && rec.material_ptr->scatter(r, rec, attenuation, r_scattered)) {
            // increases depth to limit the amount of calculated reflections
            return emitted + attenuation * visible_color(r_scattered, world, depth+1);
        }
        return emitted;
    }
    // return sky_gradient(r);
    return Vec3(0.0, 0.0, 0.0); // BLACK
}
