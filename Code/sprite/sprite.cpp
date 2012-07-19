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

namespace graphics {

int Sprite::Initialize(Context* context) {
  int hr = Component::Initialize(context);

  if (hr != S_OK)
    return hr;
  pos_.x = 0;
  pos_.y = 0;
  scale_ = 1;
  angle_ = 0;
  world_ = XMMatrixIdentity();
  vertex_buffer_.description.bind_flags = D3D11_BIND_VERTEX_BUFFER;
  vertex_buffer_.description.usage = D3D11_USAGE_DEFAULT;
  vertex_buffer_.description.byte_width = sizeof( graphics::shape::Vertex ) * 4;
  vertex_buffer_.description.cpu_access_flags = 0;
  context->CreateBuffer(vertex_buffer_,NULL);
  u0 = 0;
  v0 = 0;
  u1 = 1;
  v1 = 1;
  color_ = XMCOLOR(0xffffffff);
  return hr;
}

int Sprite::Deinitialize() {
  context_->DestroyBuffer(vertex_buffer_);
  return S_OK;
}

int Sprite::SetSize(float width,float height) {
  width_ = width;
  height_ = height;
  return S_OK;
}

int Sprite::SetColor(XMCOLOR color) {
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
  graphics::shape::Vertex vertices[] =
  {
    graphics::shape::Vertex(XMFLOAT3( 0, 0 ,0.0f), XMFLOAT2( u0, v0  ),XMCOLOR(1,1,1,1),0),  
    graphics::shape::Vertex(XMFLOAT3( width_, 0,0.0f ), XMFLOAT2( u1, v0 ),XMCOLOR(1,1,1,1),0),
    graphics::shape::Vertex(XMFLOAT3( 0, height_,0.0f ), XMFLOAT2( u0, v1 ),XMCOLOR(1,1,1,1),0),
    graphics::shape::Vertex(XMFLOAT3( width_, height_,0.0f ), XMFLOAT2( u1, v1 ),XMCOLOR(1,1,1,1),0)
  };
  return context_->CopyToVertexBuffer(vertex_buffer_,vertices,sizeof(graphics::shape::Vertex),0,4);
}

int Sprite::Update() {
  
  if (changed_ == false) return S_FALSE;
  
  world_ = XMMatrixTransformation2D(XMLoadFloat2(&XMFLOAT2(width_*0.5f,height_*0.5f)),
    0,
    XMLoadFloat2(&XMFLOAT2(scale_,scale_)),
    XMLoadFloat2(&XMFLOAT2(width_*0.5f,height_*0.5f)),
    angle_,
    XMLoadFloat2(&XMFLOAT2(pos_.x,pos_.y)));
  world_._43 = z_;
  //world_ = XMMatrixRotationZ(angle_);
  //world_ *= XMMatrixTranslation(pos_.x,pos_.y,0);
  //world_ = world_ * XMMatrixScaling(scale_,scale_,1);
  changed_ = false;
  return S_OK;
}

int Sprite::Draw() {
  UINT stride = sizeof( graphics::shape::Vertex );
  UINT offset = 0;
  context_->SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
  context_->SetVertexBuffers(0,1,&vertex_buffer_,&stride,&offset);
  return context_->Draw(4,0);
}

}