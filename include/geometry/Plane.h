#pragma once
#include "Ray.h"
#include "Real.h"
#include "Vector3.h"

#include "Shape3.h"

class Plane : public Shape3{
    
public:
    bool intersect(cg::Ray3f&, float&) override;
    cg::vec3f normal(cg::Ray3f&) override;
};

inline cg::vec3f
Plane::normal(cg::Ray3f&){
    return cg::vec3f{0.f, 1.f, 0.f};
}

inline bool
Plane::intersect(cg::Ray3f& ray, float& distance){
    
    if(cg::math::isZero(ray.direction.y))
        return false;

    distance = -ray.origin.y / ray.direction.y;

    cg::vec3f point = ray.origin + ray.direction * distance; // Ponto de interseção com o plano

    // Checagem se a interseção está contida no corte finito do plano
    
    if(cg::math::abs(point.x) > 1 || cg::math::abs(point.z) > 1)
        return false;
    
    return true;
}