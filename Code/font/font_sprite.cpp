#include "../ve.h"


namespace graphics {
namespace font {


int FontSprite::Initialize(Context* context) {
  Component::Initialize(context);


  return S_OK;
}

int FontSprite::Deinitialize() {
  //context_->DestroyBuffer(misc_buffer_);
  context_->DestroyBuffer(vertex_buffer_);
  Component::Deinitialize();
  return S_OK;
}

int FontSprite::Draw() {

  //effect_->Begin();

  UINT stride = sizeof( graphics::shape::Vertex );
  UINT offset = 0;
  context_->SetVertexBuffers(0,1,&vertex_buffer_,&stride,&offset);
  context_->ClearIndexBuffer();
  context_->SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  //camera_.SetConstantBuffer(0);

  //set contant buffer;
  //context_->SetConstantBuffers(kShaderTypeVertex,2,1,&misc_buffer_);
  //context_->SetConstantBuffers(kShaderTypePixel,2,1,&misc_buffer_);
  //context_->SetShaderResources(kShaderTypePixel,1,1,(void**)&font_->pages);

  context_->Draw(char_count*6,0);
  return S_OK;
}

}
}