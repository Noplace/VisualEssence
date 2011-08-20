#ifndef GRAPHICS_SHAPE_ARC_H
#define GRAPHICS_SHAPE_ARC_H

#include "shape.h"

namespace graphics {
namespace shape {


class Arc : public Shape {
 public:
  Arc() {}
  ~Arc() {}
  int Initialize(Context* context);
  int Deinitialize();
  int SetColor(XMCOLOR color);
  int SetParams(float radius,float start_angle,float end_angle,float thickness);
  int Construct();
  int BuildTransform();
  int Draw();
  XMMATRIX& world() { return world_; }
  XMVECTOR color() { return XMLoadColor(&color_); }
 private:
  Arc::Vertex* CreateVertices();
  float radius_;
  float start_angle_;
  float end_angle_;
  float thickness_;
  XMCOLOR color_;
};

}
}

#endif