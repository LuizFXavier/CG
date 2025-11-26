#include "shapes/Sphere.h"
#include "shapes/Plane.h"
#include "graphics/RayCast.h"
#include "entities/Scene.h"
#include "entities/Actor.h"
#include "graphics/MainWindow.h"

#include <iostream>

using namespace cg;

void
MainWindow::initialize(){

  Base::initialize();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0f, 1.0f);

  rayCaster = new RayCaster(width(), height());

  Reference<Shape3> plane = new Plane();
  Reference<Shape3> sphere = new Sphere();

  Reference<Material> cobalt = new Material(Color(0.1f, 0.f, 0.6f));
  // cobalt->ambient = cobalt->ambient * 5.f;

  Reference<Material> g = new Material(Color(0.1f, 0.6f, 0.6f));
  // g->ambient = g->ambient * 5.f;
  
  Reference<Material> r = new Material(Color(0.6f, 0.f, 0.1f));

  Reference<Material> k = new Material(Color(1.f, 1.f, 0.1f));
  // r->ambient = r->ambient * 5.f;

  Reference<Camera> camera = new Camera((float)width() / (float)height());

  // camera->setViewAngle(30.0f);
  camera->setPosition(vec3f{-2.f, 1.8f, 0.f});
  camera->setNearPlane(1.f);
  // camera->setViewUp(vec3f{0.f, 0.f, -1.f});
  camera->setDirectionOfProjection(vec3f{1.f, -0.1f, 0.f});

  // camera->setHeight(100.f);

  camera->setProjectionType(Camera::ProjectionType::Parallel);

  camera->update();

  
  rayCaster->setCamera(camera);

  // rayCaster->setPixelRay(50, 50);

  rayCaster->_pixelRay.origin.print("O: ");
  std::cout << "Size:" << rayCaster->_pixelRay.origin.length() << "\n";
  rayCaster->_pixelRay.direction.print("D: ");
  rayCaster->_vrc.n.print("N: ");
  rayCaster->_vrc.u.print("U: ");
  rayCaster->_vrc.v.print("V: ");

  // exit(1);

  Reference<Scene> scene = new Scene(Color(0.1f, 0.1f, 0.1f), Color::white);

  Reference<Actor> bola = new Actor(sphere, cobalt);
  
  Reference<Actor> plano = new Actor(plane, r);
  Reference<Actor> plano2 = new Actor(plane, g);

  plano->setTransform(vec3f{0.f, 0.f, 0.f}, quatf{-30.f, vec3f{0.f, 0.f, 1.f}}, vec3f{1.f, 1.f, 1.f});
  plano2->setTransform(vec3f{0.f, 0.f, 0.f}, quatf{-30.f, vec3f{1.f, 0.f, 0.f}}, vec3f{1.f, 1.f, 1.f});
  
  bola->setTransform(vec3f{0.f, 0.f, 0.f}, quatf{0.f, 0.f, 0.f, 1.f}, vec3f{1.f, 1.f, 1.f});

  Reference<Actor> bola2 = new Actor(sphere, k);

  Light* l1 = new Light();

  l1->color = Color::white;

  l1->setPosition(vec3f{0.f, 2.f, 0.f});
  
  bola2->setTransform(vec3f{1.f, 1.5f, 3.f}, quatf{0.f, 0.f, 0.f, 1.f}, vec3f{1.f, 1.f, 1.f});

  bola->localToWorldMatrix().print("BOLA:");
  // exit(1);
  scene->addActor(bola);
  scene->addActor(bola2);
  scene->addActor(plano);
  scene->addActor(plano2);
  scene->addLight(*l1);

  rayCaster->setScene(scene);

  rayCaster->render();
  std::cout << "Cena criada\n";
}

void
MainWindow::update(){

}

void
MainWindow::renderScene(){
  rayCaster->drawn();
}

void
MainWindow::gui(){

}

bool
MainWindow::keyInputEvent(int key, int action, int mods)
{
  return false;
  return Base::keyInputEvent(key, action, mods);
}
