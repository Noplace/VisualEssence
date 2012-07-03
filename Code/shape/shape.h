/*****************************************************************************************************************
* Copyright (c) 2012 Khalid Ali Al-Kooheji                                                                       *
*                                                                                                                *
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and              *
* associated documentation files (the "Software"), to deal in the Software without restriction, including        *
* without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        *
* copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the       *
* following conditions:                                                                                          *
*                                                                                                                *
* The above copyright notice and this permission notice shall be included in all copies or substantial           *
* portions of the Software.                                                                                      *
*                                                                                                                *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT          *
* LIMITED TO THE WARRANTIES OF MERCHANTABILITY, * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.          *
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, * DAMAGES OR OTHER LIABILITY,      *
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE            *
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                                         *
*****************************************************************************************************************/
#ifndef GRAPHICS_SHAPE_SHAPE_H
#define GRAPHICS_SHAPE_SHAPE_H

#include "../drawable.h"

namespace graphics {
namespace shape {

struct Vertex {
  XMFLOAT3 pos;
  XMFLOAT2 tex_uv;
  XMCOLOR  color;
  uint32_t tex_page;
  Vertex() : pos(XMFLOAT3(0,0,0)),tex_uv(XMFLOAT2(0,0)),color(XMCOLOR((UINT)0)),tex_page(0) {
  }
  Vertex(XMFLOAT3 pos,XMFLOAT2 tex_uv,XMCOLOR color,uint32_t tex_page) {
    this->pos = pos;
    this->tex_uv = tex_uv;
    this->color = color;
    this->tex_page = tex_page;
  }
  Vertex(const Vertex& other) {
    memcpy(this,&other,sizeof(Vertex));
  }
  Vertex& operator=(const Vertex& other) {
    memcpy(this,&other,sizeof(Vertex));
    return *this;
  }
};

class Shape : public Drawable {
 public:
  Shape() : Drawable(), x_(0), y_(0),z_(0), scale_(1), angle_(0) {
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
  float x() { return x_; }
  float y() { return y_; }
  float* scale_ptr() { return &scale_; }
  float* angle_ptr() { return &angle_; }
  XMMATRIX& world() { return world_; }
  virtual int Construct() = 0; 
  virtual int BuildTransform() {
    world_ = XMMatrixTransformation2D(XMLoadFloat2(&XMFLOAT2(0,0)),
    0,
    XMLoadFloat2(&XMFLOAT2(scale_,scale_)),
    XMLoadFloat2(&XMFLOAT2(0,0)),
    angle_,
    XMLoadFloat2(&XMFLOAT2(x_,y_)));
    world_._43 = z_;
    return S_OK;
  }
 protected:
  int vertex_count_;
  graphics::Buffer vertex_buffer_,index_buffer_;
  XMMATRIX world_;
  float x_,y_,z_,scale_,angle_;
};

}
}

#endif