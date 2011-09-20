#ifndef GRAPHICS_SHAPE_SHAPE_H
#define GRAPHICS_SHAPE_SHAPE_H

#include "../drawable.h"

namespace graphics {
namespace shape {

struct Vertex {
  XMFLOAT3 pos;
  XMFLOAT2 tex_uv;
  XMFLOAT4 color;
};

class Shape : public Drawable {
 public:
  Shape() : x_(0), y_(0),z_(0), scale_(1), angle_(0) {
  }
  virtual ~Shape() { }
  virtual int SetTopLeft(float x,float y) {
    x_ = x;
    y_ = y;
    return S_OK;
  }
  virtual int SetScale(float scale) {
    scale_ = scale;
    return S_OK;
  }
  virtual int SetRotate(float angle) {
    angle_ = angle;
    return S_OK;
  }
  virtual int SetOrder(float z) {
    z_ = z;
    return S_OK;
  }
  graphics::Buffer& vertex_buffer() { return vertex_buffer_; }
  float* x_ptr() { return &x_; }
  float* y_ptr() { return &y_; }
  float* scale_ptr() { return &scale_; }
  float* angle_ptr() { return &angle_; }
  XMMATRIX& world() { return world_; }
  virtual int Construct() = 0; 
  virtual int BuildTransform() = 0;

 protected:
  int vertex_count_;
  graphics::Buffer vertex_buffer_;
  XMMATRIX world_;
  float x_,y_,z_,scale_,angle_;
};

}
}

#endif