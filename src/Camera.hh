#ifndef CAMERAHH
#define CAMERAHH

#include "Random.hh"
#include "Ray.hh"
#include "Vec3.hh"

class Camera {
  public:
    float time0, time1;
    float lens_radius;

    Vec3 u, v, w; // orthonormal basis ("width", "height", "depth")

    Vec3 lower_left_corner; // (width/-2, height/-2, depth)
    Vec3 horizontal; // width
    Vec3 vertical; // height
    Vec3 origin;

    Camera(float aperture, float focus_dist, // used to simulate the camera's DOF
           Vec3 look_from, Vec3 look_at, Vec3 view_up,
           float vfov, float aspect,
           float t0, float t1,
           bool in_degrees = true) {
        time0 = t0;
        time1 = t1;

        float theta = in_degrees ? vfov * M_PI / 180 : vfov; // converts degrees to radians
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height; // obs.: aspect ratio = width / height

        origin = look_from;
        // note: u, v, and view_up are coplanar, and
        //       u, v, and w form an orthonormal basis
        w = unit_vector(look_from - look_at); // the camera faces -w
        u = unit_vector(cross(view_up, w));
        v = cross(w, u);

        lower_left_corner = origin - focus_dist * (half_width * u + half_height * v + w);

        horizontal = 2.0 * half_width * focus_dist * u;
        vertical = 2.0 * half_height * focus_dist * v;

        lens_radius = aperture / 2;
        // note: 0.0 aperture and focus_dist 1.0 simulate the basic "ideal" camera
    }

    Ray get_ray(float s, float t) {
        // s, t \in [0.0, 1.0]
        Vec3 rd = lens_radius * Random::point_in_unit_disk(); // random point in the camera lens
        Vec3 offset = u * rd.x() + v * rd.y();
        float time = time0 + Random::number_ge_0_lt_1() * (time1 - time0);

        // note: u and v are orthogonal to the camera's facing direction (which is parallel to w)
        return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - (origin + offset), time);
    }
};

#endif