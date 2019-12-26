#ifndef SCENEHH
#define SCENEHH

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Vec3.hh"
#include "Random.hh"
#include "Texture.hh"
#include "Material.hh"
#include "Hittable.hh"
#include "HittableList.hh"
#include "BVHNode.hh"
#include "Sphere.hh"
#include "MovingSphere.hh"
#include "AARect.hh"
#include "Box.hh"

namespace Scene {
    Hittable* cornell_box();
    Hittable* simple_light();
    Hittable* earth();
    Hittable* three_spheres();
    Hittable* two_perlin_spheres();
    Hittable* two_spheres();
    Hittable* random_scene();
    Hittable* wikipedia_scene();
}

Hittable* Scene::cornell_box() {
    Hittable **list = new Hittable*[8];
    
    Material *red   = new Lambertian(new ConstantTexture(Vec3(0.65, 0.05, 0.05)));
    Material *white = new Lambertian(new ConstantTexture(Vec3(0.73, 0.73, 0.73)));
    Material *green = new Lambertian(new ConstantTexture(Vec3(0.12, 0.45, 0.15)));
    Material *light = new DiffuseLight(new ConstantTexture(Vec3(15, 15, 15)));

    int i = 0;
    list[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, green));
    list[i++] = new YZRect(0, 555, 0, 555, 0, red);
    list[i++] = new XZRect(213, 343, 227, 332, 554, light);
    list[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
    list[i++] = new XZRect(0, 555, 0, 555, 0, white);
    list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));
    list[i++] = new Box(Vec3(130, 0,  65), Vec3(295, 165, 230), white);
    list[i++] = new Box(Vec3(265, 0, 295), Vec3(430, 330, 460), white);
    return new HittableList(list, i);
}

Hittable* Scene::simple_light() {
    Texture *perlin_texture = new NoiseTexture(4);
    Hittable **list = new Hittable*[4];
    list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(perlin_texture));
    list[1] = new Sphere(Vec3(0, 2, 0), 2, new Lambertian(perlin_texture));
    list[2] = new Sphere(Vec3(0, 7, 0), 2, 
                         new DiffuseLight(new ConstantTexture(Vec3(4, 4, 4))));
    list[3] = new XYRect(3, 5, 1, 3, -2, // plane z(t) = -2
                         new DiffuseLight(new ConstantTexture(Vec3(4, 4, 4))));
    return new HittableList(list, 4);
}

Hittable* Scene::earth() {
    int nx, ny, nn;
    unsigned char *texture_pixels = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
    Material *material_ptr = new Lambertian(new ImageTexture(texture_pixels, nx, ny));
    return new Sphere(Vec3(0, 0, 0), 2, material_ptr);
}

Hittable* Scene::three_spheres() {
    int n = 5;
    Hittable **objects = new Hittable*[n];
    objects[0] = new Sphere(Vec3( 0.0,    0.0, -1.0),   0.5, new Lambertian(new ConstantTexture(Vec3(0.8, 0.3, 0.3))));
    objects[1] = new Sphere(Vec3( 0.0, -100.5, -1.0), 100.0, new Lambertian(new ConstantTexture(Vec3(0.8, 0.8, 0.0))));
    objects[2] = new Sphere(Vec3( 1.0,    0.0, -1.0),   0.5, new Metal(Vec3(0.8, 0.6, 0.2)));
    // note: the following objects simulate a hollow sphere with 0.05 thickness (0.5 - 0.45)
    objects[3] = new Sphere(Vec3(-1.0,    0.0, -1.0),   0.5, new Dielectric(1.5));
    objects[4] = new Sphere(Vec3(-1.0,    0.0, -1.0), -0.45, new Dielectric(1.5));
    return new HittableList(objects, n);
}

Hittable* Scene::two_perlin_spheres() {
    Texture *perlin_texture = new NoiseTexture(3.5);
    Hittable **list = new Hittable*[2];
    list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(perlin_texture));
    list[1] = new Sphere(Vec3(0, 2, 0), 2, new Lambertian(perlin_texture));
    return new HittableList(list, 2);
}

Hittable* Scene::two_spheres() {
    Texture *checker = new CheckerTexture(new ConstantTexture(Vec3(0.2, 0.3, 0.1)),
                                          new ConstantTexture(Vec3(0.9, 0.9, 0.9)));
    Hittable **list = new Hittable*[2];
    list[0] = new Sphere(Vec3(0,-10, 0), 10, new Lambertian(checker));
    list[1] = new Sphere(Vec3(0, 10, 0), 10, new Lambertian(checker));
    return new HittableList(list, 2);
}

Hittable* Scene::random_scene() {
    int n = 500;
    Hittable **objects = new Hittable*[n+1];
    
    Texture *checker = new CheckerTexture(new ConstantTexture(Vec3(0.2, 0.3, 0.1)),
                                          new ConstantTexture(Vec3(0.9, 0.9, 0.9)));
    objects[0] = new Sphere(Vec3(0.0, -1000.0, 0.0), 1000.0, new Lambertian(checker));

    int i = 1;
    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            float choose_mat = Random::number_ge_0_lt_1();
            Vec3 center(a + 0.9 * Random::number_ge_0_lt_1(), 
                        0.2, 
                        b + 0.9 * Random::number_ge_0_lt_1());
            
            if ((center - Vec3(4.0, 0.2, 0.0)).length() > 0.9) {
                Material* material_ptr;

                if (choose_mat < 0.8) {
                    material_ptr = new Lambertian(new ConstantTexture(
                        // albedo
                        Vec3(Random::number_ge_0_lt_1() * Random::number_ge_0_lt_1(), 
                             Random::number_ge_0_lt_1() * Random::number_ge_0_lt_1(), 
                             Random::number_ge_0_lt_1() * Random::number_ge_0_lt_1())));
                } else if (choose_mat < 0.95) {
                    material_ptr = new Metal(
                        // albedo
                        Vec3(0.5 * (1.0 + Random::number_ge_0_lt_1()), 
                             0.5 * (1.0 + Random::number_ge_0_lt_1()), 
                             0.5 * (1.0 + Random::number_ge_0_lt_1())), 
                        // fuzz
                        0.5 * Random::number_ge_0_lt_1());
                } else { // glass
                    material_ptr = new Dielectric(
                        // refractive_index
                        1.5);
                }

                if (choose_mat < 0.8) { // diffuse
                    Vec3 end_pos = center + Vec3(0.0, 0.5 * Random::number_ge_0_lt_1(), 0.0);
                    objects[i++] = new MovingSphere(center, end_pos, 0.0, 1.0, 0.2, material_ptr);
                } else { // metal or glass
                    objects[i++] = new Sphere(center, 0.2, material_ptr);
                }
            }
        }
    }

    objects[i++] = new Sphere(Vec3( 0.0, 1.0, 0.0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));
    objects[i++] = new Sphere(Vec3(-4.0, 1.0, 0.0), 1.0, new Lambertian(new ConstantTexture(Vec3(0.4, 0.2, 0.1))));
    objects[i++] = new Sphere(Vec3( 4.0, 1.0, 0.0), 1.0, new Dielectric(1.5));

    // return new HittableList(objects, i);
    return new BVHNode(objects, i, 0.0, 1.0);
}

Hittable* Scene::wikipedia_scene() {
    // attempt to build the scene of the first image on Wikipedia's "Ray tracing (graphics)" page
    // ref.: https://en.wikipedia.org/wiki/File:Recursive_raytrace_of_a_sphere.png

    Vec3 white(0.9, 0.9, 0.9);
    Vec3 salmon(0.99, 0.71, 0.65);
    Vec3 yellow(1.0, 0.99, 0.74);
    Vec3 yellow2(0.99, 0.89, 0.6);
    Vec3 pink(1.0, 0.72, 0.92);
    Vec3 pink2(0.94, 0.5, 0.55);
    Vec3 pink3(1.0, 0.59, 0.74);
    Vec3 beige(1.0, 0.87, 0.82);

    int number_of_objects = 9;
    Hittable** objects = new Hittable*[number_of_objects];
    
    float r = 1.0;
    // objects[0] = new Sphere(Vec3(0.0, -1000.0 - r, 0.0), 1000.0, new Lambertian(new ConstantTexture(white)));
    // objects[0] = new Sphere(Vec3(0.0, -1000.0 - r, 0.0), 1000.0, new Metal(white));
    objects[0] = new Sphere(Vec3(0.0, -1000.0 - r, 0.0), 1000.0, new Dielectric(1.3));
    objects[1] = new Sphere(Vec3(0.0, 0.0, 0.0), r, new Lambertian(new ConstantTexture(salmon)));
    objects[2] = new Sphere(Vec3(0.3-r, -0.7*r, 1.8*r), 0.3*r, new Lambertian(new ConstantTexture(yellow)));
    objects[3] = new Sphere(Vec3(1.7*r, -0.4*r, r), 0.6*r, new Lambertian(new ConstantTexture(beige)));
    objects[4] = new Sphere(Vec3(-1.8*r, -0.3*r, r), 0.7*r, new Lambertian(new ConstantTexture(pink2)));
    objects[5] = new Sphere(Vec3(3.1*r, 0.6*r, -r), 1.6*r, new Lambertian(new ConstantTexture(pink)));
    objects[6] = new Sphere(Vec3(-3.8*r, 0.6*r, -r), 1.6*r, new Lambertian(new ConstantTexture(pink3)));
    objects[7] = new Sphere(Vec3(-1.8*r, -0.5*r, -1.4*r), 0.5*r, new Lambertian(new ConstantTexture(yellow2)));
    objects[8] = new Sphere(Vec3(1.5*r, -0.1*r, -1.7*r), 0.9*r, new Metal(white));
    // objects[8] = new Sphere(Vec3(1.5*r, -0.1*r, -1.7*r), 0.9*r, new Dielectric(1.3));

    return new HittableList(objects, number_of_objects);
}

#endif