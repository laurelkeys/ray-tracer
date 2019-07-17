#include <iostream>
#include "float.h"
#include "Camera.hh"
#include "Sphere.hh"
#include "HittableList.hh"
#include "Random.hh"

using namespace std;

Hittable* random_scene();
Vec3 visible_color(const Ray& r, Hittable* world, int depth);

int MAX_DEPTH = 50; // maximum amount of calculated ray reflections

int main() {
    // int nx = 200;
    // int ny = 100;
    // int ns = 100; // number of samples per pixel
    int nx = 1200;
    int ny = 800;
    int ns = 10;
    
    cout << "P3\n" << nx << " " << ny << "\n255\n";

    /*
    int number_of_objects = 5;
    Hittable* objects[number_of_objects];
    objects[0] = new Sphere(Vec3( 0.0,    0.0, -1.0),   0.5, new Lambertian(Vec3(0.8, 0.3, 0.3)));
    objects[1] = new Sphere(Vec3( 0.0, -100.5, -1.0), 100.0, new Lambertian(Vec3(0.8, 0.8, 0.0)));
    objects[2] = new Sphere(Vec3( 1.0,    0.0, -1.0),   0.5, new Metal(Vec3(0.8, 0.6, 0.2)));
    // note: the following objects simulate a hollow sphere with 0.05 thickness (0.5 - 0.45)
    objects[3] = new Sphere(Vec3(-1.0,    0.0, -1.0),   0.5, new Dielectric(1.5));
    objects[4] = new Sphere(Vec3(-1.0,    0.0, -1.0), -0.45, new Dielectric(1.5));
    Hittable* world = new HittableList(objects, number_of_objects);
    */
    Hittable* world = random_scene();

    Vec3 look_from(13.0, 2.0, 3.0);
    Vec3 look_at(0.0, 0.0, 0.0);

    float vfov = 20.0;
    float lens_aperture = 0.1;
    float dist_to_focus = 10.0;
    
    Camera cam(lens_aperture, dist_to_focus, look_from, look_at, Vec3(0.0, 1.0, 0.0), vfov, float(nx) / float(ny));
    for (int j = ny-1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            Vec3 color(0.0, 0.0, 0.0);
            for (int s = 0; s < ns; ++s) {
                // pixel sampling for antialiasing
                float u = float(i + Random::number_in_01inc_1exc()) / float(nx);
                float v = float(j + Random::number_in_01inc_1exc()) / float(ny);
                Ray r = cam.get_ray(u, v);
                Vec3 p = r.point_at_parameter(2.0);
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

Vec3 visible_color(const Ray& r, Hittable* world, int depth) {
    HitRecord rec;
    float t_min = 0.001; // decrease "shadow acne"
    if (world->hit(r, t_min, FLT_MAX, rec)) {
        Ray r_scattered;
        Vec3 attenuation;
        if (depth < MAX_DEPTH && rec.material_ptr->scatter(r, rec, attenuation, r_scattered)) {
            // increases depth to limit the amount of calculated reflections
            return attenuation * visible_color(r_scattered, world, depth+1);
        }
        return Vec3(0.0, 0.0, 0.0); // BLACK
    }
    
    Vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    // blends white and blue based on the ray's y coordinate
    return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}

Hittable* random_scene() {
    int n = 500;
    Hittable** objects = new Hittable*[n+1];
    objects[0] =  new Sphere(Vec3(0.0, -1000.0, 0.0), 1000.0, new Lambertian(Vec3(0.5, 0.5, 0.5)));

    int i = 1;
    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            float choose_mat = Random::number_in_01inc_1exc();
            Vec3 center(a + 0.9 * Random::number_in_01inc_1exc(), 
                        0.2, 
                        b + 0.9 * Random::number_in_01inc_1exc());
            
            if ((center - Vec3(4.0, 0.2, 0.0)).length() > 0.9) {
                Material* material_ptr;

                if (choose_mat < 0.8) {
                    material_ptr = new Lambertian(
                        // albedo
                        Vec3(Random::number_in_01inc_1exc() * Random::number_in_01inc_1exc(), 
                             Random::number_in_01inc_1exc() * Random::number_in_01inc_1exc(), 
                             Random::number_in_01inc_1exc() * Random::number_in_01inc_1exc()));
                } else if (choose_mat < 0.95) {
                    material_ptr = new Metal(
                        // albedo
                        Vec3(0.5 * (1.0 + Random::number_in_01inc_1exc()), 
                             0.5 * (1.0 + Random::number_in_01inc_1exc()), 
                             0.5 * (1.0 + Random::number_in_01inc_1exc())), 
                        // fuzz
                        0.5 * Random::number_in_01inc_1exc());
                } else {
                    material_ptr = new Dielectric(
                        // refractive_index
                        1.5);
                }

                objects[i++] = new Sphere(center, 0.2, material_ptr);
            }
        }
    }

    objects[i++] = new Sphere(Vec3( 0.0, 1.0, 0.0), 1.0, new Dielectric(1.5));
    objects[i++] = new Sphere(Vec3(-4.0, 1.0, 0.0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1)));
    objects[i++] = new Sphere(Vec3( 4.0, 1.0, 0.0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));

    return new HittableList(objects, i);
}
