#include <iostream>
#include "Ray.hh"

using namespace std;

bool hit_sphere(const Vec3& center, float radius, const Ray& r);
Vec3 visible_color(const Ray& r);

int main() {
    int nx = 200;
    int ny = 100;
    
    cout << "P3\n" << nx << " " << ny << "\n255\n";

    Vec3 lower_left_corner(-2.0, -1.0, -1.0);
    Vec3 horizontal(4.0, 0.0, 0.0);
    Vec3 vertical(0.0, 2.0, 0.0);
    Vec3 origin(0.0, 0.0, 0.0);

    for (int j = ny-1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            float u = float(i) / float(nx); // increases from left to right
            float v = float(j) / float(ny); // decreases from top to bottom
            
            Vec3 direction = lower_left_corner + u*horizontal + v*vertical;
            Ray r(origin, direction);
            
            Vec3 color = visible_color(r); // RGB values from 0.0 to 1.0

            // mapping [0.0, 1.0] to [0, 255]
            int ir = int(255.99*color.r());
            int ig = int(255.99*color.g());
            int ib = int(255.99*color.b());
            cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}

bool hit_sphere(const Vec3& center, float radius, const Ray& r) {
    Vec3 o_c = r.origin() - center; // A - C
    float a = dot(r.direction(), r.direction()); // <B, B>
    float b = 2.0 * dot(r.direction(), o_c); // <B, A - C>
    float c = dot(o_c, o_c) - radius*radius; // <A - C, A - C> - R*R
    // Sphere equation, with center C:
    //   <p(t) - C, p(t) - C> = R*R
    //   -> <A + t*B - C, A + t*B - C> = R*R
    //   -> t*t*<B, B> + 2*t*<B, A - C> + <A - C, A - C> - R*R = 0
    float discriminant = b*b - 4*a*c;
    return discriminant > 0;
}

Vec3 visible_color(const Ray& r) {
    if (hit_sphere(Vec3(0, 0, -1), 0.5, r)) {
        return Vec3(1, 0, 0); // color the sphere RED
    }

    Vec3 unit_direction = unit_vector(r.direction());
    // mapping [-1.0, 1.0] to [0.0, 1.0]
    float t = 0.5 * (unit_direction.y() + 1.0);
    
    // blends white and blue based on the ray's y coordinate
    return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}
