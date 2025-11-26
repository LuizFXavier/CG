#pragma once

#include "core/SharedObject.h"
#include "graphics/GLImage.h"
#include "graphics/Color.h"
#include "graphics/Camera.h"
#include "geometry/Ray.h"
#include "math/Vector3.h"

#include "shapes/Intersection.h"
#include "entities/Scene.h"

#include <iostream>

struct Viewport{
    float w;
    float h;
    float startX;
    float startY;
    float dx;
    float dy;
};

class RayCaster : public cg::SharedObject{
    
public:

    struct{
        cg::vec3f u;
        cg::vec3f v;
        cg::vec3f n;
    } _vrc;

    cg::Reference<cg::GLImage> _image;
    cg::Reference<cg::Camera> _camera;
    cg::ImageBuffer _imageBuffer;
    
    cg::Reference<Scene> _scene;

    cg::Ray3f _pixelRay;

    void setPixelRay(float x, float y);

    cg::vec3f globalPixel(float x, float y) const;

    cg::Color shoot(float x, float y);
    cg::Color trace(cg::Ray3f&);
    
    bool intersect(cg::Ray3f&, Intersection&); // Verifica se houve intersecção do raio com a cena.
    bool intersect(cg::Ray3f&); // Verifica se houve intersecção do raio com a cena.

    cg::Color shade(cg::Ray3f&, Intersection&); // Retorna a cor refletida pelo objeto.
    bool shadow(cg::Ray3f&);

public:
    Viewport _viewport;
    RayCaster(int imageWidth, int imageHeight):
    _imageBuffer(imageWidth, imageHeight){
        std::cout << "Ray caster feito\n";
        _image = new cg::GLImage(imageWidth, imageHeight);
        std::cout << "Imagem feito\n";
        _pixelRay.tMax = cg::math::Limits<float>().inf();
    }
    
    void setCamera(cg::Camera* camera);

    void setScene(cg::Reference<Scene> scene){
        this->_scene = scene;
    }

    void setViewport(int height, float ratio){
        
        auto iH = _imageBuffer.height(), iW = _imageBuffer.width();
        
        if(iW >= iH){
            _viewport.h = height;
            _viewport.w = _viewport.h * ratio;
        }
        else{
            _viewport.w = height;
            _viewport.h = _viewport.w * cg::math::inverse(ratio);
        }

        _viewport.dx = _viewport.w / _imageBuffer.width();
        _viewport.dy = _viewport.h / _imageBuffer.height();

        _viewport.startX = -_viewport.w * 0.5f;
        _viewport.startY = _viewport.h * 0.5f;

        std::cout << "Viewport:\n";
        std::cout << "w:" << _viewport.w << "\n";
        std::cout << "h:" << _viewport.h << "\n";
        std::cout << "sX:" << _viewport.startX << "\n";
        std::cout << "sY:" << _viewport.startY << "\n";
        std::cout << "dx:" << _viewport.dx << "\n";
        std::cout << "dy:" << _viewport.dy << "\n";
        // exit(1);
    }

    cg::Color background(){
        return _scene->backgroundColor();
    }

    void drawn(){
        _image->draw(0, 0);
    }
    
    void render();
};

// Retorna as coordenadas globais do pixel, no eixo da camera.
inline cg::vec3f 
RayCaster::globalPixel(float x, float y) const{

    
    float startX = _viewport.startX, startY = _viewport.startY;
    float dx = _viewport.dx, dy = _viewport.dy;
    
    return (startX + (x * dx)) * _vrc.u + (-startY + (y * dy)) * _vrc.v;

}


inline void
RayCaster::setCamera(cg::Camera* camera){
    
    if (camera != _camera.get()){
        (_camera = camera ? camera : new cg::Camera{})->update();
        const auto& m = _camera->cameraToWorldMatrix();
        
        // Inicialização das coordenadas do vrc para evitar recálculo no traçado de raios
        _vrc.u = m[0];
        _vrc.v = m[1]; 
        _vrc.n = m[2];

        _pixelRay.set(_camera->position(), -_vrc.n);
        setViewport(_camera->height(), _camera->aspectRatio());
    }
}

// Talvez a direção esteja errada, provavelmente no cálculo do ator
inline void 
RayCaster::setPixelRay(float x, float y){

    auto p = globalPixel(x, y);

    int centerX = _imageBuffer.width() / 2;
    int centerY = _imageBuffer.height() / 2;
    
     if((int)x == centerX && (int)y == centerY){
        std::cout << "=== DEBUG SISTEMA DE COORDENADAS ===\n";
        p.print("p (viewport): ");
        _camera->position().print("Camera position: ");
        std::cout << "Near plane: " << _camera->nearPlane() << "\n";
        
        _vrc.u.print("_vrc.u (direita): ");
        _vrc.v.print("_vrc.v (cima): ");
        _vrc.n.print("_vrc.n (frente/trás?): ");
        
        // Testar diferentes interpretações
        cg::vec3f point_forward = p + _camera->nearPlane() * _vrc.n;
        cg::vec3f point_backward = p - _camera->nearPlane() * _vrc.n;
        
        point_forward.print("Ponto se _vrc.n = FRENTE: ");
        point_backward.print("Ponto se _vrc.n = TRÁS: ");
        
        // Ver qual faz sentido
        cg::vec3f dir_forward = (point_forward - _camera->position()).versor();
        cg::vec3f dir_backward = (point_backward - _camera->position()).versor();
        
        dir_forward.print("Direção se FRENTE: ");
        dir_backward.print("Direção se TRÁS: ");
        
        cg::vec3f to_origin = (cg::vec3f(0,0,0) - _camera->position()).versor();
        to_origin.print("Direção esperada para origem: ");
        
        std::cout << "Dot FRENTE: " << dir_forward.dot(to_origin) << "\n";
        std::cout << "Dot TRÁS: " << dir_backward.dot(to_origin) << "\n";
    }

    switch (_camera->projectionType()){
        case cg::Camera::Perspective:
            _pixelRay.direction = (p - _camera->nearPlane() * _vrc.n).versor();
        break;

        case cg::Camera::Parallel:
            _pixelRay.origin = _camera->position() + p;
        break;
    }


}

inline bool
RayCaster::shadow(cg::Ray3f& ray){
    
    return intersect(ray);
    
}