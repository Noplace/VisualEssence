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
#define _USE_MATH_DEFINES
#include "../ve.h"


using namespace DirectX; 
using namespace DirectX::PackedVector;

//ve::VertexBufferPool<ve::shape::Vertex> pool;

namespace ve {
namespace shape {

int Arc::Initialize(Context* context) {
  int hr = Component::Initialize(context);

  if (hr != S_OK)
    return hr;
  //pool.Initialize(context,800);
  pos_.x = 0;
  pos_.y = 0;
  scale_.x = scale_.y = 1;
  angle_ = 0;
  world_ = dx::XMMatrixIdentity();
  color0_ = dxp::XMCOLOR(0xffffffff);
  color1_ = dxp::XMCOLOR(0xffffffff);

  memset(&vertex_buffer_,0,sizeof(vertex_buffer_));
  vertex_count_  = 0;
  
  return S_OK;
}

int Arc::Deinitialize() {
  //pool.Deinitialize();
  throw new std::exception();
  int hr = S_FALSE;//context_->DestroyBuffer(vertex_buffer_);
  return hr;
}

void Arc::SetParams(float radius,float start_angle,float end_angle,float thickness) {
  radius_ = radius;
  start_angle_ = start_angle;
  end_angle_ = end_angle;
  thickness_ = thickness;
}

int Arc::Construct() {
  Vertex* vertices = CreateVertices();
  if (!vertices)
    return S_FALSE;
  //vb_id = pool.Allocate(vertex_count_);

  vertex_buffer_.description.bind_flags = D3D11_BIND_VERTEX_BUFFER;
  vertex_buffer_.description.usage = D3D11_USAGE_DEFAULT;
  vertex_buffer_.description.byte_width = sizeof( Vertex ) * vertex_count_;
  vertex_buffer_.description.cpu_access_flags = 0;
  //context_->DestroyBuffer(vertex_buffer_);
  //context_->CreateBuffer(vertex_buffer_,NULL);
  throw new std::exception();
  int hr = context_->CopyToVertexBuffer(vertex_buffer_.internal_pointer,vertices,sizeof(Vertex),0,vertex_count_);
  //pool.CopyData(vb_id,vertices,vertex_count_);
  delete [] vertices;
  return hr;
}

int Arc::Update() {
  /*world_ = dx::XMMatrixTransformation2D(dx::XMLoadFloat2(&dx::XMFLOAT2(0,0)),
    0,
    dx::XMLoadFloat2(&scale_),
    dx::XMLoadFloat2(&dx::XMFLOAT2(0,0)),
    angle_,
    dx::XMLoadFloat2(&pos_));
  world_._43 = z_;*/
  //world_ = dx::XMMatrixRotationZ(angle_);
  //world_ *= dx::XMMatrixTranslation(x_,y_,0);
  //world_ = world_ * dx::XMMatrixScaling(scale_,scale_,1);
  return Shape::Update();
}

int Arc::Draw() {
  UINT stride = sizeof( Vertex );
  UINT offset = 0;
  context_->SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
  //context_->SetVertexBuffers(0,1,&vertex_buffer_,&stride,&offset);
throw new std::exception();
  //pool.Set(vb_id,0);
  return context_->Draw(vertex_count_,0);
}

Vertex* Arc::CreateVertices() {
  
  float step = dx::XM_PI/60;
  int count = (int)((end_angle_-start_angle_) / step) + 1;
  count *= 2;
  Vertex* vertices = new Vertex[count];
  float theta = start_angle_;
  int index=0;
  auto c0_vec = dxp::XMLoadColor(&color0_);
  auto c1_vec = dxp::XMLoadColor(&color1_);
  while (theta < end_angle_) {
    float x = radius_*cos(theta);
    float y = -radius_*sin(theta);
    float x2 = (radius_-thickness_)*cos(theta);
    float y2 = -(radius_-thickness_)*sin(theta);
    
    auto color_vec = dx::XMVectorLerp(c0_vec,c1_vec,theta/(end_angle_-start_angle_));
    dxp::XMCOLOR color;
    dxp::XMStoreColor(&color,color_vec);
    vertices[index].pos = dx::XMFLOAT3(x,y,0.0f);
    vertices[index].color = color;
    ++index;
    vertices[index].pos = dx::XMFLOAT3(x2,y2,0.0f);
    vertices[index].color = color;
    ++index;
    theta += step;
  }
  vertex_count_ = index;
  return vertices;
}

}
}