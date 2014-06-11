/*****************************************************************************************************************
* Copyright (c) 2014 Khalid Ali Al-Kooheji                                                                       *
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

/*#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>*/


namespace ve {

class Component;
class Camera2D;
template<typename T>
class VertexBuffer;

class ContextD3D11 : public Context {
 public:
  //static const D3DVERTEXELEMENT9 ve_xyzc[3];
  //static const D3DVERTEXELEMENT9 ve_xyzcuv[4];

  ContextD3D11();
  ~ContextD3D11();
  int Initialize();
  int Deinitialize();
  int CreateDisplay(HWND window);
  int CreateDeviceResources();
  int CreateWindowSizeDependentResources(uint32_t width, uint32_t height);
  int OnWindowSizeChange(uint32_t width, uint32_t height);
  
  int Resize(uint32_t width, uint32_t height, bool fullscreen);
  int Render();
  int HandleDeviceLost();
  int ClearTarget();
  int Begin();
  int End();
  int CreateInputLayout(const InputElement inputs[], InputLayout& input_layout);
  int CreateInputLayout(const void* elements, size_t count, FileData vs_byte_code, InputLayout& input_layout);
  int DestoryInputLayout(InputLayout&);
  int SetInputLayout(InputLayout&);
  int CreateBuffer(const void* buffer_desc, void* initial_data, void** buffer);
  int DestroyBuffer(void** buffer);
  int UpdateSubresource(const void* buffer, void* data_pointer, void* box, uint32_t row_size, uint32_t depth_size);
  int CopyToVertexBuffer(const void* buffer, void* data_pointer, uint32_t type_size, uint32_t offset , uint32_t count) {
    D3D11_BOX box;
    ZeroMemory(&box,sizeof(box));
    box.left = type_size*offset;
    box.right = type_size*(offset+count);
    box.bottom = box.back = 1;
    return UpdateSubresource(buffer,data_pointer,&box,type_size,0);
  }
  int SetConstantBuffers(ShaderType shader_type, uint32_t start_slot, uint32_t buffer_count, const void** buffer_array);
  int SetVertexBuffers(uint32_t start_slot, uint32_t buffer_count, const void** buffer_array, const uint32_t * strides,const uint32_t *offsets);
  int SetIndexBuffer(const Buffer& buffer, const uint32_t offset);
  int SetIndexBuffer(const void* buffer, int format, const uint32_t offset);
  int ClearIndexBuffer();
  int LockBuffer(void* buffer, uint32_t index, uint32_t type, BufferSubresource& subresource); 
  int UnlockBuffer(void* buffer, uint32_t index);
  int CopyBufferFast(void* buffer, void* data, size_t length, size_t offset = 0);
  int CompileShaderFromMemory(void* data, uint32_t len, LPCSTR szEntryPoint, LPCSTR szShaderModel, ShaderBlob& blob);
  int CreateVertexShader(void* data, size_t length, VertexShader& vs);
  int CreatePixelShader(void* data, size_t length, PixelShader& ps);
  int CreateGeometryShader(void* data, size_t length, GeometryShader& gs);
  int DestroyShader(Shader& shader);
  int SetShader(const Shader& shader);
  int ClearShader(ShaderType shader_type);
  int Draw(uint32_t vertex_count, uint32_t vertex_start_index);
  int DrawIndexed(uint32_t index_count, uint32_t vertex_start_index, int32_t base);
  int SetShaderResources(ShaderType shader_type, uint32_t start_slot, uint32_t count, void** resources_pointer);
  int SetPrimitiveTopology(uint32_t topology);
  int SetDepthState(void* ptr, UINT stencil);
  int CreateTexture(uint32_t width, uint32_t height, uint32_t format, uint32_t type, Texture& texture);
  int CreateTextureFromMemory(void* data_pointer, size_t data_length, Texture& texture);
  int DestroyTexture(Texture& texture);
  int CreateResourceView(Texture& texture,ResourceView& resource_view);
  int DestroyResourceView(ResourceView& resource_view);
  int SetCamera(Camera* camera);
  int SetViewport(float x,float y,float w,float h,float min_depth,float max_depth);
  int SetDefaultTargets();
  int SetRenderTargets(void* target);

  int PushDepthState(void* ptr);
  int PopDepthState();
  int PushRasterizerState(void* ptr);
  int PopRasterizerState();

  int PushVertexShader(VertexShader* ptr);
  int PopVertexShader();
  int PushPixelShader(PixelShader* ptr);
  int PopPixelShader();

  int CreateRenderTarget(int width, int height,void** render_target, Texture* texture, ResourceView* resource_view);

  ID3D11Device1* device() { return device_; }
  ID3D11DeviceContext1* device_context() { return device_context_; }
private:
  std::vector <IDXGIAdapter*> adaptors_;
  IDXGIAdapter* adaptor_;
  ID3D11DeviceContext1*    device_context_;
  ID3D11Device1*           device_;
  IDXGISwapChain1*         swap_chain_;
  ID3D11RenderTargetView* render_target_view_;
  ID3D11Texture2D*        depth_stencil_;
  ID3D11DepthStencilView* depth_stencil_view_;
  ID3D11DepthStencilState* default_depth_state;
  ID3D11BlendState* default_blend_state;
  /*ID2D1RenderTarget *d2d_render_target_;
  ID2D1Factory *d2d_factory_;
  IWICImagingFactory *wic_factory_;
  IDWriteFactory *dwrite_factory_;*/
  struct {
    uint32_t topology;
  } settings;
};

}


