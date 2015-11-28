#include "ControllerSubclass.h"

ControllerSubclass::ControllerSubclass(const std::string& windowTitle, int rcFlags) : GLFWController(windowTitle, rcFlags)
{
  bool isLeftMouseButtonPressed = false;
  int lastx = 0;
  int lasty = 0;
}

ControllerSubclass::~ControllerSubclass(){}

void ControllerSubclass::handleMouseButton(MouseButton button, bool pressed, int x, int y, int mods)
{
   mbutton = button;
   isLeftMouseButtonPressed = pressed;
   lastx = x;
   lasty = y;
}

void ControllerSubclass::handleMouseMotion(int x, int y)
{
  int dx = x - lastx;
  int dy = y - lasty;
  lastx = x;
  lasty = y;
  double anglex, angley;
  if((mbutton == LEFT_BUTTON) && isLeftMouseButtonPressed)
  {
    anglex = dx * (360 / 250);
    angley = dy * (360 / 250);
    ModelView::addToGlobalRotationDegrees(angley, anglex, 0.0);
  }
}

void ControllerSubclass::handleScroll(bool up)
{
  if(up)
    ModelView::addToGlobalZoom(-0.1);
  else
    ModelView::addToGlobalZoom(0.1);
}
