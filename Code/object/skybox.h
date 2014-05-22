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
#pragma once

namespace ve {

struct SkyBoxVertex {
    dx::XMFLOAT4 pos;
};

const D3D11_INPUT_ELEMENT_DESC SkyBoxVertexElementDesc[] = {
    { "POSITION",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


class SkyBox : public RenderObject {
 public:
  void *operator new( size_t stAllocateBlock) {
    return _aligned_malloc(sizeof(SkyBox),16);
  }

  void   operator delete (void* p)  {
    return _aligned_free(p);
  }
  SkyBox() : RenderObject() {}
  virtual ~SkyBox() {}
  int Initialize(Context* context);
  int Deinitialize();
  int OnWindowSizeChange();
  int UpdateVerticies();
  int UpdateTransform();
  int Update(float,float);
  int Render();
  dx::XMMATRIX pmWorldViewProj;
 protected:
  struct CB_VS_PER_OBJECT
    {
        dx::XMMATRIX m_WorldViewProj;
    };    
  ID3D11SamplerState* sampler_state;
	ID3D11DepthStencilState* sky_depth_state_;
  ID3D11RasterizerState* sky_rasterizer_state_;
  InputLayout il_;
  ID3D11Buffer* sky_vb_;
  ID3D11Buffer* m_pcbVSPerObject;
  PixelShader sky_ps_;
  VertexShader sky_vs_;
  ID3D11ShaderResourceView* m_pEnvironmentRV11;
  ve::Texture sky_tex_;
  ve::ResourceView sky_trv_;
};

}

