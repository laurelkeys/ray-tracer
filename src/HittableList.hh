#ifndef HITTABLELISTHH
#define HITTABLELISTHH

#include "Hittable.hh"

class HittableList : public Hittable {
    public:
        int list_size;
        Hittable** list;

        HittableList() { }
        HittableList(Hittable** list, int n) : 
            list(list), 
            list_size(n) { }

        virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
        virtual bool bounding_box(float t0, float t1, AABB& box) const;
};

bool HittableList::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    HitRecord temp_rec;
    bool hit_anything = false;
    double closest_hit_so_far = t_max;
    for (int i = 0; i < list_size; ++i) {
        if (list[i]->hit(r, t_min, closest_hit_so_far, temp_rec)) {
            hit_anything = true;
            closest_hit_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool HittableList::bounding_box(float t0, float t1, AABB& box) const {
    if (list_size < 1)
        return false; // doesn't have a bounding box
    
    AABB temp_box;
    bool first_true = list[0]->bounding_box(t0, t1, temp_box);
    if (!first_true) {
        return false;
    } else {
        box = temp_box;
    }

    for (int i = 1; i < list_size; ++i) {
        if (list[i]->bounding_box(t0, t1, temp_box)) {
            box = surrounding_box(box, temp_box);
        } else {
            return false;
        }
    }
    
    return true; // has a well-defined bounding box
}

#endif
