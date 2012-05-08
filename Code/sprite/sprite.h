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