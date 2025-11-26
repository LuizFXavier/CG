#pragma once
#include "core/SharedObject.h"
#include "geometry/Ray.h"
class Actor; //Foward declaration para evitar inclusão circular

struct Intersection{
    float distance;
    cg::Reference<Actor> actor;
    cg::Ray3f local_ray;
};