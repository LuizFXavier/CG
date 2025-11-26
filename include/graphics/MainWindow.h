#pragma once
#include "graphics/GLRenderWindow3.h"
#include "graphics/RayCast.h"

class MainWindow : public cg::GLRenderWindow3{
    
private:
    using Base = cg::GLRenderWindow3;

    
    cg::Reference<RayCaster> rayCaster;
    

    void initialize() override;
    void update() override;
    void renderScene() override;
    bool keyInputEvent(int, int, int) override;
    void gui() override;

public:
    MainWindow(int width, int height):
    Base{"Ds template", width, height}{}
};