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

#pragma comment(lib, "d3d11.lib")
#ifdef _DEBUG 
//#pragma comment(lib, "d3dx11d.lib")
#else
#pragma comment(lib, "d3dx11.lib")
#endif

//#pragma comment(lib, "dxgi.lib")
#pragma comment( lib, "dxguid.lib")

namespace ve {
  /*
const D3DVERTEXELEMENT9 ContextD3D11::ve_xyzc[3] = {
  {0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION ,0},
  {0,12,D3DDECLTYPE_D3DCOLOR,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_COLOR ,0},
  D3DDECL_END()
};

const D3DVERTEXELEMENT9 ContextD3D11::ve_xyzcuv[4] = {
  {0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION ,0},
  {0,12,D3DDECLTYPE_D3DCOLOR,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_COLOR ,0},
  {0,16,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD ,0},
  D3DDECL_END()
};
*/
ContextD3D11::ContextD3D11(): Context(),device_(NULL),device_context_(NULL),
depth_stencil_(NULL),render_target_view_(NULL),swap_chain_(NULL),depth_stencil_view_(NULL),default_depth_state(NULL),default_blend_state(NULL){

}

ContextD3D11::~ContextD3D11() {
  Deinitialize();
}

int ContextD3D11::Initialize() {
  memset(&settings,0,sizeof(settings));
  int hr = S_OK;
  /*int hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory_);
  if (SUCCEEDED(hr))  {
      // Create a WIC factory
      hr = CoCreateInstance(
          CLSID_WICImagingFactory,
          NULL,
          CLSCTX_INPROC_SERVER,
          IID_IWICImagingFactory,
          reinterpret_cast<void **>(&wic_factory_)
          );
  }
  if (SUCCEEDED(hr))  {
      // Create DWrite factory
      hr = DWriteCreateFactory(
          DWRITE_FACTORY_TYPE_SHARED,
          __uuidof(dwrite_factory_),
          reinterpret_cast<IUnknown **>(&dwrite_factory_)
          );
  }*/
  shader_manager_.Initialize(this);
  return hr;
}

int ContextD3D11::Deinitialize() {
    if( device_context_ )  device_context_->ClearState();
    
    shader_manager_.Deinitialize();
    SafeRelease(&depth_stencil_);
    //SafeRelease(&d2d_render_target_);
    SafeRelease(&depth_stencil_view_);
    SafeRelease(&render_target_view_);
    SafeRelease(&swap_chain_);
    SafeRelease(&device_context_);
    SafeRelease(&device_);
    SafeRelease(&default_depth_state);
    SafeRelease(&default_blend_state);
    /*SafeRelease(&dwrite_factory_);
    SafeRelease(&wic_factory_);
    SafeRelease(&d2d_factory_);
    */

    return S_OK;
}

int ContextD3D11::CreateDisplay(HWND window) {
  window_handle_ = window;
  int hr = S_OK;

  
  


/*
  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory( &sd, sizeof( sd ) );
  sd.BufferCount = 2;
  sd.BufferDesc.Width = width_;
  sd.BufferDesc.Height = height_;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = window_handle_;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  
  hr = D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, 0,
                                          D3D11_SDK_VERSION, &sd, &swap_chain_, &device_, &feature_level, &device_context_ );
  int inta = GetLastError();
  if( FAILED( hr ) )
      return hr;

  Resize(width_,height_);
  /*
  IDXGISurface *pBackBuffer = NULL;
  hr = swap_chain_->GetBuffer( 0, IID_PPV_ARGS(&pBackBuffer) );
  if( FAILED( hr ) )
      return hr;

  FLOAT dpiX;
  FLOAT dpiY;
  d2d_factory_->GetDesktopDpi(&dpiX, &dpiY);

  D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),0,0);

  // Create a Direct2D render target which can draw into the surface in the swap chain
  //SafeRelease(&m_pBackBufferRT);
  hr = d2d_factory_->CreateDxgiSurfaceRenderTarget(pBackBuffer,&props,&d2d_render_target_);
    
  SafeRelease(&pBackBuffer);*/
  CreateDeviceResources();
  return CreateWindowSizeDependentResources();

}



int ContextD3D11::CreateDeviceResources() {
  UINT creationFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;//D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
  creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_DRIVER_TYPE driverTypes[] =
  {
      D3D_DRIVER_TYPE_HARDWARE,
      D3D_DRIVER_TYPE_WARP,
      D3D_DRIVER_TYPE_REFERENCE,
  };
  UINT numDriverTypes = ARRAYSIZE( driverTypes );

  D3D_FEATURE_LEVEL featureLevels[] =
  {
    D3D_FEATURE_LEVEL_11_1,
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
    D3D_FEATURE_LEVEL_9_3,
    D3D_FEATURE_LEVEL_9_2,
    D3D_FEATURE_LEVEL_9_1
  };
  UINT numFeatureLevels = ARRAYSIZE( featureLevels );
  D3D_FEATURE_LEVEL feature_level;
  ID3D11Device* device;
  ID3D11DeviceContext* devicecontext;
  D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,	creationFlags, featureLevels, ARRAYSIZE(featureLevels),	D3D11_SDK_VERSION, &device, &feature_level, &devicecontext);
  device_ = (ID3D11Device1*)device;
  device_context_ = (ID3D11DeviceContext1*)devicecontext;

  SafeRelease(&default_blend_state);
  D3D11_BLEND_DESC BlendStateDescription;
  ZeroMemory(&BlendStateDescription,sizeof(BlendStateDescription));
  BlendStateDescription.AlphaToCoverageEnable = false;
  BlendStateDescription.RenderTarget[0].BlendEnable           = true;
  BlendStateDescription.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;        //D3D11_BLEND_SRC_COLOR;
  BlendStateDescription.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;//D3D11_BLEND_DEST_COLOR;
  BlendStateDescription.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_SRC_ALPHA;//D3D11_BLEND_SRC_ALPHA;
  BlendStateDescription.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_INV_SRC_ALPHA;//D3D11_BLEND_DEST_ALPHA;
  BlendStateDescription.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
  BlendStateDescription.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
  BlendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    
  device_->CreateBlendState(&BlendStateDescription,&default_blend_state);
  float blendFactor[] = {1,1, 1, 1};
  UINT sampleMask   = 0xffffffff;
  device_context_->OMSetBlendState(default_blend_state,blendFactor,sampleMask);

  return S_OK;
}

int ContextD3D11::CreateWindowSizeDependentResources() { 
  RECT rc;
  GetClientRect( window_handle_, &rc );
  width_ = rc.right - rc.left;
  height_ = rc.bottom - rc.top;


  if(swap_chain_ != nullptr)
  {
      swap_chain_->ResizeBuffers(2,	static_cast<UINT>(width_),static_cast<UINT>(height_),	DXGI_FORMAT_B8G8R8A8_UNORM,	0);
  }
  else
  {
    // Otherwise, create a new one using the same adapter as the existing Direct3D device.
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
    swapChainDesc.Width = static_cast<UINT>(width_); // Match the size of the window.
    swapChainDesc.Height = static_cast<UINT>(height_);
    swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swap chain format.
    swapChainDesc.Stereo = false;
    swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2; // Use double-buffering to minimize latency.
    swapChainDesc.Scaling = DXGI_SCALING_NONE;
    
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Windows Store apps must use this SwapEffect.
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    
    IDXGIDevice1* dxgiDevice;
    device_->QueryInterface(__uuidof(dxgiDevice),(void**)&dxgiDevice);
    IDXGIAdapter* dxgiAdapter;
    dxgiDevice->GetAdapter(&dxgiAdapter);
    IDXGIFactory2* dxgiFactory;
    dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), 		(void**)&dxgiFactory		);

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreen_desc;
    memset(&fullscreen_desc,0,sizeof(fullscreen_desc));
    fullscreen_desc.Windowed = true;
    dxgiFactory->CreateSwapChainForHwnd(device_,window_handle_,	&swapChainDesc,	&fullscreen_desc,nullptr, &swap_chain_);
    dxgiDevice->SetMaximumFrameLatency(1);

    SafeRelease(&dxgiFactory);
    SafeRelease(&dxgiAdapter);
    SafeRelease(&dxgiDevice);
  }
  
  ID3D11Texture2D* backBuffer;
  swap_chain_->GetBuffer(0,__uuidof(ID3D11Texture2D),(void**)&backBuffer);
  device_->CreateRenderTargetView(backBuffer,nullptr,&render_target_view_);
  SafeRelease(&backBuffer);
  // Create a depth stencil view.
  CD3D11_TEXTURE2D_DESC depthStencilTextureDesc(
    DXGI_FORMAT_D24_UNORM_S8_UINT, 
    static_cast<UINT>(width_),
    static_cast<UINT>(height_),
    1,
    0,
    D3D11_BIND_DEPTH_STENCIL
    );

  ID3D11Texture2D* depth_stencil_tex;
  device_->CreateTexture2D(&depthStencilTextureDesc,nullptr,&depth_stencil_tex);
  CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
  device_->CreateDepthStencilView(depth_stencil_tex,&depthStencilViewDesc,&depth_stencil_view_);
  SafeRelease(&depth_stencil_tex);

  SafeRelease(&default_depth_state);
  D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  device_->CreateDepthStencilState(&depthStencilDesc, &default_depth_state);
  SetDepthState(null);

  // Set the rendering viewport to target the entire window.
  CD3D11_VIEWPORT viewport(0.0f,0.0f,width_,height_);

  device_context_->RSSetViewports(1, &viewport);
  device_context_->OMSetRenderTargets( 1, &render_target_view_, depth_stencil_view_ );
  return S_OK;
}

// This method is called in the event handler for the SizeChanged event.
int ContextD3D11::UpdateForWindowSizeChange()
{
  /*if (m_window->Bounds.Width  != m_windowBounds.Width ||
    m_window->Bounds.Height != m_windowBounds.Height ||
    m_orientation != DisplayProperties::CurrentOrientation)*/
  {
    ID3D11RenderTargetView* nullViews[] = {nullptr};
    device_context_->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
    SafeRelease(&render_target_view_);
    SafeRelease(&depth_stencil_view_);
    device_context_->Flush();
    CreateWindowSizeDependentResources();
  }
  return S_OK;
}

int ContextD3D11::Resize(uint32_t width, uint32_t height) {
  /*int hr = S_OK;

  //Ensure that nobody is holding onto one of the old resources
  SafeRelease(&render_target_view_);
  ID3D11RenderTargetView *viewList[1] = {NULL};
  device_context_->OMSetRenderTargets(1, viewList, NULL);

  // Resize render target buffers
  if ( width != width_ || height_ != height) {
    width_ = width;
    height_ = height;
    hr = swap_chain_->ResizeBuffers(1, width_, height_, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
    if( FAILED( hr ) )
      return hr;
  }

  // Create a render target view
  ID3D11Texture2D* pBackBufferResource = NULL;
  hr = swap_chain_->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBufferResource );
  if( FAILED( hr ) )
      return hr;

  hr = device_->CreateRenderTargetView( pBackBufferResource, NULL, &render_target_view_ );
  SafeRelease(&pBackBufferResource);
    
  // Create depth stencil texture
  D3D11_TEXTURE2D_DESC descDepth;
  ZeroMemory( &descDepth, sizeof(descDepth) );
  descDepth.Width = width_;
  descDepth.Height = height_;
  descDepth.MipLevels = 1;
  descDepth.ArraySize = 1;
  descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  descDepth.SampleDesc.Count = 1;
  descDepth.SampleDesc.Quality = 0;
  descDepth.Usage = D3D11_USAGE_DEFAULT;
  descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  descDepth.CPUAccessFlags = 0;
  descDepth.MiscFlags = 0;
  SafeRelease(&depth_stencil_);
  hr = device_->CreateTexture2D( &descDepth, NULL, &depth_stencil_ );
  if( FAILED( hr ) )
      return hr;
  
  // Create the depth stencil view
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  ZeroMemory( &descDSV, sizeof(descDSV) );
  descDSV.Format = descDepth.Format;
  descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  descDSV.Texture2D.MipSlice = 0;
  SafeRelease(&depth_stencil_view_);
  hr = device_->CreateDepthStencilView( depth_stencil_, &descDSV, &depth_stencil_view_ );
  if( FAILED( hr ) )
      return hr;

  device_context_->OMSetRenderTargets( 1, &render_target_view_, depth_stencil_view_ );

  // Setup the viewport
  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)width_;
  vp.Height = (FLOAT)height_;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  device_context_->RSSetViewports( 1, &vp );


  
  SafeRelease(&default_depth_state);
  D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
  // Clear the second depth stencil state before setting the parameters.
  ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
  // Default depth stencil state
  depthDisabledStencilDesc.DepthEnable = true;
  depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
  depthDisabledStencilDesc.StencilEnable = false;
  depthDisabledStencilDesc.StencilReadMask = 0xFF;
  depthDisabledStencilDesc.StencilWriteMask = 0xFF;
  depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  device_->CreateDepthStencilState(&depthDisabledStencilDesc, &default_depth_state);
  SetDepthState(null);

  SafeRelease(&default_blend_state);
  D3D11_BLEND_DESC BlendStateDescription;
  ZeroMemory(&BlendStateDescription,sizeof(BlendStateDescription));
  BlendStateDescription.AlphaToCoverageEnable = false;
  BlendStateDescription.RenderTarget[0].BlendEnable = true;

  BlendStateDescription.RenderTarget[0].SrcBlend                  = D3D11_BLEND_SRC_ALPHA;        //D3D11_BLEND_SRC_COLOR;
  BlendStateDescription.RenderTarget[0].DestBlend                 = D3D11_BLEND_INV_SRC_ALPHA;//D3D11_BLEND_DEST_COLOR;
  BlendStateDescription.RenderTarget[0].SrcBlendAlpha             = D3D11_BLEND_ONE;//D3D11_BLEND_SRC_ALPHA;
  BlendStateDescription.RenderTarget[0].DestBlendAlpha    = D3D11_BLEND_ONE;//D3D11_BLEND_DEST_ALPHA;
  BlendStateDescription.RenderTarget[0].BlendOp                   = D3D11_BLEND_OP_ADD;
  BlendStateDescription.RenderTarget[0].BlendOpAlpha              = D3D11_BLEND_OP_ADD;
  BlendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    
  device_->CreateBlendState(&BlendStateDescription,&default_blend_state);
  float blendFactor[] = {1,1, 1, 1};
  UINT sampleMask   = 0xffffffff;
  device_context_->OMSetBlendState(default_blend_state,blendFactor,sampleMask);
    

  return hr;*/
  return S_FALSE;
}

int ContextD3D11::Render() {
  

  //HRESULT result = swap_chain_->Present(0,0);

  /*
  HRESULT result = device_->Present(NULL,NULL,NULL,NULL);
  if (result == D3DERR_DEVICELOST) {
    //signal resources to be recreated
    device_->Reset(&d3dpp);
  }*/

// The application may optionally specify "dirty" or "scroll"
  // rects to improve efficiency in certain scenarios.
  DXGI_PRESENT_PARAMETERS parameters = {0};
  parameters.DirtyRectsCount = 0;
  parameters.pDirtyRects = nullptr;
  parameters.pScrollRect = nullptr;
  parameters.pScrollOffset = nullptr;
  
  // The first argument instructs DXGI to block until VSync, putting the application
  // to sleep until the next VSync. This ensures we don't waste any cycles rendering
  // frames that will never be displayed to the screen.
  HRESULT hr = swap_chain_->Present1(1, 0, &parameters);

  // Discard the contents of the render target.
  // This is a valid operation only when the existing contents will be entirely
  // overwritten. If dirty or scroll rects are used, this call should be removed.
  device_context_->DiscardView(render_target_view_);

  // Discard the contents of the depth stencil.
  device_context_->DiscardView(depth_stencil_view_);

  // If the device was removed either by a disconnect or a driver upgrade, we 
  // must recreate all device resources.
  if (hr == DXGI_ERROR_DEVICE_REMOVED)
  {
    HandleDeviceLost();
  }
  else
  {
    return S_FALSE;
  }

  return S_OK;
}

int ContextD3D11::HandleDeviceLost() {
  SafeRelease(&swap_chain_);

  CreateDeviceResources();
  UpdateForWindowSizeChange();
  return S_OK;
}

int ContextD3D11::ClearTarget() {
  float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
  device_context_->ClearRenderTargetView( render_target_view_, ClearColor );
  device_context_->ClearDepthStencilView( depth_stencil_view_, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0 );
  //return device_->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
  return S_OK;
}

int ContextD3D11::Begin() {
  //device_context_->Begin();
  return S_OK;//device_->BeginScene();
}

int ContextD3D11::End() {
  return S_OK;//device_->EndScene();
}

int ContextD3D11::CreateInputLayout(const InputElement inputs[], InputLayout& input_layout) {
  return S_FALSE;
}

int ContextD3D11::CreateInputLayout(const void* elements, size_t count, FileData vs_byte_code, InputLayout& input_layout) {
  ID3D11InputLayout* ptr;
  int hr = device_->CreateInputLayout((const D3D11_INPUT_ELEMENT_DESC*)elements,count,vs_byte_code.data,vs_byte_code.length,&ptr);
  input_layout.set_pointer(ptr);
  return hr;
}

int ContextD3D11::DestoryInputLayout(InputLayout& input_layout) {
  if( input_layout.pointer() ) 
    ((IUnknown*)input_layout.pointer())->Release();
  input_layout.set_pointer(NULL);
  return S_OK;
}

int ContextD3D11::SetInputLayout(InputLayout& input_layout) {
  device_context_->IASetInputLayout( (ID3D11InputLayout*)input_layout.pointer() );
  return S_OK;
}

  
int ContextD3D11::CreateBuffer(const void* buffer_desc, void* initial_data, void** buffer) {
 /* if (buffer.internal_pointer != NULL)
    return S_FALSE;
  D3D11_BUFFER_DESC bd;
  ZeroMemory( &bd, sizeof(bd) );
  bd.Usage = (D3D11_USAGE)buffer.description.usage;
  bd.ByteWidth = buffer.description.byte_width;
  bd.BindFlags = buffer.description.bind_flags;
  bd.CPUAccessFlags = buffer.description.cpu_access_flags;
  D3D11_SUBRESOURCE_DATA InitData;
  ZeroMemory( &InitData, sizeof(InitData) );
  InitData.pSysMem = initial_data;
  HRESULT result = device_->CreateBuffer( &bd, NULL, (ID3D11Buffer**)&buffer.internal_pointer );
  return result;*/

  HRESULT result = device_->CreateBuffer( (const D3D11_BUFFER_DESC*)buffer_desc, (const D3D11_SUBRESOURCE_DATA*)initial_data, (ID3D11Buffer**)buffer );
  return result;
}

int ContextD3D11::DestroyBuffer(void* buffer) {
  SafeRelease((ID3D11Buffer**)buffer);
  return S_OK;
 /* ID3D11Buffer* internal_buffer_ = (ID3D11Buffer*)buffer.internal_pointer;
  if (internal_buffer_ != NULL) {
    internal_buffer_->Release();
    internal_buffer_ = NULL;
    buffer.internal_pointer =  NULL;
    return S_OK;
  }
  return S_FALSE;*/
}

int ContextD3D11::UpdateSubresource(const void* buffer, void* data_pointer, void* box, uint32_t row_size, uint32_t depth_size) {
  device_context_->UpdateSubresource( (ID3D11Resource*)buffer, 0, (D3D11_BOX*)box, data_pointer, row_size, depth_size );
  return S_OK;
}

int ContextD3D11::SetConstantBuffers(ShaderType shader_type, uint32_t start_slot, uint32_t buffer_count, Buffer* buffer_array) {
  register void* buffers_[60];
  for (uint32_t i=0;i<buffer_count;++i)
    buffers_[i] = buffer_array[i].internal_pointer;

  switch (shader_type) {
    case kShaderTypeVertex:
      device_context_->VSSetConstantBuffers(start_slot,buffer_count,(ID3D11Buffer**)buffers_);
      return S_OK;
    case kShaderTypePixel:
      device_context_->PSSetConstantBuffers(start_slot,buffer_count,(ID3D11Buffer**)buffers_);
      return S_OK;
    case kShaderTypeGeometry:
      device_context_->GSSetConstantBuffers(start_slot,buffer_count,(ID3D11Buffer**)buffers_);
      return S_OK;
    case kShaderTypeHull:
      device_context_->HSSetConstantBuffers(start_slot,buffer_count,(ID3D11Buffer**)buffers_);
      return S_OK;
    case kShaderTypeDomain:
      device_context_->DSSetConstantBuffers(start_slot,buffer_count,(ID3D11Buffer**)buffers_);
      return S_OK;
    case kShaderTypeCompute:
      device_context_->CSSetConstantBuffers(start_slot,buffer_count,(ID3D11Buffer**)buffers_);
      return S_OK;
    default:
      return S_FALSE;
  }
}

int ContextD3D11::SetVertexBuffers(uint32_t start_slot, uint32_t buffer_count, const void** buffer_array, const uint32_t * strides,const uint32_t *offsets) {
  device_context_->IASetVertexBuffers(start_slot,buffer_count,(ID3D11Buffer**)buffer_array,strides,offsets);
  return S_OK;
}

int ContextD3D11::SetIndexBuffer(const Buffer& buffer, const uint32_t offset) {
  DXGI_FORMAT format = DXGI_FORMAT_R16_UINT;
  device_context_->IASetIndexBuffer((ID3D11Buffer*)buffer.internal_pointer,format,offset);
  return S_OK;
}

int ContextD3D11::ClearIndexBuffer() {
  device_context_->IASetIndexBuffer(NULL,DXGI_FORMAT_UNKNOWN,0);
  return S_OK;
}

int ContextD3D11::LockBuffer(void* buffer,uint32_t index,uint32_t type,BufferSubresource& subresource) {
  D3D11_MAPPED_SUBRESOURCE sub;
  device_context_->Map((ID3D11Resource*)buffer,index,(D3D11_MAP)type,0,&sub);
  subresource.data = sub.pData;
  subresource.row_pitch = sub.RowPitch;
  subresource.depth_pitch = sub.DepthPitch;
  return S_OK;
}

int ContextD3D11::UnlockBuffer(void* buffer,uint32_t index) {
  device_context_->Unmap((ID3D11Resource*)buffer,index);
  return S_OK;
}

int ContextD3D11::CompileShaderFromMemory(void* data, uint32_t len, LPCSTR szEntryPoint, LPCSTR szShaderModel, ShaderBlob& blob) {
    int hr = S_OK;
    /*DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
    #if defined( DEBUG ) || defined( _DEBUG )
      dwShaderFlags |= D3DCOMPILE_DEBUG;
    #endif

    ID3DBlob* pErrorBlob,*buffer;
    hr = -1;/*D3DX11CompileFromMemory( (LPCSTR)data,len,NULL, NULL, NULL, szEntryPoint, szShaderModel, 
      dwShaderFlags, 0, NULL, &buffer, &pErrorBlob, NULL );*/
    /*if( FAILED(hr) ) {
      #ifdef _DEBUG
      if( pErrorBlob != NULL )
          OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
          SafeRelease(&pErrorBlob);
      #endif
      return hr;
    }
    SafeRelease(&pErrorBlob);

    blob.internal_ = buffer;
    blob.set_data(buffer->GetBufferPointer());
    blob.set_size(buffer->GetBufferSize());
*/
    return S_OK;
}

int ContextD3D11::CreateVertexShader(void* data, size_t length, VertexShader& vs) {
  void* ptr;
  int hr = device_->CreateVertexShader(data, length, NULL, (ID3D11VertexShader**)&ptr);
  vs.set_internal_pointer(ptr);
  return hr;
}

int ContextD3D11::CreatePixelShader(void* data, size_t length, PixelShader& ps) {
  void* ptr;
  int hr = device_->CreatePixelShader(data, length, NULL, (ID3D11PixelShader**)&ptr);
  ps.set_internal_pointer(ptr);
  return hr;
}

int ContextD3D11::CreateGeometryShader(void* data, size_t length, GeometryShader& gs) {
  void* ptr;
  int hr = device_->CreateGeometryShader(data, length, NULL, (ID3D11GeometryShader**)&ptr);
  gs.set_internal_pointer(ptr);
  return hr;
}

int ContextD3D11::DestroyShader(Shader& shader) {
  if( shader.internal_pointer() ) {
    ((IUnknown*)shader.internal_pointer())->Release();
    shader.set_internal_pointer(nullptr);
    return S_OK;
  } else {
    return S_FALSE;
  }
}

int ContextD3D11::SetShader(const Shader& shader) {

  switch (shader.type) {
    case kShaderTypeVertex:
      if (shaders_.vs == nullptr || shaders_.vs->internal_pointer() != shader.internal_pointer()) {
        shaders_.vs = (VertexShader*)&shader;
        device_context_->VSSetShader((ID3D11VertexShader*)(shaders_.vs->internal_pointer()), NULL, 0);
        //settings.vertex_shader = shader.internal_pointer();
      }
      return S_OK;
    case kShaderTypePixel:
      if (shaders_.ps == nullptr || shaders_.ps->internal_pointer() != shader.internal_pointer()) {
        shaders_.ps = (PixelShader*)&shader;
        device_context_->PSSetShader((ID3D11PixelShader*)(shaders_.ps->internal_pointer()), NULL, 0);
      }
      return S_OK;
    case kShaderTypeGeometry:
      device_context_->GSSetShader((ID3D11GeometryShader*)(shader.internal_pointer()), NULL, 0);
      return S_OK;
    case kShaderTypeHull:
      device_context_->HSSetShader((ID3D11HullShader*)(shader.internal_pointer()), NULL, 0);
      return S_OK;
    case kShaderTypeDomain:
      device_context_->DSSetShader((ID3D11DomainShader*)(shader.internal_pointer()), NULL, 0);
      return S_OK;
    case kShaderTypeCompute:
      device_context_->CSSetShader((ID3D11ComputeShader*)(shader.internal_pointer()), NULL, 0);
      return S_OK;
    default:
      return S_FALSE;
  }
}


int ContextD3D11::ClearShader(ShaderType shader_type) {
  switch (shader_type) {
    case kShaderTypeVertex:
      device_context_->VSSetShader(NULL,NULL,0);
      return S_OK;
    case kShaderTypePixel:
      device_context_->PSSetShader(NULL,NULL,0);
      return S_OK;
    case kShaderTypeGeometry:
      device_context_->GSSetShader(NULL,NULL,0);
      return S_OK;
    case kShaderTypeHull:
      device_context_->HSSetShader(NULL,NULL,0);
      return S_OK;
    case kShaderTypeDomain:
      device_context_->DSSetShader(NULL,NULL,0);
      return S_OK;
    case kShaderTypeCompute:
      device_context_->CSSetShader(NULL,NULL,0);
      return S_OK;
    default:
      return S_FALSE;
  }
}

int ContextD3D11::Draw(uint32_t vertex_count, uint32_t vertex_start_index) {
  device_context_->Draw(vertex_count,vertex_start_index);
  return S_OK;
}

int ContextD3D11::DrawIndexed(uint32_t index_count, uint32_t vertex_start_index, int32_t base) {
  device_context_->DrawIndexed(index_count,vertex_start_index,base);
  return S_OK;
}

int ContextD3D11::SetShaderResources(ShaderType shader_type,uint32_t start_slot,uint32_t count,void** resources_pointer) {
  switch (shader_type) {
    case kShaderTypeVertex:
      device_context_->VSSetShaderResources(start_slot,count,(ID3D11ShaderResourceView*const*)resources_pointer);
      return S_OK;
    case kShaderTypePixel:
      device_context_->PSSetShaderResources(start_slot,count,(ID3D11ShaderResourceView*const*)resources_pointer);
      return S_OK;
    case kShaderTypeGeometry:
      device_context_->GSSetShaderResources(start_slot,count,(ID3D11ShaderResourceView*const*)resources_pointer);
      return S_OK;
    case kShaderTypeHull:
      device_context_->HSSetShaderResources(start_slot,count,(ID3D11ShaderResourceView*const*)resources_pointer);
      return S_OK;
    case kShaderTypeDomain:
      device_context_->DSSetShaderResources(start_slot,count,(ID3D11ShaderResourceView*const*)resources_pointer);
      return S_OK;
    case kShaderTypeCompute:
      device_context_->CSSetShaderResources(start_slot,count,(ID3D11ShaderResourceView*const*)resources_pointer);
      return S_OK;
    default:
      return S_FALSE;
  }
}

int ContextD3D11::SetPrimitiveTopology(uint32_t topology) {
  if (settings.topology != topology) {
    device_context_->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)topology);
    settings.topology = topology;
  }
  return S_OK;
}

int ContextD3D11::SetDepthState(void* ptr) {

  if (ptr == nullptr)  {
    states_.ds = default_depth_state;
  } else {
    states_.ds = ptr;
  }
  device_context_->OMSetDepthStencilState((ID3D11DepthStencilState*)states_.ds, 0);
  return S_OK;
}

int ContextD3D11::CreateTexture(uint32_t width, uint32_t height, uint32_t format, uint32_t type, Texture& texture) {
  //todo whole thing
  
  CD3D11_TEXTURE2D_DESC desc((DXGI_FORMAT)format,width,height);
  ID3D11Texture2D* ptr;
  int hr = device_->CreateTexture2D(&desc,nullptr,&ptr);
  texture.data_pointer = ptr;
  texture.format = format;
  
  return hr;
}

int ContextD3D11::CreateTextureFromMemory(void* data_pointer, size_t data_length, Texture& texture) {
  texture.data_length = data_length;
  
  int result = -1;//D3DX11CreateTextureFromMemory(device_,data_pointer,data_length,NULL,NULL,(ID3D11Resource**)&texture.data_pointer,NULL);
  return result;
}

int ContextD3D11::DestroyTexture(Texture& texture) {
  SafeRelease((ID3D11Resource**)&texture.data_pointer);
  return S_OK;
}

int ContextD3D11::CreateResourceView(Texture& texture,ResourceView& resource_view) {
  return device_->CreateShaderResourceView((ID3D11Resource*)texture.data_pointer,NULL,(ID3D11ShaderResourceView**)&resource_view.data_pointer);
}

int ContextD3D11::DestroyResourceView(ResourceView& resource_view) {
  SafeRelease((ID3D11ShaderResourceView**)&resource_view.data_pointer);
  return S_OK;
}

int ContextD3D11::SetCamera(Camera* camera) {
  return S_FALSE;
}

int ContextD3D11::SetViewport(float x,float y,float w,float h,float min_depth,float max_depth) {
  D3D11_VIEWPORT vp;
  vp.MaxDepth = min_depth;
  vp.MinDepth = max_depth;
  vp.Height = h;
  vp.Width = w;
  vp.TopLeftX = x;
  vp.TopLeftY = y;
  device_context_->RSSetViewports(0,&vp);
  return S_OK;
}

int ContextD3D11::SetDefaultTargets() {
  device_context_->OMSetRenderTargets( 1, &render_target_view_, depth_stencil_view_ );
  return S_OK;
}

int ContextD3D11::PushDepthState(void* ptr) {
  states_.ds_list.push_back(states_.ds);
  states_.ds = ptr;
  device_context_->OMSetDepthStencilState((ID3D11DepthStencilState*)states_.ds,0);
  return S_OK;
}

int ContextD3D11::PopDepthState() {
  states_.ds = states_.ds_list.back();
  states_.ds_list.pop_back();
  device_context_->OMSetDepthStencilState((ID3D11DepthStencilState*)states_.ds,0);
  return S_OK;
}

int ContextD3D11::PushRasterizerState(void* ptr) {
  states_.rs_list.push_back(states_.rs);
  states_.rs = ptr;
  device_context_->RSSetState((ID3D11RasterizerState*)states_.rs);
  return S_OK;
}

int ContextD3D11::PopRasterizerState() {
  states_.rs = states_.rs_list.back();
  states_.rs_list.pop_back();
  device_context_->RSSetState((ID3D11RasterizerState*)states_.rs);
  return S_OK;
}

int ContextD3D11::PushVertexShader(VertexShader* ptr) {
  shaders_.vs_list.push_back(shaders_.vs);
  SetShader(*ptr);
  return S_OK;
}

int ContextD3D11::PopVertexShader() {
  auto ptr = shaders_.vs_list.back();
  shaders_.vs_list.pop_back();
if (ptr)  SetShader(*ptr);
  return S_OK;
}

int ContextD3D11::PushPixelShader(PixelShader* ptr) {
  shaders_.ps_list.push_back(shaders_.ps);
  SetShader(*ptr);
  return S_OK;
}

int ContextD3D11::PopPixelShader() {
  auto ptr = shaders_.ps_list.back();
  shaders_.ps_list.pop_back();
  SetShader(*ptr);
  return S_OK;
}



}