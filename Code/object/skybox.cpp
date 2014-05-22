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
#include <VisualEssence/Code/util/DDSTextureLoader.h>

namespace ve {

int SkyBox::Initialize(Context* context) { 
  RenderObject::Initialize(context);
  sky_vb_ = nullptr;
  world_ = dx::XMMatrixIdentity();
  auto sky_vs_result = context_->shader_manager().RequestVertexShader("vs_skybox.cso",ve::SkyBoxVertexElementDesc,ARRAYSIZE(ve::SkyBoxVertexElementDesc));
  sky_vs_ = sky_vs_result.vs;
  il_ = sky_vs_result.il;
  auto sky_ps_result = context_->shader_manager().RequestPixelShader("ps_skybox.cso");
  sky_ps_ = sky_ps_result.ps;

  auto gfx = (ContextD3D11*)context;
  D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
  ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
  depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = false;
  auto hr = gfx->device()->CreateDepthStencilState(&depthDisabledStencilDesc, &sky_depth_state_);


  // Query support for linear filtering on DXGI_FORMAT_R32G32B32A32
  UINT FormatSupport = 0;
  gfx->device()->CheckFormatSupport( DXGI_FORMAT_R32G32B32A32_FLOAT, &FormatSupport );

  // Setup linear or point sampler according to the format Query result
  D3D11_SAMPLER_DESC SamDesc;
  SamDesc.Filter = ( FormatSupport & D3D11_FORMAT_SUPPORT_SHADER_SAMPLE ) > 0 ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
  SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  SamDesc.MipLODBias = 0.0f;
  SamDesc.MaxAnisotropy = 1;
  SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
  SamDesc.BorderColor[0] = SamDesc.BorderColor[1] = SamDesc.BorderColor[2] = SamDesc.BorderColor[3] = 0;
  SamDesc.MinLOD = 0;
  SamDesc.MaxLOD = D3D11_FLOAT32_MAX;
  gfx->device()->CreateSamplerState( &SamDesc, &sampler_state );

  // Setup constant buffer
  D3D11_BUFFER_DESC Desc;
  Desc.Usage = D3D11_USAGE_DYNAMIC;
  Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  Desc.MiscFlags = 0;
  Desc.ByteWidth = sizeof( CB_VS_PER_OBJECT );
  gfx->device()->CreateBuffer( &Desc, NULL, &m_pcbVSPerObject );


  DirectX::CreateDDSTextureFromFile(gfx->device(),L"D:\\Personal\\Projects\\GameDemo2\\Solution\\GameDemo2\\Resources\\sky_cube.dds",(ID3D11Resource**)&sky_tex_.data_pointer,(ID3D11ShaderResourceView**)&sky_trv_.data_pointer);
  


  return S_OK; 
}

int SkyBox::Deinitialize() { 
 
  SafeRelease( &sampler_state );
  context_->DestroyShader(sky_vs_);
  context_->DestroyShader(sky_ps_);
  context_->DestoryInputLayout(il_);
  context_->DestroyResourceView(sky_trv_);
  context_->DestroyTexture(sky_tex_);
  SafeRelease( &m_pcbVSPerObject );
  SafeRelease( &sky_rasterizer_state_ );
  SafeRelease( &sky_depth_state_ );
  RenderObject::Deinitialize();
  return S_OK; 
}

int SkyBox::OnWindowSizeChange() {
    // Fill the vertex buffer
  SkyBoxVertex* pVertex = new SkyBoxVertex[4];
  if ( !pVertex )
      return S_FALSE;

  // Map texels to pixels 
  float fHighW = -1.0f - ( 1.0f / ( float )context_->width() );
  float fHighH = -1.0f - ( 1.0f / ( float )context_->height() );
  float fLowW = 1.0f + ( 1.0f / ( float )context_->width() );
  float fLowH = 1.0f + ( 1.0f / ( float )context_->height() );
    
  pVertex[0].pos = dx::XMFLOAT4( fLowW, fLowH, 1.0f, 1.0f );
  pVertex[1].pos = dx::XMFLOAT4( fLowW, fHighH, 1.0f, 1.0f );
  pVertex[2].pos = dx::XMFLOAT4( fHighW, fLowH, 1.0f, 1.0f );
  pVertex[3].pos = dx::XMFLOAT4( fHighW, fHighH, 1.0f, 1.0f );

  UINT uiVertBufSize = 4 * sizeof( SkyBoxVertex );
  //Vertex Buffer
  D3D11_BUFFER_DESC vbdesc;
  vbdesc.ByteWidth = uiVertBufSize;
  vbdesc.Usage = D3D11_USAGE_IMMUTABLE;
  vbdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vbdesc.CPUAccessFlags = 0;
  vbdesc.MiscFlags = 0;    

  D3D11_SUBRESOURCE_DATA InitData;
  InitData.pSysMem = pVertex;    
  context_->DestroyBuffer((void**)&sky_vb_);
  context_->CreateBuffer(&vbdesc, &InitData, (void**)&sky_vb_);
  delete [] pVertex; 

  return S_OK;
}

int SkyBox::Update(float delta, float total) {
  return S_OK;
}

int SkyBox::UpdateVerticies() {
  return S_OK;
}

int SkyBox::UpdateTransform() {
  return S_OK;
}

int SkyBox::Render() {
  HRESULT hr;
    auto gfx = (ContextD3D11*)context_;
    auto pd3dImmediateContext = gfx->device_context();
  
    context_->SetInputLayout(il_);

    UINT uStrides = sizeof( SkyBoxVertex );
    UINT uOffsets = 0;
    context_->SetVertexBuffers( 0, 1, (const void**)&sky_vb_, &uStrides, &uOffsets );
    context_->SetIndexBuffer(nullptr,DXGI_FORMAT_R32_UINT,0);
    context_->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    context_->PushVertexShader(&sky_vs_);
    context_->PushPixelShader(&sky_ps_);

    D3D11_MAPPED_SUBRESOURCE MappedResource;
    pd3dImmediateContext->Map( m_pcbVSPerObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource ) ;
    CB_VS_PER_OBJECT* pVSPerObject = ( CB_VS_PER_OBJECT* )MappedResource.pData;  
    pmWorldViewProj = world()*dx::XMMatrixMultiply(scene_->camera()->view(),scene_->camera()->projection());
    pVSPerObject->m_WorldViewProj = dx::XMMatrixTranspose(dx::XMMatrixInverse(NULL, pmWorldViewProj ));
    pd3dImmediateContext->Unmap( m_pcbVSPerObject, 0 );
    pd3dImmediateContext->VSSetConstantBuffers( 0, 1, &m_pcbVSPerObject );

    pd3dImmediateContext->PSSetSamplers( 0, 1, &sampler_state );
    pd3dImmediateContext->PSSetShaderResources( 0, 1, (ID3D11ShaderResourceView*const*)&sky_trv_.data_pointer );

    ID3D11DepthStencilState* pDepthStencilStateStored11 = NULL;
    UINT StencilRef;
    pd3dImmediateContext->OMGetDepthStencilState( &pDepthStencilStateStored11, &StencilRef );
    pd3dImmediateContext->OMSetDepthStencilState( sky_depth_state_, 0 );

    pd3dImmediateContext->Draw( 4, 0 );

    pd3dImmediateContext->OMSetDepthStencilState( pDepthStencilStateStored11, StencilRef );

    context_->PopVertexShader();
    context_->PopPixelShader();
  return S_OK;
}


}