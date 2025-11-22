#pragma once

#include "SharedObject.h"
#include "GLImage.h"
#include "Color.h"
#include "Camera.h"
#include "CameraHolder.h"
#include "Ray.h"
#include "Vector3.h"

#include "entities/Scene.h"

struct Viewport{
    int w;
    int h;
};

class RayModel : public cg::SharedObject{
    
protected:

    struct{
        cg::vec3f u;
        cg::vec3f v;
        cg::vec3f n;
    } _vrc;

    Viewport _viewport;
    
    cg::Reference<cg::GLImage> _image;
    cg::Reference<cg::Camera> _camera;
    cg::ImageBuffer _imageBuffer;
    
    cg::Reference<Scene> _scene;

    cg::Ray3f _pixelRay;
    
    RayModel(int imageWidth, int imageHeight):
    _imageBuffer(imageWidth, imageHeight){}

    void setPixelRay(float x, float y);

    cg::vec3f globalPixel(float x, float y) const;

    cg::Color virtual shoot(float x, float y) = 0;
    
    bool virtual intersect(Intersection&) = 0; // Verifica se houve intersecção do raio com a cena.

    cg::Color virtual shade(cg::Ray3f&, Intersection&) = 0; // Retorna a cor refletida pelo objeto.

public:
    void setCamera(cg::Camera* camera);

    void setScene(cg::Reference<Scene> scene){
        this->_scene = scene;
    }

    void setViewport(int width, int height){
        _viewport.w = width;
        _viewport.h = height;
    }

    cg::Color background(){
        return _scene->backgroundColor();
    }
    
    void virtual render() = 0;
};

// Retorna as coordenadas globais do pixel, no eixo da camera.
inline cg::vec3f 
RayModel::globalPixel(float x, float y) const{

    // Talvez o iW e iH não signifique o que eu acho, então pode dar problema
    int iW = _imageBuffer.width(), iH = _imageBuffer.height();
    int vW = _viewport.w, vH = _viewport.h;
    
    return vW * (x * iW - 0.5f) * _vrc.u + vH * (y * iH - 0.5f) * _vrc.v;
}


inline void
RayModel::setCamera(cg::Camera* camera){
    
    if (camera != _camera.get()){
        (_camera = camera ? camera : new cg::Camera{})->update();
        const auto& m = _camera->cameraToWorldMatrix();
        
        // Inicialização das coordenadas do vrc para evitar recálculo no traçado de raios
        _vrc.u = m[0];
        _vrc.v = m[1]; 
        _vrc.n = m[2];
        
        _pixelRay.origin = _camera->position();
    }
}

inline void 
RayModel::setPixelRay(float x, float y){
    
    auto p = globalPixel(x, y);

    switch (_camera->projectionType()){
        case cg::Camera::Perspective:
            _pixelRay.direction = (p - _camera->nearPlane() * _vrc.n).versor();
        break;

        case cg::Camera::Parallel:
            _pixelRay.origin = _camera->position() + p;
        break;
    }
}