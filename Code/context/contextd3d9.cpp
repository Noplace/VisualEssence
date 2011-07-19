#include "contextd3d9.h"

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

int ContextD3D9::CreateBuffer(BufferDescription& buffer_description,void** buffer_pointer) {
  return device_->CreateVertexBuffer(buffer_description.byte_width,
           buffer_description.usage,
           0,
           D3DPOOL_MANAGED,
           (IDirect3DVertexBuffer9**)buffer_pointer,
           NULL);
}

int ContextD3D9::SetVertexBuffers(uint32_t start_slot, uint32_t buffer_count, void *const * buffers, const uint32_t * strides,const uint32_t *offsets) {
  return device_->SetStreamSource(start_slot,(IDirect3DVertexBuffer9*)*buffers,offsets[0],strides[0]);
}

}