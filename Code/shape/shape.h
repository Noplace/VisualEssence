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
#pragma once

namespace ve {
namespace shape {

struct Vertex {
  dx::XMFLOAT3 pos;
  dx::XMFLOAT2 tex_uv;
  dxp::XMCOLOR  color;
  uint32_t tex_page;
  Vertex() : pos(dx::XMFLOAT3(0,0,0)),tex_uv(dx::XMFLOAT2(0,0)),color(dxp::XMCOLOR((UINT)0)),tex_page(0) {
  }
  Vertex(dx::XMFLOAT3 pos,dx::XMFLOAT2 tex_uv,dxp::XMCOLOR color,uint32_t tex_page) {
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
  Shape() : Drawable(), pos_(0,0), z_(0), scale_(1,1), angle_(0),changed_(true) {
  }
  virtual ~Shape() { }
  virtual int SetTopLeft(float x,float y) {
    pos_.x = x;
    pos_.y = y;
    changed_ = true;
    return S_OK;
  }
  virtual int SetScale(float scale) {
    scale_.x = scale_.y = scale;
    changed_ = true;
    return S_OK;
  }
  virtual int SetRotate(float angle) {
    angle_ = angle;
    changed_ = true;
    return S_OK;
  }
  virtual int SetOrder(float z) {
    z_ = z;
    changed_ = true;
    return S_OK;
  }
  ve::Buffer& vertex_buffer() { return vertex_buffer_; }
  float* x_ptr() { return &pos_.x; }
  float* y_ptr() { return &pos_.y; }
  float x() { return pos_.x; }
  float y() { return pos_.y; }
  float* scalex_ptr() { return &scale_.x; }
  float* scaley_ptr() { return &scale_.y; }
  float* angle_ptr() { return &angle_; }
  dx::XMMATRIX& world() { return world_; }
  virtual int Construct() = 0; 
  virtual int Update() {
    if (changed_ == false) return S_FALSE;
    world_ = dx::XMMatrixTransformation2D(dx::XMLoadFloat2(&dx::XMFLOAT2(0,0)),
    0,
    dx::XMLoadFloat2(&scale_),
    dx::XMLoadFloat2(&dx::XMFLOAT2(0,0)),
    angle_,
    dx::XMLoadFloat2(&pos_));
    //world_._43 = z_;
    changed_ = false;
    return S_OK;
  }
 protected:
  int vertex_count_;
  ve::Buffer vertex_buffer_,index_buffer_;
  dx::XMMATRIX world_;
  dx::XMFLOAT2 scale_;
  dx::XMFLOAT2 pos_;
  float z_,angle_;
  bool changed_;
};

}
}

