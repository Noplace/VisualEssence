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
#include <math.h>
#include "../ve.h"


namespace graphics {
namespace shape {

int Rectangle::Initialize(Context* context) {
  int hr = Component::Initialize(context);

  if (hr != S_OK)
    return hr;
  x_ = 0;
  y_ = 0;
  scale_ = 1;
  angle_ = 0;
  world_ = XMMatrixIdentity();
  color_ = XMCOLOR(0xffffffff);

  memset(&vertex_buffer_,0,sizeof(vertex_buffer_));
  vertex_count_  = 0;
  return S_OK;
}

int Rectangle::Deinitialize() {
  int hr = context_->DestroyBuffer(vertex_buffer_);
  return hr;
}

int Rectangle::SetColor(XMCOLOR color) {
  color_ = color;
  return S_OK;
}

int Rectangle::SetParams(float width,float height) {
  width_ = width;
  height_ = height;
  return S_OK;
}

int Rectangle::Construct() {
  Vertex* vertices = CreateVertices();
  if (!vertices)
    return S_FALSE;

  vertex_buffer_.description.bind_flags = D3D11_BIND_VERTEX_BUFFER;
  vertex_buffer_.description.usage = D3D11_USAGE_DEFAULT;
  vertex_buffer_.description.byte_width = sizeof( Vertex ) * vertex_count_;
  vertex_buffer_.description.cpu_access_flags = 0;
  context_->DestroyBuffer(vertex_buffer_);
  context_->CreateBuffer(vertex_buffer_,NULL);
  int hr = context_->CopyToVertexBuffer(vertex_buffer_,vertices,sizeof(Vertex),0,vertex_count_);
  delete [] vertices;
  return hr;
}

int Rectangle::BuildTransform() {
  world_ = XMMatrixTransformation2D(XMLoadFloat2(&XMFLOAT2(0,0)),
    0,
    XMLoadFloat2(&XMFLOAT2(scale_,scale_)),
    XMLoadFloat2(&XMFLOAT2(0,0)),
    angle_,
    XMLoadFloat2(&XMFLOAT2(x_,y_)));
  world_._43 = z_;
  return S_OK;
}

int Rectangle::Draw() {
  UINT stride = sizeof( Vertex );
  UINT offset = 0;
  context_->SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
  context_->SetVertexBuffers(0,1,&vertex_buffer_,&stride,&offset);
  return context_->Draw(vertex_count_,0);
}

Vertex* Rectangle::CreateVertices() {

  Vertex* vertices = new Vertex[4];
  vertices[0].pos = XMFLOAT3(0,0,0.0f);
  vertices[0].color = XMFLOAT4(1,1,1,1);
  vertices[1].pos = XMFLOAT3(width_,0,0.0f);
  vertices[1].color = XMFLOAT4(1,1,1,1);
  vertices[2].pos = XMFLOAT3(0,height_,0.0f);
  vertices[2].color = XMFLOAT4(1,1,1,1);
  vertices[3].pos = XMFLOAT3(width_,height_,0.0f);
  vertices[3].color = XMFLOAT4(1,1,1,1);
  vertex_count_ = 4;
  return vertices;
}

}
}