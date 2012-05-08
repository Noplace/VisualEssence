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