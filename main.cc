#include <iostream>
#include "Ray.hh"

using namespace std;

Vec3 bg_color(const Ray& r);

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
            
            Vec3 color = bg_color(r); // RGB values from 0.0 to 1.0

            // mapping [0.0, 1.0] to [0, 255]
            int ir = int(255.99*color.r());
            int ig = int(255.99*color.g());
            int ib = int(255.99*color.b());
            cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}

Vec3 bg_color(const Ray& r) {
    Vec3 unit_direction = unit_vector(r.direction());
    // mapping [-1.0, 1.0] to [0.0, 1.0]
    float t = 0.5 * (unit_direction.y() + 1.0);
    
    // blends white and blue based on the ray's y coordinate
    return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}
