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

int SpriteBatch::Initialize(ve::Context* context) {
  ve::RenderObject::Initialize(context);

  auto vs_result = context_->shader_manager().RequestVertexShader("vs_spritebatch.cso",SpriteBatchVertexElementDesc,ARRAYSIZE(SpriteBatchVertexElementDesc));
  vs_ = vs_result.vs;
  input_layout_ = vs_result.il;
  auto ps_result = context_->shader_manager().RequestPixelShader("ps_spritebatch.cso");
  ps_ = ps_result.ps;

	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(SpriteBatchVertex)*4*max_sprite_count_, D3D11_BIND_VERTEX_BUFFER,D3D11_USAGE_DYNAMIC,D3D11_CPU_ACCESS_WRITE);
	context_->CreateBuffer(&vertexBufferDesc,nullptr,(void**)&vb_);

  vertices_data = new SpriteBatchVertex[max_sprite_count_<<2];



  //prepare index buffer for max sprites
  unsigned short* indices = new unsigned short[max_sprite_count_*6];
  auto indices_ptr = indices;
  for (int i=0;i<max_sprite_count_;++i) {
    *indices_ptr++ = (i<<2);
    *indices_ptr++ = (i<<2)+1;
    *indices_ptr++ = (i<<2)+2;
    *indices_ptr++ = (i<<2)+2;
    *indices_ptr++ = (i<<2)+1;
    *indices_ptr++ = (i<<2)+3;
  }
  D3D11_SUBRESOURCE_DATA InitData;
  ZeroMemory( &InitData, sizeof(InitData) );
  InitData.pSysMem = indices;
	CD3D11_BUFFER_DESC indexBufferDesc(max_sprite_count_*6*sizeof(uint16_t), D3D11_BIND_INDEX_BUFFER);
  context_->CreateBuffer(&indexBufferDesc,(void*)&InitData,(void**)&ib_);
  delete [] indices;

  CD3D11_BUFFER_DESC constantBufferDesc(sizeof(SpriteBatchVSConstantBuffer), D3D11_BIND_CONSTANT_BUFFER,D3D11_USAGE_DYNAMIC,D3D11_CPU_ACCESS_WRITE);
 	context_->CreateBuffer(&constantBufferDesc,nullptr, (void**)&vs_cb_ );
  vs_cb_data_ = new SpriteBatchVSConstantBuffer();
  
  auto gfx = (ve::ContextD3D11*)context_;
  D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
  ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
  depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = false;
	auto hr = gfx->device()->CreateDepthStencilState(&depthDisabledStencilDesc, &depth_state_);
    
  world_ = dx::XMMatrixIdentity();
  dirty_ = 0xffffffff;
  //UpdateTransform();
  return S_OK;
}

int SpriteBatch::Deinitialize() {
  delete [] vertices_data;
  delete vs_cb_data_;
  SafeRelease(&depth_state_);
  context_->DestroyBuffer((void**)&vs_cb_);
  context_->DestroyBuffer((void**)&ib_);
  context_->DestroyBuffer((void**)&vb_);
  context_->DestoryInputLayout(input_layout_);

  if (auto_garbage_cleanup_ == true) {
    for(auto i : render_list_) {
      i->Deinitialize();
      delete i;
    }
  }

  return S_OK;
}

int SpriteBatch::Render() {
  context_->PushDepthState(depth_state_);
  context_->SetInputLayout(input_layout_);
  context_->PushVertexShader(&vs_);
  context_->PushPixelShader(&ps_);
	UINT stride = sizeof(SpriteBatchVertex);
	UINT offset = 0;
  context_->SetConstantBuffers(ve::kShaderTypeVertex,3,1,(const void**)&vs_cb_);    
  context_->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  context_->SetVertexBuffers(0,1,(const void**)&vb_,&stride,&offset);
  context_->SetIndexBuffer((const void*)ib_,DXGI_FORMAT_R16_UINT,0);
  context_->DrawIndexed(6*(sprite_count_),0,0);
  context_->SetIndexBuffer(nullptr,0,0);
  context_->PopPixelShader();
  context_->PopVertexShader();
  context_->PopDepthState();
  return S_OK;
}

int SpriteBatch::AddSprite(SpriteBatchSprite* sprite) {
  sprite->index = sprite_count_;
  render_list_.push_back(sprite);
  sprite->set_parent(this);
  return sprite_count_++;
}
  
int SpriteBatch::Compile() {
  auto ptr = vertices_data;

  std::sort(render_list_.begin(), render_list_.end(), [](SpriteBatchSprite* a, SpriteBatchSprite* b) {
      return a->zorder() < b->zorder();   
  });

  for(auto i : render_list_) {
    i->GenerateVertices(ptr);
    ptr += 4;
  }
    
  int hr = context_->CopyBufferFast(vb_,vertices_data,sizeof(SpriteBatchVertex)*(sprite_count_<<2),0);
  return hr;
}


int SpriteBatch::UpdateVerticies() {
  if (dirty_&ve::kRenderObjectDirtySize) {
    int result = Compile();
    dirty_ &= ~ve::kRenderObjectDirtySize;
    return result;
  } else return S_FALSE;
}

int SpriteBatch::UpdateTransform() {
	if (dirty_&ve::kRenderObjectDirtyTransform) {
      
      //world_ = dx::XMMatrixAffineTransformation2D(dx::XMVectorSet(scale_,scale_,1,0),
      //        dx::XMVectorSet(w_/2.0f,h_/2.0f,0,0),angle_,dx::XMVectorSet(x_,y_,0,0));


    //int index =0;
    for(auto i : render_list_) {
      i->GenerateTransform(&vs_cb_data_->sprite_info[i->index]);
    }
    
    //context_->UpdateSubresource(vs_cb_,vs_cb_data_,nullptr,0,0);
    context_->CopyBufferFast(vs_cb_,vs_cb_data_,sizeof(SpriteBatchVSConstantBuffer),0);
    
    dirty_ &= ~ve::kRenderObjectDirtyTransform;
    return S_OK;
  } else return S_FALSE;
}
 

}