#include "../ve.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")


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

int ContextD3D9::DestoryInputLayout(InputLayout& input_layout) {
  return S_FALSE;
}

int ContextD3D9::SetInputLayout(InputLayout& input_layout) {
  return S_FALSE;
}

int ContextD3D9::CreateBuffer(Buffer& buffer, void* initial_data) {
  if (buffer.internal_pointer != NULL)
    return S_FALSE;
  return device_->CreateVertexBuffer(buffer.description.byte_width,
          buffer.description.usage,
          0,
          D3DPOOL_MANAGED,
          (IDirect3DVertexBuffer9**)&buffer.internal_pointer,
          NULL);
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
  return S_FALSE; 
};

int ContextD3D9::SetConstantBuffers(ShaderType shader_type, uint32_t start_slot, uint32_t buffer_count, Buffer* buffer_array) {
  return S_FALSE;
}

int ContextD3D9::SetVertexBuffers(uint32_t start_slot, uint32_t buffer_count, Buffer* buffers, const uint32_t* strides, const uint32_t* offsets) {
  return device_->SetStreamSource(start_slot,(IDirect3DVertexBuffer9*)buffers->internal_pointer,offsets[0],strides[0]);
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
  return S_FALSE;
}
int ContextD3D9::SetShaderResources(ShaderType shader_type,uint32_t start_slot,uint32_t count,void** resources_pointer) {
  return S_FALSE;
}
int ContextD3D9::SetPrimitiveTopology(uint32_t topology) {
  return S_FALSE;
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
int ContextD3D9::CreateResourceView(Texture& texture,ResourceView& resource_view) {
  resource_view.data_pointer = texture.data_pointer;
  resource_view.data_length = texture.data_length;
  return S_OK;
}
int ContextD3D9::DestroyResourceView(ResourceView& resource_view) {
  return S_OK;
}


}