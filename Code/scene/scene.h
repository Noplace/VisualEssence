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

class Scene : public RenderObject {
 public:
  Scene() { }
  virtual ~Scene() { }
  virtual int Initialize(Context* context) {
    int hr = S_OK;
    hr = Component::Initialize(context);
    return hr;
  }
  virtual int Deinitialize() {
    int hr  = Component::Deinitialize();
    return hr;
  }
  virtual concurrency::task<int> LoadAsync() = 0;
  virtual concurrency::task<int>  UnloadAsync() = 0;
  virtual int Load() {
    return LoadAsync().get();
  }
  virtual int Unload() {
    return UnloadAsync().get();
  }
  virtual int Set() = 0;
  virtual int Unset() = 0;
  //virtual int Update(float,float) = 0;
  //virtual int Render() = 0;
  virtual int UpdateVerticies() { return S_OK; };
  virtual int UpdateTransform() { return S_OK; };
  virtual int AddRenderObject(RenderObject* obj) {
    obj->set_scene(this);
    obj->set_parent(this);
    render_list_.push_back(obj);
    return S_OK;
  }
  virtual int RemoveRenderObject(RenderObject* obj) {
    obj->set_scene(nullptr);
    obj->set_parent(nullptr);
    auto iter = std::remove(render_list_.begin(),render_list_.end(),obj);
    render_list_.erase(iter,render_list_.end());
    return S_OK;
  }
  virtual int RemoveRenderObject(int index) {
    auto obj = render_list_.at(index);
    return RemoveRenderObject(obj);
  }
  virtual int UpdateWorldMatrix(const dx::XMMATRIX& world) {
    return S_FALSE;
  }
  virtual int OnInput(int msg, int data1) {
    return S_OK;
  }
  virtual Camera* camera() = 0;
 protected:
  VertexShader vs_;
  PixelShader ps_;
  std::vector<RenderObject*> render_list_;
};

}

