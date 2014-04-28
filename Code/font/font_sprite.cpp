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
namespace font {


int FontSprite::Initialize(Context* context) {
  Component::Initialize(context);


  return S_OK;
}

int FontSprite::Deinitialize() {
  //context_->DestroyBuffer(misc_buffer_);
  //context_->DestroyBuffer(vertex_buffer_);
  throw new std::exception();
  Component::Deinitialize();
  return S_OK;
}

int FontSprite::Draw() {

  //effect_->Begin();

  UINT stride = sizeof( ve::shape::Vertex );
  UINT offset = 0;
//  context_->SetVertexBuffers(0,1,&vertex_buffer_,&stride,&offset);
throw new std::exception();
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