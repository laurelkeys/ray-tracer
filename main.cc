#include <iostream>
#include <random>
#include "float.h"
#include "Camera.hh"
#include "Sphere.hh"
#include "HittableList.hh"

using namespace std;

Vec3 visible_color(const Ray& r, Hittable* world);

int main() {
    random_device rd;
    mt19937 e2(rd()); // engine
    uniform_real_distribution<> dist(0.0, 1.0); // distribution \in [0.0, 1.0)
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
                // obs.: 0.0 <= dist(e2) < 1.0
                float u = float(i + dist(e2)) / float(nx);
                float v = float(j + dist(e2)) / float(ny);
                Ray r = cam.get_ray(u, v);
                Vec3 p = r.point_at_parameter(2.0);
                color += visible_color(r, world);
            }
            color /= ns; // averages the sampled colors

            // mapping [0.0, 1.0] to [0, 255]
            int ir = int(255.99*color.r());
            int ig = int(255.99*color.g());
            int ib = int(255.99*color.b());
            cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}

Vec3 visible_color(const Ray& r, Hittable* world) {
    HitRecord rec;
    if (world->hit(r, 0.0, FLT_MAX, rec)) {
        // mapping [-1.0, 1.0] to [0.0, 1.0]
        return 0.5 * (rec.surface_normal + Vec3(1.0, 1.0, 1.0)); // RGB values from 0.0 to 1.0
    }
    
    Vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    // blends white and blue based on the ray's y coordinate
    return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}
