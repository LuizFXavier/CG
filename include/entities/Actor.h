#pragma once

#include <iostream>

#include "graphics/TransformableObject.h"
#include "core/NameableObject.h"
#include "graphics/Material.h"
#include "geometry/Ray.h"
#include "math/Vector3.h"
#include "math/Matrix3x3.h"

#include "shapes/Shape3.h"
#include "shapes/Intersection.h"

// TODO: Aparentemente o raio de pixel está sendo cr

class Actor : public cg::TransformableObject{
protected:
    cg::Reference<Shape3> _shape;
    cg::Reference<cg::Material> _material;
    float _m; // Índice de "metalicidade"
    float _r; // Rugosidade do material
    
    
    cg::mat3f _normalMatrix;
    
public:

    Actor(cg::Reference<Shape3> shape, cg::Reference<cg::Material>& material, const char* name = "Actor"):
    _shape(shape), _material(material){}

    void setTransform(const cg::vec3f&, const cg::quatf&, const cg::vec3f&);
    const cg::Reference<cg::Material> material() const{ return _material;}
    bool intersect(cg::Ray3f&);
    bool intersect(cg::Ray3f&, Intersection& hit);
    cg::vec3f normal(Intersection& hit);
};

inline cg::vec3f 
Actor::normal(Intersection& hit){

    return _normalMatrix.transform(_shape->normal(hit.local_ray, hit.distance));
}

inline bool
Actor::intersect(cg::Ray3f& ray, Intersection& hit){
    
    cg::vec3f local_origin = worldToLocalMatrix().transform(ray.origin);
    cg::vec3f local_direction = worldToLocalMatrix().transformVector(ray.direction);

    cg::Ray3f local_ray{local_origin, local_direction};

    // localToWorldMatrix().print("L: ");
    // worldToLocalMatrix().print("W: ");

    cg::vec3f center = localToWorldMatrix().transform(cg::vec3f{0.f, 0.f, 0.f});

    float distance;
    // std::cout << "--------\n";
    // ray.origin.print("WO: ");
    // ray.direction.print("WD: ");
    // center.print("C: ");
    // local_ray.origin.print("O: ");
    // local_ray.direction.print("D: ");
    // exit(1);

    if(_shape->intersect(local_ray, distance)){
        hit.actor = cg::Reference<Actor>(this);
        hit.distance = distance;
        hit.local_ray = local_ray;

        // cg::vec3f v = ray.direction * distance + ray.origin;
        // cg::vec3f& o = ray.origin;
        // cg::vec3f& D = ray.direction;
        // std::cout<<"-----------------\n";
        // std::cout << "t: " << distance <<"\n";
        // o.print("O: ");
        // D.print("D: ");
        // v.print("P: ");

        // std::cout << "R";
        return true;
    }
    // std::cout << "F";
    return false;
}

inline bool
Actor::intersect(cg::Ray3f& ray){
    
    cg::vec3f local_origin = worldToLocalMatrix().transform3x4(ray.origin);
    cg::vec3f local_direction = worldToLocalMatrix().transformVector(ray.direction);

    cg::Ray3f local_ray{local_origin, local_direction};

    float distance;

    if(_shape->intersect(local_ray, distance)){
        return true;
    }

    return false;
}

inline void
Actor::setTransform(const cg::vec3f& p, const cg::quatf& r, const cg::vec3f& s){
    cg::TransformableObject::setTransform(p, r, s);
    
    if(cg::math::isEqual(s.x, s.y) && cg::math::isEqual(s.x, s.z)){
        _normalMatrix = cg::mat3f{_localToWorld}.transposed();
    }
    else{
        _normalMatrix = cg::mat3f{_localToWorld};
        _normalMatrix.invert();
        _normalMatrix.transpose();
    }
}