#include "RayCaster.h"

void 
RayCaster::render(){
    int iW = _imageBuffer.width(), iH = _imageBuffer.height();

    for(int j = 0; j < iH; ++j){

        auto y = (float)j + 0.5f;

        for(int i = 0; i < iW; ++i){
            
            auto x = (float)i + 0.5f;

            cg::Color color = shoot(x, y);
        }
    }
}

cg::Color
RayCaster::shoot(float x, float y){
    setPixelRay(x, y);
    
    return trace(_pixelRay);
}

cg::Color 
RayCaster::trace(cg::Ray3f& ray){
    Intersection hit;

    return intersect(hit) ? shade(ray, hit) : background();
}

bool
RayCaster::intersect(Intersection& hit){
}

// TODO: Coloração baseada na cena 
cg::Color 
RayCaster::shade(cg::Ray3f &, Intersection &){
    return cg::Color();
}
