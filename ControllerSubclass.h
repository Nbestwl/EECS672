#ifndef CONTROLLERSUBCLASS_H
#define CONTROLLERSUBCLASS_H

#include "GLFWController.h"
#include "ModelView.h"
#include "GLFW/glfw3.h"

class ControllerSubclass : public GLFWController
{
public:
  ControllerSubclass(const std::string& windowTitle, int rcFlags = 0);
  ~ControllerSubclass();

protected:
  int lastx, lasty;
  bool isLeftMouseButtonPressed;
  MouseButton mbutton;
  void handleMouseButton(MouseButton button, bool pressed, int x, int y, int mods);
  void handleMouseMotion(int x, int y);
  void handleScroll(bool up);
};

#endif
