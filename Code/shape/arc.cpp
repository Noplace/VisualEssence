#define _USE_MATH_DEFINES
#include <math.h>
#include "../context/context.h"
#include "arc.h"


namespace graphics {
namespace shape {

int Arc::Initialize(Context* context) {
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

int Arc::Deinitialize() {
  int hr = context_->DestroyBuffer(vertex_buffer_);
  return hr;
}

int Arc::SetColor(XMCOLOR color) {
  color_ = color;
  return S_OK;
}

int Arc::SetParams(float radius,float start_angle,float end_angle,float thickness) {
  radius_ = radius;
  start_angle_ = start_angle;
  end_angle_ = end_angle;
  thickness_ = thickness;
  return S_OK;
}

int Arc::Construct() {
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

int Arc::BuildTransform() {
  world_ = XMMatrixTransformation2D(XMLoadFloat2(&XMFLOAT2(0,0)),
    0,
    XMLoadFloat2(&XMFLOAT2(scale_,scale_)),
    XMLoadFloat2(&XMFLOAT2(0,0)),
    angle_,
    XMLoadFloat2(&XMFLOAT2(x_,y_)));
  world_._43 = z_;
  //world_ = XMMatrixRotationZ(angle_);
  //world_ *= XMMatrixTranslation(x_,y_,0);
  //world_ = world_ * XMMatrixScaling(scale_,scale_,1);
  return S_OK;
}

int Arc::Draw() {
  UINT stride = sizeof( Vertex );
  UINT offset = 0;
  context_->SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
  context_->SetVertexBuffers(0,1,&vertex_buffer_,&stride,&offset);
  return context_->Draw(vertex_count_,0);
}

Vertex* Arc::CreateVertices() {
  
  float step = M_PI/60;
  int count = (int)((end_angle_-start_angle_) / step) + 1;
  count *= 2;
  Vertex* vertices = new Vertex[count];
  float theta = start_angle_;
  int index=0;
  while (theta < end_angle_) {
    float x = radius_*cos(theta);
    float y = -radius_*sin(theta);
    float x2 = (radius_-thickness_)*cos(theta);
    float y2 = -(radius_-thickness_)*sin(theta);
    theta += step;

    vertices[index].pos = XMFLOAT3(x,y,0.0f);
    vertices[index].color = XMFLOAT4(1,1,1,1);
    ++index;
    vertices[index].pos = XMFLOAT3(x2,y2,0.0f);
    vertices[index].color = XMFLOAT4(1,1,1,1);
    ++index;
  }
  vertex_count_ = index;
  return vertices;
}

}
}