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
#include "../ve.h"

using namespace DirectX; 
using namespace DirectX::PackedVector;

namespace ve {

int Sprite::Initialize(Context* context) {
  int hr = Component::Initialize(context);

  if (hr != S_OK)
    return hr;
  pos_.x = 0;
  pos_.y = 0;
  scale_ = 1;
  angle_ = 0;
  world_ = dx::XMMatrixIdentity();
  vertex_buffer_.description.bind_flags = D3D11_BIND_VERTEX_BUFFER;
  vertex_buffer_.description.usage = D3D11_USAGE_DEFAULT;
  vertex_buffer_.description.byte_width = sizeof( ve::shape::Vertex ) * 4;
  vertex_buffer_.description.cpu_access_flags = 0;
  throw new std::exception();
  //context->CreateBuffer(vertex_buffer_,NULL);
  u0 = 0;
  v0 = 0;
  u1 = 1;
  v1 = 1;
  color_ = dxp::XMCOLOR(0xffffffff);
  return hr;
}

int Sprite::Deinitialize() {
  throw new std::exception();
  //context_->DestroyBuffer(vertex_buffer_);
  return S_OK;
}

int Sprite::SetSize(float width,float height) {
  width_ = width;
  height_ = height;
  return S_OK;
}

int Sprite::SetColor(dxp::XMCOLOR color) {
  color_ = color;
  return S_OK;
}

int Sprite::SetUV(float u0,float v0,float u1,float v1) {
  this->u0 = u0;
  this->v0 = v0;
  this->u1 = u1;
  this->v1 = v1;
  return S_OK;
}

int Sprite::Construct() {
  ve::shape::Vertex vertices[] =
  {
    ve::shape::Vertex(dx::XMFLOAT3( 0, 0 ,0.0f), dx::XMFLOAT2( u0, v0  ),dxp::XMCOLOR(1,1,1,1),0),  
    ve::shape::Vertex(dx::XMFLOAT3( width_, 0,0.0f ), dx::XMFLOAT2( u1, v0 ),dxp::XMCOLOR(1,1,1,1),0),
    ve::shape::Vertex(dx::XMFLOAT3( 0, height_,0.0f ), dx::XMFLOAT2( u0, v1 ),dxp::XMCOLOR(1,1,1,1),0),
    ve::shape::Vertex(dx::XMFLOAT3( width_, height_,0.0f ), dx::XMFLOAT2( u1, v1 ),dxp::XMCOLOR(1,1,1,1),0)
  };
  return context_->CopyToVertexBuffer(vertex_buffer_.internal_pointer,vertices,sizeof(ve::shape::Vertex),0,4);
}

int Sprite::Update() {
  
  if (changed_ == false) return S_FALSE;
  
  world_ = dx::XMMatrixTransformation2D(dx::XMLoadFloat2(&dx::XMFLOAT2(width_*0.5f,height_*0.5f)),
    0,
    dx::XMLoadFloat2(&dx::XMFLOAT2(scale_,scale_)),
    dx::XMLoadFloat2(&dx::XMFLOAT2(width_*0.5f,height_*0.5f)),
    angle_,
    dx::XMLoadFloat2(&dx::XMFLOAT2(pos_.x,pos_.y)));
  //world_.r[3]._43 = z_;
  //world_ = dx::XMMatrixRotationZ(angle_);
  //world_ *= dx::XMMatrixTranslation(pos_.x,pos_.y,0);
  //world_ = world_ * dx::XMMatrixScaling(scale_,scale_,1);
  changed_ = false;
  return S_OK;
}

int Sprite::Draw() {
  UINT stride = sizeof( ve::shape::Vertex );
  UINT offset = 0;
  context_->SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
  //context_->SetVertexBuffers(0,1,&vertex_buffer_,&stride,&offset);
  throw new std::exception();
  return context_->Draw(4,0);
}

}