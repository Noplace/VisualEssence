#ifndef GRAPHICS_SHAPE_RECTANGLE_H
#define GRAPHICS_SHAPE_RECTANGLE_H

#include "shape.h"

namespace graphics {
namespace shape {


class Rectangle : public Shape {
 public:
  Rectangle() {}
  ~Rectangle() {}
  int Initialize(Context* context);
  int Deinitialize();
  int SetColor(XMCOLOR color);
  int SetParams(float width, float height);
  int Construct();
  int BuildTransform();
  int Draw();
  XMVECTOR color() { return XMLoadColor(&color_); }
  float width() { return width_; }
  float height() { return height_; }
 private:
  Rectangle::Vertex* CreateVertices();
  XMCOLOR color_;
  float width_;
  float height_;
};

}
}

#endif