#pragma once
#include "core/SharedObject.h"
#include "geometry/Ray.h"
#include "math/Vector3.h"

class Shape3 : public virtual cg::SharedObject{
public:
    /// @brief Checagem de interseção entre um raio e o objeto.
    /// @param  ray Raio traçado.
    /// @param  distance Distância entre a origem do raio e a interseção. Indefinido caso a função retorne falso.
    /// @return True caso o objeto foi interceptado pelo raio e false caso o contrário. 
    virtual bool intersect(cg::Ray3f& ray, float& distance) = 0;
    virtual cg::vec3f normal(cg::Ray3f& ray, float) = 0;
};