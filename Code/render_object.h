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

const int kRenderObjectDirtySize = 0x1;
const int kRenderObjectDirtyUV = 0x1;
const int kRenderObjectDirtyPosition = 0x2;
const int kRenderObjectDirtyTransform = 0x2;

class RenderObject : public Component {
 public:
  RenderObject() : Component(),opacity_(1.0f),dirty_(0) {}
  virtual ~RenderObject() {}
  virtual int UpdateVerticies() = 0;
  virtual int UpdateTransform() = 0;
  virtual int Update(float,float) = 0;
  virtual int Render() = 0;
  dx::XMMATRIX& world() { return world_; }
  float opacity() const { return opacity_; }
  void set_opacity(float opacity) { opacity_ = opacity; }
 protected:
  dx::XMMATRIX world_;
  float opacity_;
  int dirty_;
};

}

