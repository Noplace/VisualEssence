#ifndef GRAPHICS_SPRITE_H
#define GRAPHICS_SPRITE_H

#include "../context/context.h"

namespace graphics {

class Sprite : public graphics::shape::Shape {
 public:
  Sprite() {}
  ~Sprite() {}
  int Initialize(Context* context);
  int Deinitialize();
  int SetSize(float width,float height);
  int SetColor(XMCOLOR color);
  int SetUV(float u0,float v0,float u1,float v1);
  int Construct();
  int BuildTransform();
  int Draw();
  XMVECTOR color() { return XMLoadColor(&color_); }
 private:
  graphics::Buffer vertex_buffer_;
  float scale_,angle_;
  float width_,height_;
  float u0,v0,u1,v1;
  XMCOLOR color_;
};

}

#endif