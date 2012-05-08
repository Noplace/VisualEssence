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
#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include "component.h"
#include "../camera/camera.h"
#include "../shader/shader.h"

namespace graphics {

class Scene : public Component {
 public:
  Scene() { }
  virtual ~Scene() { }
  virtual int Initialize(Context* context) {
    int hr = S_OK;
    hr = Component::Initialize(context);
    hr = camera_.Initialize(context);
    return hr;
  }
  virtual int Deinitialize() {
    int hr = S_OK;
    hr = camera_.Deinitialize();
    return hr;
  }
  virtual void Draw() = 0;
 protected:

  Camera camera_;
  VertexShader vertex_shader_;
  PixelShader pixel_shader_;
};

}

#endif