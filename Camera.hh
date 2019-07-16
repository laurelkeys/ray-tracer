#ifndef CAMERAHH
#define CAMERAHH

#include "Ray.hh"

class Camera {
    public:
        Vec3 lower_left_corner; // (width/-2, height/-2, depth)
        Vec3 horizontal; // width
        Vec3 vertical; // height
        Vec3 origin;

        Camera() : 
            lower_left_corner(Vec3(-2.0, -1.0, -1.0)), 
            horizontal(Vec3(4.0, 0.0, 0.0)), 
            vertical(Vec3(0.0, 2.0, 0.0)), 
            origin(Vec3(0.0, 0.0, 0.0)) { }
        
        Ray get_ray(float u, float v) {
            // u, v in [0.0, 1.0]
            return Ray(origin, lower_left_corner + u*horizontal + v*vertical);
        }
};

#endif
