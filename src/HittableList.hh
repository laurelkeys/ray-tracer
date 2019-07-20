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

#endif
