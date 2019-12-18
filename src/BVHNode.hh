#ifndef BVHNODEHH
#define BVHNODEHH

#include "AABB.hh"
#include "Random.hh"
#include "Hittable.hh"

class BVHNode : public Hittable { // Bounding Volume Hierarchy
    public:
        AABB box;
        Hittable *left;
        Hittable *right;

        BVHNode() {}
        BVHNode(Hittable **list, int n, float time0, float time1);

        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
        virtual bool bounding_box(float t0, float t1, AABB& box) const;
};

int box_x_compare(const void *a, const void *b) {
    AABB box_left, box_right;
    Hittable *ah = *(Hittable**) a;
    Hittable *bh = *(Hittable**) b;
    if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
        std::cerr << "No bounding box in BVHNode constructor\n";
    return (box_left.min().x() - box_right.min().x() < 0.0) ? -1 : 1;
}

int box_y_compare(const void *a, const void *b) {
    AABB box_left, box_right;
    Hittable *ah = *(Hittable**) a;
    Hittable *bh = *(Hittable**) b;
    if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
        std::cerr << "No bounding box in BVHNode constructor\n";
    return (box_left.min().y() - box_right.min().y() < 0.0) ? -1 : 1;
}

int box_z_compare(const void *a, const void *b) {
    AABB box_left, box_right;
    Hittable *ah = *(Hittable**) a;
    Hittable *bh = *(Hittable**) b;
    if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
        std::cerr << "No bounding box in BVHNode constructor\n";
    return (box_left.min().z() - box_right.min().z() < 0.0) ? -1 : 1;
}

BVHNode::BVHNode(Hittable **list, int n, float time0, float time1) {
    int axis = int(3 * Random::number_ge_0_lt_1());

    if (axis == 0)
       qsort(list, n, sizeof(Hittable *), box_x_compare);
    else if (axis == 1)
       qsort(list, n, sizeof(Hittable *), box_y_compare);
    else
       qsort(list, n, sizeof(Hittable *), box_z_compare);

    if (n == 1) {
        left = right = list[0];
    } else if (n == 2) {
        left = list[0];
        right = list[1];
    } else {
        left = new BVHNode(list, n / 2, time0, time1);
        right = new BVHNode(list + n / 2, n - n / 2, time0, time1);
    }

    AABB box_left, box_right;
    if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
        std::cerr << "No bounding box in BVHNode constructor\n";
    box = surrounding_box(box_left, box_right);
}

bool BVHNode::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    if (box.hit(r, t_min, t_max)) {
        HitRecord left_rec, right_rec;
        bool hit_left = left->hit(r, t_min, t_max, left_rec);
        bool hit_right = right->hit(r, t_min, t_max, right_rec);
        
        if (hit_left && hit_right) {
            rec = (left_rec.t < right_rec.t) ? left_rec : right_rec;
            return true;
        } else if (hit_left) {
            rec = left_rec;
            return true;
        } else if (hit_right) {
            rec = right_rec;
            return true;
        }
    }

    return false;
}

bool BVHNode::bounding_box(float t0, float t1, AABB& b) const {
    b = box;
    return true;
}

#endif