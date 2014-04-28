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


class Rectangle : public Shape {
 public:
  Rectangle() {}
  ~Rectangle() {}
  int Initialize(Context* context);
  int Deinitialize();
  int SetColors(dxp::XMCOLOR color1,dxp::XMCOLOR color2,dxp::XMCOLOR color3,dxp::XMCOLOR color4);
  int SetParams(float width, float height);
  int Construct();
  int Update();
  int Draw();
  float width() { return width_; }
  float height() { return height_; }
 private:
  Rectangle::Vertex* CreateVertices();
  dxp::XMCOLOR color1_;
  dxp::XMCOLOR color2_;
  dxp::XMCOLOR color3_;
  dxp::XMCOLOR color4_;
  float width_;
  float height_;
};

}
}

