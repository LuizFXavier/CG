#pragma once
#include <vector>

#include "core/SharedObject.h"
#include "core/NameableObject.h"
#include "graphics/Light.h"
#include "geometry/Ray.h"

#include "shapes/Intersection.h"
#include "entities/Actor.h"

class Scene : public cg::NameableObject{
private:

    std::vector<cg::Reference<Actor>> _actorList;
    std::vector<cg::Light> _lightList;
    cg::Color _backgroundColor;
    cg::Color _ambientLight{cg::Color::darkGray};

public:
    const std::vector<cg::Reference<Actor>> actors() const{
        return _actorList;
    }
    const std::vector<cg::Light> lights() const{
        return _lightList;
    }

    const cg::Color& backgroundColor() const{
        return _backgroundColor;
    }
    const cg::Color& ambientLight() const{
        return _ambientLight;
    }

    Scene(cg::Color bgColor, cg::Color ambientLight, const char* name = "Scene"):
    cg::NameableObject(name), _backgroundColor(bgColor), _ambientLight(ambientLight){}

    void addActor(cg::Reference<Actor> actor){
        this->_actorList.push_back(actor);
    }
    void addLight(cg::Light light){
        this->_lightList.push_back(light);
    }
};
