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

namespace ve {
namespace shader { 

Shader2DHelper::Shader2DHelper() {
  memset(&cef_buffer,0,sizeof(cef_buffer));
}

Shader2DHelper::~Shader2DHelper() {

}

int Shader2DHelper::Initialize(Context* context) { 
  ve::Component::Initialize(context);
  effect_.Initialize(context_);
  camera_.Initialize(context);
  //camera_.Ortho2D();
  //camera_.UpdateConstantBuffer();
  cef_buffer.description.bind_flags = D3D11_BIND_CONSTANT_BUFFER;
  cef_buffer.description.usage = D3D11_USAGE_DEFAULT;
  cef_buffer.description.byte_width = sizeof(ConstantBuffer2Type);
  cef_buffer.description.cpu_access_flags = 0;
  int hr =  S_FALSE;throw new std::exception();//context_->CreateBuffer(cef_buffer,NULL);
  if( FAILED( hr ) )
      return hr;
  return S_OK; 
}

int Shader2DHelper::Deinitialize() { 
  if (context_!=nullptr) {
    //context_->DestroyBuffer(cef_buffer);
    throw new std::exception();
  }
  camera_.Deinitialize();
  effect_.Deinitialize();
  return S_OK; 
}

void Shader2DHelper::CreateFromMemory(void* data_pointer, size_t data_length) {
  ve::Effect::Entry entry_list[] = {
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

  context_->ClearShader(ve::kShaderTypeGeometry);
  //camera_.SetConstantBuffer(0);

  //bug
  //context_->SetConstantBuffers(ve::kShaderTypeVertex,2,1,&cef_buffer);
  //context_->SetConstantBuffers(ve::kShaderTypePixel,2,1,&cef_buffer);
  return S_OK;
}

int Shader2DHelper::UpdateChangesEveryFrame(ConstantBuffer2Type* cef) {
  return context_->UpdateSubresource(cef_buffer.internal_pointer,cef,NULL,0,0);
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