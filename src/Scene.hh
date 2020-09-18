#ifndef SCENEHH
#define SCENEHH

#define STB_IMAGE_IMPLEMENTATION
#include "inc/stb_image.h"

#include "AARect.hh"
#include "BVHNode.hh"
#include "Box.hh"
#include "ConstantMedium.hh"
#include "Hittable.hh"
#include "HittableList.hh"
#include "Material.hh"
#include "MovingSphere.hh"
#include "Random.hh"
#include "Sphere.hh"
#include "Texture.hh"
#include "Transformations.hh"
#include "Vec3.hh"

namespace Scene {
    Hittable* demo();
    Hittable* final();
    Hittable* cornell_glass_balls();
    Hittable* cornell_balls();
    Hittable* cornell_smoke();
    Hittable* cornell_box();
    Hittable* simple_light();
    Hittable* earth();
    Hittable* three_spheres();
    Hittable* two_perlin_spheres();
    Hittable* two_spheres();
    Hittable* random_scene();
    Hittable* wikipedia_scene();
    Hittable* wikipedia_scene_sss();
}

Hittable* Scene::demo() {
    Hittable** list = new Hittable*[9];

    Material* red = new Lambertian(new ConstantTexture(Vec3(0.65, 0.05, 0.05)));
    Material* white = new Lambertian(new ConstantTexture(Vec3(0.73, 0.73, 0.73)));
    Material* green = new Lambertian(new ConstantTexture(Vec3(0.12, 0.45, 0.15)));
    Material* light = new DiffuseLight(new ConstantTexture(Vec3(7, 7, 7)));

    int i = 0;
    list[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, green));
    list[i++] = new YZRect(0, 555, 0, 555, 0, red);
    list[i++] = new XZRect(113, 443, 127, 432, 554, light);
    list[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
    list[i++] = new XZRect(0, 555, 0, 555, 0, white);
    list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));

    Hittable* b1 = new Translate(
                    new RotateY(new Box(Vec3(0, 0, 0), Vec3(165, 165, 165), white), -18),
                    Vec3(130, 0, 65)
                   );
    Hittable* b2 = new Translate(
                    new RotateY(new Box(Vec3(0, 0, 0), Vec3(165, 330, 165), white), 15),
                    Vec3(265, 0, 295)
                   );
    Hittable* s1 = new Translate(
                    new RotateY(new Sphere(Vec3(0, 0, 0), 80, white), 15),
                    Vec3(180, 280, 190)
                   );

    list[i++] = new ConstantMedium(b1, 0.01, new ConstantTexture(Vec3(1.0, 1.0, 1.0)));
    list[i++] = new ConstantMedium(b2, 0.01, new ConstantTexture(Vec3(0.0, 0.0, 0.0)));
    list[i++] = new ConstantMedium(s1, 0.1, new ConstantTexture(Vec3(0.4, 0.4, 0.4)));

    return new HittableList(list, i);
}

Hittable* Scene::final() {
    int nb = 20;
    Hittable** list = new Hittable*[30];
    Hittable** boxlist = new Hittable*[10000];
    Hittable** boxlist2 = new Hittable*[10000];
    Material* white = new Lambertian(new ConstantTexture(Vec3(0.73, 0.73, 0.73)));
    Material* ground = new Lambertian(new ConstantTexture(Vec3(0.48, 0.83, 0.53)));
    int b = 0;
    for (int i = 0; i < nb; ++i) {
        for (int j = 0; j < nb; ++j) {
            float w = 100;
            float x0 = -1000 + i * w;
            float z0 = -1000 + j * w;
            float y0 = 0;
            float x1 = x0 + w;
            float y1 = 100 * (Random::number_ge_0_lt_1() + 0.01);
            float z1 = z0 + w;
            boxlist[b++] = new Box(Vec3(x0, y0, z0), Vec3(x1, y1, z1), ground);
        }
    }

    int l = 0;
    list[l++] = new BVHNode(boxlist, b, 0, 1);

    Material* light = new DiffuseLight(new ConstantTexture(Vec3(7, 7, 7)));
    list[l++] = new XZRect(123, 423, 147, 412, 554, light);

    Vec3 center(400, 400, 200);
    list[l++] = new MovingSphere(center, center + Vec3(30, 0, 0), 0, 1, 50,
                                 new Lambertian(new ConstantTexture(Vec3(0.7, 0.3, 0.1))));
    list[l++] = new Sphere(Vec3(260, 150, 45), 50, new Dielectric(1.5));
    list[l++] = new Sphere(Vec3(0, 150, 145), 50, new Metal(Vec3(0.8, 0.8, 0.9), 10.0));

    Hittable* boundary = new Sphere(Vec3(360, 150, 145), 70, new Dielectric(1.5));
    list[l++] = boundary;
    list[l++] = new ConstantMedium(boundary, 0.2, new ConstantTexture(Vec3(0.2, 0.4, 0.9)));

    boundary = new Sphere(Vec3(0, 0, 0), 5000, new Dielectric(1.5));
    list[l++] = new ConstantMedium(boundary, 0.0001, new ConstantTexture(Vec3(1.0, 1.0, 1.0)));

    int nx, ny, nn;
    unsigned char* texture_pixels = stbi_load("res/earthmap.jpg", &nx, &ny, &nn, 0);
    Material* emat = new Lambertian(new ImageTexture(texture_pixels, nx, ny));
    list[l++] = new Sphere(Vec3(400, 200, 400), 100, emat);

    Texture* pertext = new NoiseTexture(0.1);
    list[l++] = new Sphere(Vec3(220, 280, 300), 80, new Lambertian(pertext));

    int ns = 1000;
    for (int j = 0; j < ns; ++j) {
        boxlist2[j] = new Sphere(Vec3(165 * Random::number_ge_0_lt_1(),
                                      165 * Random::number_ge_0_lt_1(),
                                      165 * Random::number_ge_0_lt_1()),
                                 10, white);
    }
    list[l++] = new Translate(
                    new RotateY(new BVHNode(boxlist2, ns, 0.0, 1.0), 15),
                    Vec3(-100, 270, 395)
                );

    return new HittableList(list, l);
}

Hittable* Scene::cornell_glass_balls() {
    Hittable** list = new Hittable*[8];

    Material* red = new Lambertian(new ConstantTexture(Vec3(0.65, 0.05, 0.05)));
    Material* white = new Lambertian(new ConstantTexture(Vec3(0.73, 0.73, 0.73)));
    Material* blue = new Lambertian(new ConstantTexture(Vec3(0.3, 0.65, 0.8)));
    Material* light = new DiffuseLight(new ConstantTexture(Vec3(7, 7, 7)));

    int i = 0;
    list[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, red));
    list[i++] = new YZRect(0, 555, 0, 555, 0, blue);
    list[i++] = new XZRect(113, 443, 127, 432, 554, light);
    list[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
    list[i++] = new XZRect(0, 555, 0, 555, 0, white);
    list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));

    // note: a volume inside a dielectric is a subsurface material
    list[i++] = new Sphere(Vec3(160, 100, 145), 100, new Dielectric(1.5));
    list[i++] = new Translate(new Sphere(Vec3(165, 100, 125), 100, new Dielectric(1.5)), Vec3(265, 0, 295));
    return new HittableList(list, i);
}

Hittable* Scene::cornell_balls() {
    Hittable** list = new Hittable*[9];

    Material* red = new Lambertian(new ConstantTexture(Vec3(0.65, 0.05, 0.05)));
    Material* white = new Lambertian(new ConstantTexture(Vec3(0.73, 0.73, 0.73)));
    Material* green = new Lambertian(new ConstantTexture(Vec3(0.12, 0.45, 0.15)));
    Material* light = new DiffuseLight(new ConstantTexture(Vec3(7, 7, 7)));

    int i = 0;
    list[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, green));
    list[i++] = new YZRect(0, 555, 0, 555, 0, red);
    list[i++] = new XZRect(113, 443, 127, 432, 554, light);
    list[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
    list[i++] = new XZRect(0, 555, 0, 555, 0, white);
    list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));

    // note: a volume inside a dielectric is a subsurface material
    Hittable* boundary = new Sphere(Vec3(160, 100, 145), 100, new Dielectric(1.5));
    list[i++] = boundary;
    list[i++] = new ConstantMedium(boundary, 0.1, new ConstantTexture(Vec3(1, 1, 1)));
    list[i++] = new Translate(new RotateY(new Box(Vec3(0, 0, 0), Vec3(165, 330, 165), white), 15), Vec3(265, 0, 295));
    return new HittableList(list, i);
}

Hittable* Scene::cornell_smoke() {
    Hittable** list = new Hittable*[8];

    Material* red = new Lambertian(new ConstantTexture(Vec3(0.65, 0.05, 0.05)));
    Material* white = new Lambertian(new ConstantTexture(Vec3(0.73, 0.73, 0.73)));
    Material* green = new Lambertian(new ConstantTexture(Vec3(0.12, 0.45, 0.15)));
    Material* light = new DiffuseLight(new ConstantTexture(Vec3(7, 7, 7)));

    int i = 0;
    list[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, green));
    list[i++] = new YZRect(0, 555, 0, 555, 0, red);
    list[i++] = new XZRect(113, 443, 127, 432, 554, light);
    list[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
    list[i++] = new XZRect(0, 555, 0, 555, 0, white);
    list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));

    Hittable* b1 = new Translate(
                    new RotateY(new Box(Vec3(0, 0, 0), Vec3(165, 165, 165), white), -18),
                    Vec3(130, 0, 65)
                   );
    Hittable* b2 = new Translate(
                    new RotateY(new Box(Vec3(0, 0, 0), Vec3(165, 330, 165), white), 15),
                    Vec3(265, 0, 295)
                   );

    list[i++] = new ConstantMedium(b1, 0.01, new ConstantTexture(Vec3(1.0, 1.0, 1.0)));
    list[i++] = new ConstantMedium(b2, 0.01, new ConstantTexture(Vec3(0.0, 0.0, 0.0)));

    return new HittableList(list, i);
}

Hittable* Scene::cornell_box() {
    Hittable** list = new Hittable*[8];

    Material* red = new Lambertian(new ConstantTexture(Vec3(0.65, 0.05, 0.05)));
    Material* white = new Lambertian(new ConstantTexture(Vec3(0.73, 0.73, 0.73)));
    Material* green = new Lambertian(new ConstantTexture(Vec3(0.12, 0.45, 0.15)));
    Material* light = new DiffuseLight(new ConstantTexture(Vec3(15, 15, 15)));

    int i = 0;
    list[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, green));
    list[i++] = new YZRect(0, 555, 0, 555, 0, red);
    list[i++] = new XZRect(213, 343, 227, 332, 554, light);
    list[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
    list[i++] = new XZRect(0, 555, 0, 555, 0, white);
    list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));
    list[i++] = new Translate(
                    new RotateY(new Box(Vec3(0, 0, 0), Vec3(165, 165, 165), white),
                                -18), // rotation
                    Vec3(130, 0, 65)  // translation
                );
    list[i++] = new Translate(
                    new RotateY(new Box(Vec3(0, 0, 0), Vec3(165, 330, 165), white),
                                15),  // rotation
                    Vec3(265, 0, 295) // translation
                );
    return new HittableList(list, i);
}

Hittable* Scene::simple_light() {
    Texture* perlin_texture = new NoiseTexture(4);
    Hittable** list = new Hittable*[4];
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
    unsigned char* texture_pixels = stbi_load("res/earthmap.jpg", &nx, &ny, &nn, 0);
    Material* material_ptr = new Lambertian(new ImageTexture(texture_pixels, nx, ny));
    return new Sphere(Vec3(0, 0, 0), 2, material_ptr);
}

Hittable* Scene::three_spheres() {
    int n = 5;
    Hittable** objects = new Hittable*[n];
    objects[0] = new Sphere(Vec3(0.0, 0.0, -1.0), 0.5, new Lambertian(new ConstantTexture(Vec3(0.8, 0.3, 0.3))));
    objects[1] = new Sphere(Vec3(0.0, -100.5, -1.0), 100.0, new Lambertian(new ConstantTexture(Vec3(0.8, 0.8, 0.0))));
    objects[2] = new Sphere(Vec3(1.0, 0.0, -1.0), 0.5, new Metal(Vec3(0.8, 0.6, 0.2)));
    // note: the following objects simulate a hollow sphere with 0.05 thickness (0.5 - 0.45)
    objects[3] = new Sphere(Vec3(-1.0, 0.0, -1.0), 0.5, new Dielectric(1.5));
    objects[4] = new Sphere(Vec3(-1.0, 0.0, -1.0), -0.45, new Dielectric(1.5));
    return new HittableList(objects, n);
}

Hittable* Scene::two_perlin_spheres() {
    Texture* perlin_texture = new NoiseTexture(3.5);
    Hittable** list = new Hittable*[2];
    list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(perlin_texture));
    list[1] = new Sphere(Vec3(0, 2, 0), 2, new Lambertian(perlin_texture));
    return new HittableList(list, 2);
}

Hittable* Scene::two_spheres() {
    Texture* checker = new CheckerTexture(new ConstantTexture(Vec3(0.2, 0.3, 0.1)),
                                          new ConstantTexture(Vec3(0.9, 0.9, 0.9)));
    Hittable** list = new Hittable*[2];
    list[0] = new Sphere(Vec3(0, -10, 0), 10, new Lambertian(checker));
    list[1] = new Sphere(Vec3(0, 10, 0), 10, new Lambertian(checker));
    return new HittableList(list, 2);
}

Hittable* Scene::random_scene() {
    int n = 500;
    Hittable** objects = new Hittable*[n + 1];

    Texture* checker = new CheckerTexture(new ConstantTexture(Vec3(0.2, 0.3, 0.1)),
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

    objects[i++] = new Sphere(Vec3(0.0, 1.0, 0.0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));
    objects[i++] = new Sphere(Vec3(-4.0, 1.0, 0.0), 1.0, new Lambertian(new ConstantTexture(Vec3(0.4, 0.2, 0.1))));
    objects[i++] = new Sphere(Vec3(4.0, 1.0, 0.0), 1.0, new Dielectric(1.5));

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
    objects[2] = new Sphere(Vec3(0.3 - r, -0.7 * r, 1.8 * r), 0.3 * r, new Lambertian(new ConstantTexture(yellow)));
    objects[3] = new Sphere(Vec3(1.7 * r, -0.4 * r, r), 0.6 * r, new Lambertian(new ConstantTexture(beige)));
    objects[4] = new Sphere(Vec3(-1.8 * r, -0.3 * r, r), 0.7 * r, new Lambertian(new ConstantTexture(pink2)));
    objects[5] = new Sphere(Vec3(3.1 * r, 0.6 * r, -r), 1.6 * r, new Lambertian(new ConstantTexture(pink)));
    objects[6] = new Sphere(Vec3(-3.8 * r, 0.6 * r, -r), 1.6 * r, new Lambertian(new ConstantTexture(pink3)));
    objects[7] = new Sphere(Vec3(-1.8 * r, -0.5 * r, -1.4 * r), 0.5 * r, new Lambertian(new ConstantTexture(yellow2)));
    objects[8] = new Sphere(Vec3(1.5 * r, -0.1 * r, -1.7 * r), 0.9 * r, new Metal(white));
    // objects[8] = new Sphere(Vec3(1.5*r, -0.1*r, -1.7*r), 0.9*r, new Dielectric(1.3));

    return new HittableList(objects, number_of_objects);
}

Hittable* Scene::wikipedia_scene_sss() {
    // note: using a volume inside of a dielectric for subsurface reflection
    //       (sss - subsurface scattering)
    Vec3 white(0.9, 0.9, 0.9);
    Vec3 salmon(0.99, 0.71, 0.65);
    Vec3 yellow(1.0, 0.99, 0.74);
    Vec3 yellow2(0.99, 0.89, 0.6);
    Vec3 pink(1.0, 0.72, 0.92);
    Vec3 pink2(0.94, 0.5, 0.55);
    Vec3 pink3(1.0, 0.59, 0.74);
    Vec3 beige(1.0, 0.87, 0.82);

    int number_of_objects = 17;
    Hittable** objects = new Hittable*[number_of_objects];

    int n = 0;
    float r = 1.0;
    float optical_density = 0.8;
    float refractive_index = 1.5;
    Hittable* boundary;
    
    // objects[n++] = new Sphere(Vec3(0.0, -1000.0 - r, 0.0), 1000.0, new Lambertian(new ConstantTexture(white)));
    // objects[n++] = new Sphere(Vec3(0.0, -1000.0 - r, 0.0), 1000.0, new Metal(white));
    // objects[n++] = new Sphere(Vec3(0.0, -1000.0 - r, 0.0), 1000.0, new Dielectric(1.3));
    
    boundary = new Sphere(Vec3(0.0, -1000.0 - r, 0.0), 1000.0, new Dielectric(refractive_index));
    objects[n++] = boundary;
    objects[n++] = new ConstantMedium(boundary, 40.0, new ConstantTexture(white));

    boundary = new Sphere(Vec3(0.0, 0.0, 0.0), r, new Dielectric(refractive_index));
    objects[n++] = boundary;
    objects[n++] = new ConstantMedium(boundary, 0.9, new ConstantTexture(salmon));
    
    boundary = new Sphere(Vec3(0.3 - r, -0.7 * r, 1.8 * r), 0.3 * r, new Dielectric(refractive_index));
    objects[n++] = boundary;
    objects[n++] = new ConstantMedium(boundary, 3.2, new ConstantTexture(yellow));
    boundary = new Sphere(Vec3(1.7 * r, -0.4 * r, r), 0.6 * r, new Dielectric(refractive_index));
    objects[n++] = boundary;
    objects[n++] = new ConstantMedium(boundary, 1.5, new ConstantTexture(beige));
    boundary = new Sphere(Vec3(-1.8 * r, -0.3 * r, r), 0.7 * r, new Dielectric(refractive_index));
    objects[n++] = boundary;
    objects[n++] = new ConstantMedium(boundary, 1.4, new ConstantTexture(pink2));
    boundary = new Sphere(Vec3(3.1 * r, 0.6 * r, -r), 1.6 * r, new Dielectric(refractive_index));
    objects[n++] = boundary;
    objects[n++] = new ConstantMedium(boundary, 0.6, new ConstantTexture(pink));
    boundary = new Sphere(Vec3(-3.8 * r, 0.6 * r, -r), 1.6 * r, new Dielectric(refractive_index));
    objects[n++] = boundary;
    objects[n++] = new ConstantMedium(boundary, optical_density, new ConstantTexture(pink3));
    boundary = new Sphere(Vec3(-1.8 * r, -0.5 * r, -1.4 * r), 0.5 * r, new Dielectric(refractive_index));
    objects[n++] = boundary;
    objects[n++] = new ConstantMedium(boundary, 1.5, new ConstantTexture(yellow2));
    
    objects[n++] = new Sphere(Vec3(1.5 * r, -0.1 * r, -1.7 * r), 0.9 * r, new Metal(white));
    // objects[n++] = new Sphere(Vec3(1.5*r, -0.1*r, -1.7*r), 0.9*r, new Dielectric(1.3));

    return new HittableList(objects, number_of_objects);
}

#endif