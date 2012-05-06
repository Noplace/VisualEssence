#include "../ve.h"

#pragma comment(lib, "d3d11.lib")
#ifdef _DEBUG 
#pragma comment(lib, "d3dx11d.lib")
#else
#pragma comment(lib, "d3dx11.lib")
#endif

//#pragma comment(lib, "dxgi.lib")

namespace graphics {
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
 
  return hr;
}

int ContextD3D11::Deinitialize() {
    if( device_context_ )  device_context_->ClearState();

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

int ContextD3D11::CreateDisplay(core::windows::Window* window) {
  window_ = window;
  int hr = S_OK;

  RECT rc;
  GetClientRect( window_->handle(), &rc );
  width_ = rc.right - rc.left;
  height_ = rc.bottom - rc.top;

  UINT createDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;//D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
 // createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
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
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
  };
  UINT numFeatureLevels = ARRAYSIZE( featureLevels );

  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory( &sd, sizeof( sd ) );
  sd.BufferCount = 2;
  sd.BufferDesc.Width = width_;
  sd.BufferDesc.Height = height_;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = window_->handle();
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  D3D_FEATURE_LEVEL feature_level;
  hr = D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, NULL, 0,
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
  return S_OK;

}

int ContextD3D11::Resize(uint32_t width, uint32_t height) {
  int hr = S_OK;

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
    

  return hr;
}

int ContextD3D11::Render() {
  

  HRESULT result = swap_chain_->Present(0,0);

  /*
  HRESULT result = device_->Present(NULL,NULL,NULL,NULL);
  if (result == D3DERR_DEVICELOST) {
    //signal resources to be recreated
    device_->Reset(&d3dpp);
  }*/

  return S_OK;
}

int ContextD3D11::ClearTarget() {
  float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
  device_context_->ClearRenderTargetView( render_target_view_, ClearColor );
  device_context_->ClearDepthStencilView( depth_stencil_view_, D3D11_CLEAR_DEPTH, 1.0f, 0 );
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

int ContextD3D11::CreateBuffer(Buffer& buffer, void* initial_data) {
  if (buffer.internal_pointer != NULL)
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
  return result;
}

int ContextD3D11::DestroyBuffer(Buffer& buffer) {
  ID3D11Buffer* internal_buffer_ = (ID3D11Buffer*)buffer.internal_pointer;
  if (internal_buffer_ != NULL) {
    internal_buffer_->Release();
    internal_buffer_ = NULL;
    buffer.internal_pointer =  NULL;
    return S_OK;
  }
  return S_FALSE;
}

int ContextD3D11::UpdateSubresource(const Buffer& buffer, void* data_pointer, void* box, uint32_t row_size, uint32_t depth_size) {
  device_context_->UpdateSubresource( (ID3D11Resource*)buffer.internal_pointer, 0, (D3D11_BOX*)box, data_pointer, row_size, depth_size );
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

int ContextD3D11::SetVertexBuffers(uint32_t start_slot, uint32_t buffer_count, Buffer* buffer_array, const uint32_t * strides,const uint32_t *offsets) {
  register void* buffers_[60];
  for (uint32_t i=0;i<buffer_count;++i)
    buffers_[i] = buffer_array[i].internal_pointer;
  device_context_->IASetVertexBuffers(start_slot,buffer_count,(ID3D11Buffer**)buffers_,strides,offsets);
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

int ContextD3D11::CompileShaderFromMemory(void* data, uint32_t len, LPCSTR szEntryPoint, LPCSTR szShaderModel, ShaderBlob& blob)
{
    int hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromMemory( (LPCSTR)data,len,NULL, NULL, NULL, szEntryPoint, szShaderModel, 
      dwShaderFlags, 0, NULL, &blob.internal_, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}

int ContextD3D11::CreateVertexShader(void* data, uint32_t length, VertexShader& vs) {
  void* ptr;
  int hr = device_->CreateVertexShader(data, length, NULL, (ID3D11VertexShader**)&ptr);
  vs.set_internal_pointer(ptr);
  return hr;
}

int ContextD3D11::CreatePixelShader(void* data, uint32_t length, PixelShader& ps) {
  void* ptr;
  int hr = device_->CreatePixelShader(data, length, NULL, (ID3D11PixelShader**)&ptr);
  ps.set_internal_pointer(ptr);
  return hr;
}

int ContextD3D11::CreateGeometryShader(void* data, uint32_t length, GeometryShader& gs) {
  void* ptr;
  int hr = device_->CreateGeometryShader(data, length, NULL, (ID3D11GeometryShader**)&ptr);
  gs.set_internal_pointer(ptr);
  return hr;
}

int ContextD3D11::DestroyShader(Shader& shader) {
  if( shader.internal_pointer() ) 
    ((IUnknown*)shader.internal_pointer())->Release();
  return S_OK;
}

int ContextD3D11::SetShader(const Shader& shader) {

  switch (shader.type) {
    case kShaderTypeVertex:
      //if (settings.vertex_shader != &shader) {
        device_context_->VSSetShader((ID3D11VertexShader*)(shader.internal_pointer()), NULL, 0);
        //settings.vertex_shader = shader.internal_pointer();
      //}
      return S_OK;
    case kShaderTypePixel:
      device_context_->PSSetShader((ID3D11PixelShader*)(shader.internal_pointer()), NULL, 0);
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
  device_context_->OMSetDepthStencilState(default_depth_state, 0);
  return S_OK;
}

int ContextD3D11::CreateTextureFromMemory(void* data_pointer, uint32_t data_length, Texture& texture) {
  texture.data_length = data_length;
  
  int result = D3DX11CreateTextureFromMemory(device_,data_pointer,data_length,NULL,NULL,(ID3D11Resource**)&texture.data_pointer,NULL);
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

}