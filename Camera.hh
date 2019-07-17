#ifndef CAMERAHH
#define CAMERAHH

#include "Ray.hh"

class Camera {
    public:
        Vec3 lower_left_corner; // (width/-2, height/-2, depth)
        Vec3 horizontal; // width
        Vec3 vertical; // height
        Vec3 origin;

        // vfov: vertical field of view (top to bottom in degrees)
        // aspect: aspect ratio (width / height)
        Camera(Vec3 look_from, Vec3 look_at, Vec3 view_up, float vfov, float aspect, bool in_degrees = true) {
            float theta = in_degrees ? vfov * M_PI / 180 : vfov; // converts degrees to radians
            float half_height = tan(theta / 2);
            float half_width = aspect * half_height;

            origin = look_from;
            Vec3 w = unit_vector(look_from - look_at); // the camera faces -w
            
            // note: u, v, and view_up are coplanar
            Vec3 u = unit_vector(cross(view_up, w));
            Vec3 v = cross(w, u);

            lower_left_corner = Vec3(-half_width, -half_height, -1.0);
            lower_left_corner = origin - half_width * u - half_height * v - w;

            horizontal = 2.0 * half_width * u;
            vertical = 2.0 * half_height * v;
        }
        
        Ray get_ray(float u, float v) {
            // u, v in [0.0, 1.0]
            return Ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
        }
};

#endif
