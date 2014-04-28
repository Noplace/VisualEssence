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

#include "../component.h"
#include "../camera/camera.h"
#include "../effect/effect.h"

namespace ve {
namespace shader {

struct ConstantBuffer2Type {
    dx::XMMATRIX world;
    bool enable_texture;
    dx::XMVECTOR ps_color;
};

class Shader2DHelper : public ve::Component {
 public:
  Shader2DHelper();
  ~Shader2DHelper();
  int Initialize(Context* context);
  int Deinitialize();
  void CreateFromMemory(void* data_pointer, size_t data_length);
  int PrepareDraw();
  int UpdateChangesEveryFrame(ConstantBuffer2Type* cef);
  int SetColorShader();
  int SetTextureShader();
  int SetTexturePagesShader();
 private:
  ve::Camera camera_;
  ve::Effect effect_;
  ve::Buffer cef_buffer;
};

}
}

