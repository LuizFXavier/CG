#include <cmath>
#include <iostream>

#include "graphics/RayCast.h"
void 
RayCaster::render(){
    
    int iW = _imageBuffer.width(), iH = _imageBuffer.height();
    
    std::cout << "Render chamado";
    
    for(int j = 0; j < iH; ++j){
        
        auto y = (float)j + 0.5f;
        
        for(int i = 0; i < iW; ++i){
            
            auto x = (float)i + 0.5f;
            
            cg::Color color = shoot(x, y);

            _imageBuffer(i, j) = color;
        }
    }

    _image->setData(_imageBuffer);
}



cg::Color
RayCaster::shoot(float x, float y){
    setPixelRay(x, y);

    cg::Color color = trace(_pixelRay);
    
    // adjust RGB color
    if (color.r > 1.0f)
        color.r = 1.0f;
    if (color.g > 1.0f)
        color.g = 1.0f;
    if (color.b > 1.0f)
        color.b = 1.0f;

    if (!cg::math::isPositive(color.r))
        color.r = 0.f;
    if (!cg::math::isPositive(color.g))
        color.g = 0.f;
    if (!cg::math::isPositive(color.b))
        color.b = 0.f;
    

    return color;
}

cg::Color 
RayCaster::trace(cg::Ray3f& ray){
    Intersection hit;

    return intersect(ray, hit) ? shade(ray, hit) : background();
}

bool
RayCaster::intersect(cg::Ray3f& ray, Intersection& hit){
    Intersection closestHit;
    closestHit.distance = ray.tMax;
    bool found = false;

    for (auto& actor : _scene->actors()){
        if(actor->intersect(ray, hit)){
            // std::cout << "Ator intersectado! Distância: " << hit.distance << std::endl;
            if(hit.distance <= closestHit.distance){
                closestHit = hit;
                found = true;
            }
        }
    }
    
    hit = closestHit;
    return found;
}
bool
RayCaster::intersect(cg::Ray3f& ray){

    Intersection hit;

    for (auto& actor : _scene->actors()){
        if(actor->intersect(ray, hit)){
            if(hit.distance <= ray.tMax){
                return true;
            }
        }
    }

    return false;
}

// Espslon necessário para evitar colisão consigo mesmo
inline constexpr auto
rt_eps(){
  return 1e-4f;
}


cg::Color 
RayCaster::shade(cg::Ray3f& ray, Intersection& hit){

    // Inicia com a iluminação ambiente
    auto finalColor = hit.actor->material()->ambient * _scene->ambientLight();
    
    cg::vec3f N = hit.actor->normal(hit);
    
    cg::Color& Od = hit.actor->material()->diffuse;
    cg::Color& Os = hit.actor->material()->specular;
    float Ns = hit.actor->material()->shine;
    
    cg::vec3f P = ray(hit.distance);
    cg::vec3f& V = ray.direction;
    
    for(auto& light : _scene->lights()){
        
        cg::vec3f L;
        float d;
        
        if (!light.lightVector(P, L, d))
            continue;
        
        L = -L;
        float NL = N.dot(L);
        
        if(NL > 0)
            continue;

        cg::vec3f R = L - (2 * (NL)) * N;

        float RV = R.dot(V);
        
        cg::Ray3f lightRay = cg::Ray3f{P - L * rt_eps(), light.position() - P};

        if(shadow(lightRay))
            continue;
        
        cg::Color Il = light.lightColor(d);

        if (Il.r > 1.0f)
            Il.r = 1.0f;
        if (Il.g > 1.0f)
            Il.g = 1.0f;
        if (Il.b > 1.0f)
            Il.b = 1.0f;

        // std::cout << "Hto a luz\n";

        finalColor += (Od * Il * (-NL)) + (Os * Il) * pow(-RV, Ns);
    }

    return finalColor;
}
