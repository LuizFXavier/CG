#pragma once
#include <vector>

#include "SharedObject.h"
#include "NameableObject.h"
#include "Light.h"

#include "Actor.h"

// TODO: Intersecção com os atores na cena, retornando o mais próximo.
// TODO: Modelo de iluminação guardado nos atores? Talvez na cena? 

class Scene : public cg::NameableObject{
private:

    std::vector<cg::Reference<Actor>> _actorList;
    std::vector<cg::Light> _lightList;
    cg::Color _backgroundColor;

public:
    const std::vector<cg::Reference<Actor>> actors() const{
        return _actorList;
    }

    const cg::Color& backgroundColor() const{
        return _backgroundColor;
    }

    Scene(const char* name, cg::Color bgColor):
    cg::NameableObject(name), _backgroundColor(bgColor){}

    void addActor(cg::Reference<Actor> actor){
        this->_actorList.push_back(actor);
    }
    void addLight(cg::Light light){
        this->_lightList.push_back(light);
    }
    
    
};
