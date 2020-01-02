#ifndef TRANSFORMATIONSHH
#define TRANSFORMATIONSHH

#include "AABB.hh"
#include "Constants.hh"
#include "Hittable.hh"
#include "Ray.hh"
#include "Vec3.hh"

class FlipNormals : public Hittable {
    public:
        Hittable* ptr;

        FlipNormals(Hittable* p) :
            ptr(p) { }

        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
            if (ptr->hit(r, t_min, t_max, rec)) {
                rec.surface_normal = -rec.surface_normal;
                return true;
            }
            return false;
        }

        virtual bool bounding_box(float t0, float t1, AABB& box) const {
            return ptr->bounding_box(t0, t1, box);
        }
};

class Translate : public Hittable {
    public:
        Hittable* ptr;
        Vec3 offset;

        Translate(Hittable* p, const Vec3& displacement) :
            ptr(p),
            offset(displacement) { }

        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
        virtual bool bounding_box(float t0, float t1, AABB& box) const;
};

bool Translate::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    Ray moved_r(r.origin() - offset, r.direction(), r.time());
    if (ptr->hit(moved_r, t_min, t_max, rec)) {
        rec.p += offset;
        return true;
    }
    return false;
}

bool Translate::bounding_box(float t0, float t1, AABB& box) const {
    if (ptr->bounding_box(t0, t1, box)) {
        box = AABB(box.min() + offset, box.max() + offset);
        return true;
    }
    return false;
}

class RotateY : public Hittable {
    public:
        Hittable* ptr;
        AABB bbox;
        float sin_theta;
        float cos_theta;
        bool has_bbox;

        RotateY(Hittable* p, float angle);

        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;

        virtual bool bounding_box(float t0, float t1, AABB& box) const {
            box = bbox;
            return has_bbox;
        }
};

RotateY::RotateY(Hittable* p, float angle_in_degrees) :
    ptr(p) {
    float radians = (_PI_ / 180.0) * angle_in_degrees;
    sin_theta = sin(radians);
    cos_theta = cos(radians);

    has_bbox = ptr->bounding_box(0, 1, bbox);

    Vec3 min(_INFINITY_, _INFINITY_, _INFINITY_);
    Vec3 max(-_INFINITY_, -_INFINITY_, -_INFINITY_);
    for (int i = 0; i <= 1; ++i)
        for (int j = 0; j <= 1; ++j)
            for (int k = 0; k <= 1; ++k) {
                float x = i * bbox.max().x() + (1 - i) * bbox.min().x();
                float y = j * bbox.max().y() + (1 - j) * bbox.min().y();
                float z = k * bbox.max().z() + (1 - k) * bbox.min().z();
                float new_x = cos_theta * x + sin_theta * z;
                float new_z = -sin_theta * x + cos_theta * z;
                Vec3 tester(new_x, y, new_z);
                for (int c = 0; c < 3; ++c) {
                    if (tester[c] > max[c])
                        max[c] = tester[c];
                    if (tester[c] < min[c])
                        min[c] = tester[c];
                }
            }
    bbox = AABB(min, max);
}

bool RotateY::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    Vec3 origin = r.origin();
    Vec3 direction = r.direction();
    origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
    origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];
    direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
    direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

    Ray rotated_r(origin, direction, r.time());
    if (ptr->hit(rotated_r, t_min, t_max, rec)) {
        Vec3 p = rec.p;
        p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
        p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];
        rec.p = p;

        Vec3 n = rec.surface_normal;
        n[0] = cos_theta * rec.surface_normal[0] + sin_theta * rec.surface_normal[2];
        n[2] = -sin_theta * rec.surface_normal[0] + cos_theta * rec.surface_normal[2];
        rec.surface_normal = n;

        return true;
    }
    return false;
}

#endif