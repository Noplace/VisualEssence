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
#include <functional>
#include "../ve.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")

namespace _1 {

UINT vert_to_prim_D3DPT_POINTLIST(UINT i) {
  return i;
}

UINT vert_to_prim_D3DPT_LINELIST(UINT i) {
  return i>>1;
};

UINT vert_to_prim_D3DPT_LINESTRIP(UINT i) {
  return i-1;
};

UINT vert_to_prim_D3DPT_TRIANGLELIST(UINT i) {
  return i/3;
}

UINT vert_to_prim_D3DPT_TRIANGLESTRIP(UINT i) {
  return i-2;
}

UINT vert_to_prim_D3DPT_TRIANGLEFAN(UINT i) {
  return i-2;
}

UINT (*vert_to_prim)(UINT) = nullptr;

}

namespace graphics {

const D3DVERTEXELEMENT9 ContextD3D9::ve_xyzc[3] = {
  {0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION ,0},
  {0,12,D3DDECLTYPE_D3DCOLOR,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_COLOR ,0},
  D3DDECL_END()
};

const D3DVERTEXELEMENT9 ContextD3D9::ve_xyzcuv[4] = {
  {0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION ,0},
  {0,12,D3DDECLTYPE_D3DCOLOR,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_COLOR ,0},
  {0,16,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD ,0},
  D3DDECL_END()
};

ContextD3D9::ContextD3D9(): Context(),direct3d_(NULL),display_modes_(NULL),device_(NULL) {

}

ContextD3D9::~ContextD3D9() {

  if (device_ != NULL) {
    device_->Release();
    device_ = NULL;
  }

  if (direct3d_ != NULL) {
    direct3d_->Release();
    direct3d_ = NULL;
  }
}

int ContextD3D9::Initialize() {
  

  if( NULL == (direct3d_ = Direct3DCreate9(D3D_SDK_VERSION)))
    return E_FAIL;
  
  int count = direct3d_->GetAdapterModeCount(D3DADAPTER_DEFAULT,D3DFMT_X8R8G8B8);
  if (count == 0)
    return E_FAIL;

  display_modes_ = new D3DDISPLAYMODE[count];
  for (int i=0;i<count;++i) {
    direct3d_->EnumAdapterModes(D3DADAPTER_DEFAULT ,D3DFMT_X8R8G8B8,i,&display_modes_[i]);
  }

  return S_OK;
}

int ContextD3D9::Deinitialize() {
  delete []display_modes_;
  SafeRelease(&device_);
  SafeRelease(&direct3d_);
  return S_OK;
}

int ContextD3D9::CreateDisplay(core::windows::Window* window) {
  window_ = window;

  
  D3DDISPLAYMODE mode;
  direct3d_->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&mode);
  
  ZeroMemory( &d3dpp, sizeof(d3dpp) );
  d3dpp.Windowed   = TRUE;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.BackBufferFormat = mode.Format;
  d3dpp.hDeviceWindow = window_->handle();
  RECT rc;
  GetClientRect(window_->handle(),&rc);
  width_ = rc.right;
  height_ = rc.bottom;
  if( FAILED( direct3d_->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window_->handle(),
									  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									  &d3dpp, &device_ ) ) )
	  return E_FAIL;

  return S_OK;

}

int ContextD3D9::Render() {
  


  HRESULT result = device_->Present(NULL,NULL,NULL,NULL);
  if (result == D3DERR_DEVICELOST) {
    //signal resources to be recreated
    device_->Reset(&d3dpp);
  }

  return S_OK;
}

int ContextD3D9::ClearTarget() {
  return device_->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
}

int ContextD3D9::Begin() {
  return device_->BeginScene();
}

int ContextD3D9::End() {
  return device_->EndScene();
}

int ContextD3D9::CreateInputLayout(const InputElement inputs[], InputLayout& input_layout) {

  int element_count = 0;
  auto ptr = inputs;
  while (ptr->Stream != 0xFF && ptr->Type != D3DDECLTYPE_UNUSED) {
    ++ptr;
    ++element_count;
  }
  ++element_count;

  auto d3d9elements = new D3DVERTEXELEMENT9[element_count];
  for (int i=0;i<element_count; ++i) {
    d3d9elements[i].Method = inputs[i].Method;
    d3d9elements[i].Offset = inputs[i].Offset;
    d3d9elements[i].Stream = inputs[i].Stream;
    d3d9elements[i].Type = inputs[i].Type;
    d3d9elements[i].Usage = inputs[i].Usage;
    d3d9elements[i].UsageIndex = inputs[i].UsageIndex;
  }
  IDirect3DVertexDeclaration9* d3ddec;
  device_->CreateVertexDeclaration(d3d9elements,&d3ddec);
  input_layout.set_pointer(d3ddec);
  delete [] d3d9elements;
  return S_OK;
}

int ContextD3D9::DestoryInputLayout(InputLayout& input_layout) {
  auto decl = (IDirect3DVertexDeclaration9*)input_layout.pointer();
  decl->Release();
  return S_OK;
}

int ContextD3D9::SetInputLayout(InputLayout& input_layout) {
  auto decl = (IDirect3DVertexDeclaration9*)input_layout.pointer();
  device_->SetVertexDeclaration(decl);
  return S_OK;
}

int ContextD3D9::CreateBuffer(Buffer& buffer, void* initial_data) {
  if (buffer.internal_pointer != NULL)
    return S_FALSE;
  int result = device_->CreateVertexBuffer(buffer.description.byte_width,
          buffer.description.usage,
          0,
          D3DPOOL_DEFAULT,
          (IDirect3DVertexBuffer9**)&buffer.internal_pointer,
          NULL);
  if (initial_data != nullptr) {
    auto internal_buffer_ = (IDirect3DVertexBuffer9*)buffer.internal_pointer;
    void* buffer_pointer;
    internal_buffer_->Lock(0,buffer.description.byte_width,&buffer_pointer,D3DLOCK_DISCARD);
    memcpy(buffer_pointer,initial_data,buffer.description.byte_width);
    internal_buffer_->Unlock();
  }
  return result;
}

int ContextD3D9::DestroyBuffer(Buffer& buffer) {
  IDirect3DVertexBuffer9* internal_buffer_ = (IDirect3DVertexBuffer9*)buffer.internal_pointer;
  if (internal_buffer_ != NULL) {
    internal_buffer_->Release();
    internal_buffer_ = NULL;
    buffer.internal_pointer =  NULL;
    return S_OK;
  }
  return S_FALSE;
}

int ContextD3D9::UpdateSubresource(const Buffer& buffer, void* data_pointer, void* box, uint32_t row_size, uint32_t depth_size) {
  return S_FALSE;
}

int ContextD3D9::CopyToVertexBuffer(const Buffer& buffer, void* data_pointer, uint32_t type_size, uint32_t offset, uint32_t count) { 
  auto internal_buffer_ = (IDirect3DVertexBuffer9*)buffer.internal_pointer;
  void* buffer_pointer;
  internal_buffer_->Lock(offset*type_size,count*type_size,&buffer_pointer,D3DLOCK_DISCARD);
  memcpy(buffer_pointer,data_pointer,type_size*count);
  internal_buffer_->Unlock();
  return S_FALSE; 
};

int ContextD3D9::SetConstantBuffers(ShaderType shader_type, uint32_t start_slot, uint32_t buffer_count, Buffer* buffer_array) {
  return S_FALSE;
}

int ContextD3D9::SetVertexBuffers(uint32_t start_slot, uint32_t buffer_count, Buffer* buffers, const uint32_t* strides, const uint32_t* offsets) {
  int result = S_OK;
  for (uint32_t i=0;i<buffer_count;++i) {
    result = device_->SetStreamSource(start_slot+i,(IDirect3DVertexBuffer9*)buffers[i].internal_pointer,offsets[i],strides[i]);
  }
  return result;
}

int ContextD3D9::SetIndexBuffer(const Buffer& buffer, const uint32_t offset) {
  return S_FALSE;
}
int ContextD3D9::ClearIndexBuffer() {
    return S_FALSE;
}
int ContextD3D9::LockBuffer(void* buffer,uint32_t index,uint32_t type,BufferSubresource& subresource) { 
  return S_FALSE;
}
int ContextD3D9::UnlockBuffer(void* buffer,uint32_t index) {
  return S_FALSE;
}
int ContextD3D9::CompileShaderFromMemory(void* data, uint32_t len, LPCSTR szEntryPoint, LPCSTR szShaderModel, ShaderBlob& blob) {
  return S_FALSE;
}
int ContextD3D9::CreateVertexShader(void* data, uint32_t length, VertexShader& vs) {
  return S_FALSE;
}

int ContextD3D9::CreatePixelShader(void* data, uint32_t length, PixelShader& ps) {
  return S_FALSE;
}

int ContextD3D9::CreateGeometryShader(void* data, uint32_t length, GeometryShader& gs) {
  return S_FALSE;
}

int ContextD3D9::DestroyShader(Shader& shader) {
  if( shader.internal_pointer() ) 
    ((IUnknown*)shader.internal_pointer())->Release();
  return S_OK;
}

int ContextD3D9::SetShader(const Shader& shader) {
  return S_FALSE;
}

int ContextD3D9::ClearShader(ShaderType shader_type) {
  return S_FALSE;
}

int ContextD3D9::Draw(uint32_t vertex_count, uint32_t vertex_start_index) {
  device_->DrawPrimitive((D3DPRIMITIVETYPE)primitive_topology_,vertex_start_index,_1::vert_to_prim(vertex_count));
  return S_FALSE;
}

int ContextD3D9::SetShaderResources(ShaderType shader_type,uint32_t start_slot,uint32_t count,void** resources_pointer) {
  return S_FALSE;
}
     
int ContextD3D9::SetPrimitiveTopology(uint32_t topology) {
  if (primitive_topology_ == topology)
    return S_FALSE;
  primitive_topology_ = topology;
  switch (primitive_topology_) {
    case D3DPT_POINTLIST:
      _1::vert_to_prim = _1::vert_to_prim_D3DPT_POINTLIST;
    break;
    case D3DPT_LINELIST:
      _1::vert_to_prim = _1::vert_to_prim_D3DPT_LINELIST;
    break;
    case D3DPT_LINESTRIP:
     _1::vert_to_prim = _1::vert_to_prim_D3DPT_LINESTRIP;
    break;
    case D3DPT_TRIANGLELIST:
     _1::vert_to_prim = _1::vert_to_prim_D3DPT_TRIANGLELIST;
    break;
    case D3DPT_TRIANGLESTRIP:
      _1::vert_to_prim = _1::vert_to_prim_D3DPT_TRIANGLESTRIP;
    break;
    case D3DPT_TRIANGLEFAN:
      _1::vert_to_prim = _1::vert_to_prim_D3DPT_TRIANGLEFAN;
    break;
  }
  return S_OK;
}

int ContextD3D9::CreateTexture(uint32_t width, uint32_t height, uint32_t format, uint32_t type, Texture& texture) {
  //todo : change the defaults ( make them configurable )
  return device_->CreateTexture(width,height,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,(IDirect3DTexture9**)&texture.data_pointer,0);
}

int ContextD3D9::CreateTextureFromMemory(void* data_pointer, uint32_t data_length, Texture& texture) {
  texture.data_length = data_length;
  
  int result = D3DXCreateTextureFromFileInMemory(device_,data_pointer,data_length,(IDirect3DTexture9**)&texture.data_pointer);
  return result;
}
int ContextD3D9::DestroyTexture(Texture& texture) {
  SafeRelease((IDirect3DTexture9**)&texture.data_pointer);
  return S_OK;
}

int ContextD3D9::CopyToTexture(Texture& texture, void* data_pointer,uint32_t data_format, uint32_t data_pitch, const TexturePoint src_pos, const TexturePoint dest_pos, uint32_t w, uint32_t h) {
    auto d3dtex = (IDirect3DTexture9*)texture.data_pointer;
    D3DLOCKED_RECT lr;
    RECT rc = {dest_pos.x,dest_pos.y,dest_pos.x+w,dest_pos.x+h};
    d3dtex->LockRect(0,&lr,&rc,D3DLOCK_DISCARD);
    auto dest_bytes = (uint8_t*)lr.pBits;
    auto src_bytes = (uint8_t*)data_pointer;
    src_bytes += src_pos.y*(data_pitch<<2);
    for (int j=0;j<h;++j) {
      auto dest_line = (uint32_t*)dest_bytes;
      auto src_line = (uint32_t*)src_bytes;
      for (int i=0;i<w;++i) {
        dest_line[i] = src_line[i+src_pos.x];
      }
      dest_bytes += lr.Pitch;
      src_bytes += data_pitch<<2;
    }
    d3dtex->UnlockRect(0);
  return S_OK;
}

int ContextD3D9::CreateResourceView(Texture& texture,ResourceView& resource_view) {
  resource_view.data_pointer = texture.data_pointer;
  resource_view.data_length = texture.data_length;
  return S_OK;
}

int ContextD3D9::DestroyResourceView(ResourceView& resource_view) {
  return S_OK;
}

int ContextD3D9::SetCamera(Camera* camera) {
  camera_ = camera;
  device_->SetTransform(D3DTS_VIEW,(D3DMATRIX*)&camera_->view());
  device_->SetTransform(D3DTS_PROJECTION,(D3DMATRIX*)&camera_->projection());

  return S_OK;
}


int ContextD3D9::SetViewport(float x,float y,float w,float h,float min_depth,float max_depth) {
  D3DVIEWPORT9 vp;
  vp.MinZ = min_depth;
  vp.MaxZ = max_depth;
  vp.Height = (DWORD)h;
  vp.Width = (DWORD)w;
  vp.X = (DWORD)x;
  vp.Y = (DWORD)y;
  return device_->SetViewport(&vp);
}


}