#pragma once

#include "Ray.h"
#include "Color.h"

#include "RayModel.h"

class RayCaster : public RayModel{

private:
    cg::Color shoot(float x, float y) override;
    bool intersect(Intersection&) override;
    cg::Color shade(cg::Ray3f&, Intersection&) override;

    cg::Color trace(cg::Ray3f&);
    
public:
    void render() override;
};


