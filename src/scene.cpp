#include "scene.hpp"

bool Scene::hit(const Ray& r, double t_min, double t_max, Intersection& rec) const
{
    Intersection tmp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for(const auto& model : models) {
        if (model->hit(r, t_min, closest_so_far, tmp_rec)) {
            hit_anything = true;
            closest_so_far = tmp_rec.distance;
            rec = tmp_rec;
        }
    }

    return hit_anything;
}

bool Scene::make_bounding_box(float time0, float time1, AABB& output_box) const
{
    if(models.empty())
        return false;

    AABB tmp_box;
    bool first_box = true;

    for(const auto& model : models)
    {
        // Check if there's a possible bounding box for the model
        if(!model->make_bounding_box(time0, time1, tmp_box))
            return false;
        // if its the first box, output_box is just that AABB, otherwise
        // compute the surrounding_box of output_box and tmp_box
        output_box = first_box ? tmp_box : surrounding_box(output_box, tmp_box);
        first_box = false;
    }

    return true;
}