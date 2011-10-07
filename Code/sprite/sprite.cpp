#include "sprite.h"

namespace graphics {

int Sprite::Initialize(Context* context) {
  int hr = Component::Initialize(context);

  if (hr != S_OK)
    return hr;
  x_ = 0;
  y_ = 0;
  scale_ = 1;
  angle_ = 0;
  world_ = XMMatrixIdentity();
  vertex_buffer_.description.bind_flags = D3D11_BIND_VERTEX_BUFFER;
  vertex_buffer_.description.usage = D3D11_USAGE_DEFAULT;
  vertex_buffer_.description.byte_width = sizeof( graphics::shape::Vertex ) * 4;
  vertex_buffer_.description.cpu_access_flags = 0;
  context->CreateBuffer(vertex_buffer_,NULL);

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
    graphics::shape::Vertex(XMFLOAT3( 0, 0 ,0.0f), XMFLOAT2( u0, v0  ),XMFLOAT4(1,1,1,1),0),  
    graphics::shape::Vertex(XMFLOAT3( width_, 0,0.0f ), XMFLOAT2( u1, v0 ),XMFLOAT4(1,1,1,1),0),
    graphics::shape::Vertex(XMFLOAT3( 0, height_,0.0f ), XMFLOAT2( u0, v1 ),XMFLOAT4(1,1,1,1),0),
    graphics::shape::Vertex(XMFLOAT3( width_, height_,0.0f ), XMFLOAT2( u1, v1 ),XMFLOAT4(1,1,1,1),0)
  };
  return context_->CopyToVertexBuffer(vertex_buffer_,vertices,sizeof(graphics::shape::Vertex),0,4);
}

int Sprite::BuildTransform() {
  world_ = XMMatrixTransformation2D(XMLoadFloat2(&XMFLOAT2(width_*0.5f,height_*0.5f)),
    0,
    XMLoadFloat2(&XMFLOAT2(scale_,scale_)),
    XMLoadFloat2(&XMFLOAT2(width_*0.5f,height_*0.5f)),
    angle_,
    XMLoadFloat2(&XMFLOAT2(x_,y_)));
  world_._43 = z_;
  //world_ = XMMatrixRotationZ(angle_);
  //world_ *= XMMatrixTranslation(x_,y_,0);
  //world_ = world_ * XMMatrixScaling(scale_,scale_,1);
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