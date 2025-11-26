#pragma once
#include <cmath>
#include <iostream>

#include "math/Vector3.h"
#include "math/Real.h"

#include "Shape3.h"

class Sphere : public Shape3{
    
public:
    bool intersect(cg::Ray3f&, float&) override;
    cg::vec3f normal(cg::Ray3f&, float) override;
};

inline cg::vec3f
Sphere::normal(cg::Ray3f& ray, float d){

    return ray(d);
}

inline bool
Sphere::intersect(cg::Ray3f& ray, float& distance){

    cg::vec3f& o = ray.origin;
    cg::vec3f& D = ray.direction;

    // o.print("O: ");
    // D.print("D: ");
    
    float b = o.dot(D);
    float c = o.dot(o) - 1;

    float delta = b * b - c;

    if(cg::math::isNegative(delta)){
        // std::cout << "delta: " << delta << "\n";
        return false;
    }

    float delta_sqrt = std::sqrt(delta);

    distance = (-b - delta_sqrt);

    if(!cg::math::isPositive(distance)){
        
        distance = (-b + delta_sqrt);

        if(!cg::math::isPositive(distance)){
            // std::cout << "Errou: " << -b << " + " << delta_sqrt << " = " << distance << ", " << c << "\n";
            return false;
        }
    }
    // std::cout << "Hitou: " << distance << "\n";
    return true;

}
