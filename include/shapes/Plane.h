#pragma once
#include "geometry/Ray.h"
#include "math/Real.h"
#include "math/Vector3.h"

#include "shapes/Shape3.h"

class Plane : public Shape3{
    
public:
    bool intersect(cg::Ray3f&, float&) override;
    cg::vec3f normal(cg::Ray3f&, float) override;
};

inline cg::vec3f
Plane::normal(cg::Ray3f&, float){
    return cg::vec3f{0.f, 1.f, 0.f};
}

inline bool
Plane::intersect(cg::Ray3f& ray, float& distance){
    
    if(cg::math::isZero(ray.direction.y))
        return false;


    float t = -ray.origin.y * cg::math::inverse(ray.direction.y);

    if(!cg::math::isPositive(t))
        return false;

    float px = ray.origin.x + (t * ray.direction.x);
    float pz = ray.origin.z + (t * ray.direction.z);

    // Checagem se a interseção está contida no corte finito do plano
    
    if(cg::math::abs(px) > 1.5f || cg::math::abs(pz) > 1.5f)
        return false;

    distance = t;
    
    return true;
}