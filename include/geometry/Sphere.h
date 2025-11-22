#pragma once
#include <cmath>

#include "Vector3.h"
#include "Real.h"

#include "Shape3.h"

class Sphere : public Shape3{
    
public:
    bool intersect(cg::Ray3f&, float&) override;
    cg::vec3f normal(cg::Ray3f&) override;
};

inline cg::vec3f
Sphere::normal(cg::Ray3f& ray){

    return ray.origin.versor();
}

inline bool
Sphere::intersect(cg::Ray3f& ray, float& distance){

    cg::vec3f& o = ray.origin;
    cg::vec3f& D = ray.direction;

    float b = 2 * o.dot(D);
    float c = o.dot(o) - 1;

    float delta = b * b - c;

    if(cg::math::isNegative(delta))
        return false;

    float delta_sqrt = std::sqrt(delta);

    distance = -b - delta_sqrt;

    if(!cg::math::isPositive(distance)){
        
        distance = -b + delta_sqrt;

        if(!cg::math::isPositive(distance))
            return false;
    }
    
    return true;

}
