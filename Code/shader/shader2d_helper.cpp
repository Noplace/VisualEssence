#include "../ve.h"

namespace graphics {
namespace shader { 

Shader2DHelper::Shader2DHelper() {
  memset(&cef_buffer,0,sizeof(cef_buffer));
}

Shader2DHelper::~Shader2DHelper() {

}

int Shader2DHelper::Initialize(Context* context) { 
  graphics::Component::Initialize(context);
  effect_.Initialize(context_);
  camera_.Initialize(context);
  camera_.Ortho2D();
  camera_.UpdateConstantBuffer();
  cef_buffer.description.bind_flags = D3D11_BIND_CONSTANT_BUFFER;
  cef_buffer.description.usage = D3D11_USAGE_DEFAULT;
  cef_buffer.description.byte_width = sizeof(ConstantBuffer2Type);
  cef_buffer.description.cpu_access_flags = 0;
  int hr =  context_->CreateBuffer(cef_buffer,NULL);
  if( FAILED( hr ) )
      return hr;
  return S_OK; 
}

int Shader2DHelper::Deinitialize() { 
  if (context_!=nullptr) {
    context_->DestroyBuffer(cef_buffer);
  }
  camera_.Deinitialize();
  effect_.Deinitialize();
  return S_OK; 
}

void Shader2DHelper::CreateFromMemory(void* data_pointer, uint32_t data_length) {
  graphics::Effect::Entry entry_list[] = {
    {"VS","vs_5_0"},
    {"PS","ps_5_0"},
    {"PSTex","ps_5_0"},
    {"PSFont","ps_5_0"},
    {NULL,NULL},
  };
  effect_.CreateFromMemory2(entry_list,data_pointer,data_length);
}

int Shader2DHelper::PrepareDraw() {
  context_->SetInputLayout(effect_.input_layout());
  context_->SetShader(effect_.vs(0));
  context_->SetShader(effect_.ps(0));

  context_->ClearShader(graphics::kShaderTypeGeometry);
  camera_.SetConstantBuffer(0);

  context_->SetConstantBuffers(graphics::kShaderTypeVertex,2,1,&cef_buffer);
  context_->SetConstantBuffers(graphics::kShaderTypePixel,2,1,&cef_buffer);
  return S_OK;
}

int Shader2DHelper::UpdateChangesEveryFrame(ConstantBuffer2Type* cef) {
  return context_->UpdateSubresource(cef_buffer,cef,NULL,0,0);
}

int Shader2DHelper::SetColorShader() {
  return context_->SetShader(effect_.ps(0));
}

int Shader2DHelper::SetTextureShader() {
  return context_->SetShader(effect_.ps(1));
}

int Shader2DHelper::SetTexturePagesShader() {
  return context_->SetShader(effect_.ps(2));
}

}
}