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

__declspec(align(16))
struct ModelViewProjectionConstantBuffer {
	dx::XMFLOAT4X4 model;
	dx::XMFLOAT4X4 view;
  dx::XMFLOAT4X4 viewInv;
	dx::XMFLOAT4X4 projection;
};


class Camera : public Component {
 public:
  virtual ~Camera() {
    Deinitialize();
  }

  virtual int Initialize(Context* context) {
    int hr = S_OK;
    Component::Initialize(context);
    return hr;
  }
  virtual int Update(float dt) { return S_OK; }
  dx::XMMATRIX& projection() { return projection_;  }
  dx::XMMATRIX& view() { return view_;  }
  dx::XMMATRIX viewprojection() { return view_*projection_;  }
  dx::XMMATRIX projection_transposed() { return dx::XMMatrixTranspose(projection_);  }
  dx::XMMATRIX view_transposed() { return dx::XMMatrixTranspose(view_);  }
 protected:
  dx::XMMATRIX view_;
  dx::XMMATRIX projection_;
};

class CameraNode : public RenderObject {
 public:
  CameraNode() : RenderObject(),cb_index(0) {
  }       

  int Initialize(Context* context) {
    int hr = S_OK;
    RenderObject::Initialize(context);
    CD3D11_BUFFER_DESC desc(sizeof(ve::ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
    context_->CreateBuffer(&desc,nullptr, (void**)&cb_);
    return hr;
  }
  int Deinitialize() {
    context_->DestroyBuffer((void**)&cb_);
    return S_OK;
  }
  int UpdateVerticies() { return S_OK; }
  int UpdateTransform() { return S_OK; }
  int Update(float time,float dt) {
    camera_->Update(dt);
	  dx::XMStoreFloat4x4(&cb_data_.view, camera_->view_transposed());
    dx::XMStoreFloat4x4(&cb_data_.projection, camera_->projection_transposed());
    return S_OK;
  }
  int Render() {
    context_->SetConstantBuffers(kShaderTypeVertex,cb_index,1,(const void**)&cb_);
    return S_OK;
  }
  
  int SetWorldMatrix(const dx::XMMATRIX& world) {
    dx::XMStoreFloat4x4(&cb_data_.model, dx::XMMatrixTranspose(world));
    context_->UpdateSubresource((const void*)cb_,&cb_data_,nullptr,0,0);
    return S_OK;
  }

  void set_camera(Camera* camera) { camera_ = camera; }
 protected:
  Camera* camera_;
  ID3D11Buffer* cb_;
  uint32_t cb_index;
  ModelViewProjectionConstantBuffer cb_data_;
};

class PrespectiveCamera : public Camera {
 public:
  void BuildProjectionMatrix(float fov, float aspect_ratio, float near_z, float far_z) {
		projection_ = dx::XMMatrixPerspectiveFovRH(fov,aspect_ratio,near_z,far_z);
  }
  void BuildViewMatrix(dx::XMVECTOR eye, dx::XMVECTOR at, dx::XMVECTOR up) {
    view_ = dx::XMMatrixLookAtRH(eye, at, up);
  }
/*  void Build() {
    BuildViewMatrix();
    BuildProjectionMatrix();
  }*/

};

class OrthoCamera : public Camera {
 public:
  void BuildProjectionMatrix(float aspect_ratio) {
		projection_ = dx::XMMatrixOrthographicOffCenterRH(0.0f,aspect_ratio,1.0f,0.0f,0.0f,1.0f);
  }

  void BuildProjectionMatrix(float width, float height) {
		projection_ = dx::XMMatrixOrthographicOffCenterRH(0.0f,width,height,0.0f,0.0f,1.0f);
  }



  void BuildViewMatrix() {
    view_ = dx::XMMatrixIdentity();
    cam_left = cam_top = 0;
  }

  int Update(float timeDelta) {
  //testing code
    {
    float speed = 100.0f * timeDelta;

      /*if (GetAsyncKeyState(VK_UP))
        cam_top += speed;
      if (GetAsyncKeyState(VK_DOWN))
        cam_top -= speed;

      if (GetAsyncKeyState(VK_LEFT))
        cam_left += speed;
      if (GetAsyncKeyState(VK_RIGHT))
        cam_left -= speed;*/
    }

    view_ = dx::XMMatrixTranslation(cam_left,cam_top,0);
    return S_OK;
  }
/*  void Build() {
    BuildViewMatrix();
    BuildProjectionMatrix();
  }*/
 protected:
    float cam_left,cam_top; 
};



/*
class Camera : public Component {
 public:
  struct ConstantBuffer {
    dx::XMMATRIX mView;
    dx::XMMATRIX mProjection;
  };

  Camera() : Component(),ratio_(0) {

  }
  virtual ~Camera() {
    Deinitialize();
  }

  virtual int Initialize(Context* context) {
    int hr = S_OK;
    Component::Initialize(context);
    // Create the camera constant buffer
    cam_buffer_.description.usage = D3D11_USAGE_DEFAULT;
    cam_buffer_.description.byte_width = sizeof(ConstantBuffer);
    cam_buffer_.description.bind_flags = D3D11_BIND_CONSTANT_BUFFER;
    cam_buffer_.description.cpu_access_flags = 0;
    hr = context_->CreateBuffer(cam_buffer_,NULL);
    if( FAILED( hr ) )
        return hr;

    return hr;
  }


  virtual int Deinitialize() {
    if (context_ == NULL) 
      return S_FALSE;
    context_->DestroyBuffer(cam_buffer_);
    context_ = nullptr;
    return S_OK;
  }

  void Ortho2D() {
    //RECT rect;
	  //GetClientRect(context()->window()->handle(),&rect);
    //D3Ddx::XMatrixOrthoLH(&Ortho2D, (FLOAT)rect.right, (FLOAT)rect.bottom, 0.0f, 1.0f);
    //D3Ddx::XMatrixOrthoOffCenterLH(&Ortho2D, 0.0f,(FLOAT)rect.right,(FLOAT)rect.bottom,0.0f,0.0f,1.0f);
    ratio_ = (FLOAT)context()->width()/context()->height();
    //D3Ddx::XMatrixOrthoOffCenterLH(&projection_, 0.0f,ratio,1.0f,0.0f,0.0f,1.0f);
    projection_ = dx::XMMatrixOrthographicOffCenterLH(0.0f,(FLOAT)context()->width(),(FLOAT)context()->height(),0.0f,-10000.0f,10000.0f);
	  //D3Ddx::XMatrixIdentity(&view_);
    view_ = dx::XMMatrixIdentity();
  }

  void Ortho2D(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom) {
    projection_ = dx::XMMatrixOrthographicOffCenterLH(left,right,bottom,top,-10000.0f,10000.0f);
    view_ = dx::XMMatrixIdentity();
  }

  inline dx::XMFLOAT2 OrthoPoint(FLOAT rx, FLOAT ox, FLOAT ry, FLOAT oy) {
    return dx::XMFLOAT2((rx*(FLOAT)context()->width()) + ox,(ry*(FLOAT)context()->height()) + oy);
  }

  void Perspective() {
    ratio_ = (FLOAT)context()->width()/context()->height();
     projection_ = dx::XMMatrixPerspectiveFovLH( dx::XM_PIDIV4, ratio_, 0.01f, 100.0f );
     //view_ = dx::XMMatrixIdentity();
  }

  void UpdateConstantBuffer() {
    ConstantBuffer cbCamera;
    cbCamera.mView = view_transposed();
    cbCamera.mProjection = projection_transposed();
    context_->UpdateSubresource(cam_buffer_,&cbCamera,NULL,0,0);
  }

  void SetConstantBuffer(uint32_t slot) {
    //send to vertex shader only
    context_->SetConstantBuffers(kShaderTypeVertex,slot,1,&cam_buffer_);
  }

  float ratio() { return ratio_; }
  dx::XMMATRIX& projection() { return projection_;  }
  dx::XMMATRIX& view() { return view_;  }
  dx::XMMATRIX viewprojection() { return view_*projection_;  }
  dx::XMMATRIX projection_transposed() { return dx::XMMatrixTranspose(projection_);  }
  dx::XMMATRIX view_transposed() { return dx::XMMatrixTranspose(view_);  }
 private:
  float ratio_;
  dx::XMMATRIX view_;	
  dx::XMMATRIX projection_;
  ve::Buffer cam_buffer_;
};
*/

}


#include "first_person_camera.h"