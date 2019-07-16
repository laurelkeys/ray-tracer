#include <iostream>
#include <random>
#include "float.h"
#include "Camera.hh"
#include "Sphere.hh"
#include "HittableList.hh"

using namespace std;

Vec3 random_point_in_unit_sphere();
Vec3 visible_color(const Ray& r, Hittable* world);

// ref.: https://stackoverflow.com/questions/686353/random-float-number-generation/17798317#17798317
random_device rd;
mt19937 e2(rd()); // engine
uniform_real_distribution<> rand_dist(0.0, 1.0); // distribution \in [0.0, 1.0)

int main() {
    int nx = 200;
    int ny = 100;
    int ns = 100; // number of samples per pixel
    
    cout << "P3\n" << nx << " " << ny << "\n255\n";

    Hittable* objects[2];
    objects[0] = new Sphere(Vec3(0.0, 0.0, -1.0), 0.5);
    objects[1] = new Sphere(Vec3(0.0, -100.5, -1.0), 100.0);
    Hittable* world = new HittableList(objects, 2);

    Camera cam;
    for (int j = ny-1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            Vec3 color(0.0, 0.0, 0.0);
            for (int s = 0; s < ns; ++s) {
                // pixel sampling for antialiasing
                // obs.: 0.0 <= rand_dist(e2) < 1.0
                float u = float(i + rand_dist(e2)) / float(nx);
                float v = float(j + rand_dist(e2)) / float(ny);
                Ray r = cam.get_ray(u, v);
                Vec3 p = r.point_at_parameter(2.0);
                color += visible_color(r, world);
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

Vec3 random_point_in_unit_sphere() {
    Vec3 p;
    do {
        // we pick a random point in the unit cube with x, y, z \in [-1.0, 1.0]
        // and if it's outside the sphere we reject it and try again
        // obs.: 0.0 <= rand_dist(e2) < 1.0, so we map [0.0, 1.0) to [-1.0, 1.0)
        p = 2.0 * Vec3(rand_dist(e2), rand_dist(e2), rand_dist(e2)) - Vec3(1.0, 1.0, 1.0);
    } while (p.squared_length() >= 1.0);
    return p;
}

Vec3 visible_color(const Ray& r, Hittable* world) {
    HitRecord rec;
    float t_min = 0.001; // decrease "shadow acne"
    if (world->hit(r, t_min, FLT_MAX, rec)) {
        float reflectance = 0.5;
        // simulating a matte object reflection by choosing a 
        // random point in the unit sphere with center p(t) + N
        Vec3 target = rec.p + rec.surface_normal + random_point_in_unit_sphere();
        return reflectance * visible_color(Ray(rec.p, target - rec.p), world);
    }
    
    Vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    // blends white and blue based on the ray's y coordinate
    return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}
