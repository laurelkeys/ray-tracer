#include <iostream>
#include "Vec3.hh"

using namespace std;

int main() {
    int nx = 200;
    int ny = 100;
    
    cout << "P3\n" << nx << " " << ny << "\n255\n";

    for (int j = ny-1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            // RGB values from 0.0 to 1.0
            Vec3 color(float(i) / float(nx), // RED increases from left to right
                       float(j) / float(ny), // GREEN decreases from top to bottom
                       0.2);                 // BLUE is fixed at 20%
            // mapping [0.0, 1.0] to [0, 255]
            int ir = int(255.99*color.r());
            int ig = int(255.99*color.g());
            int ib = int(255.99*color.b());
            cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}