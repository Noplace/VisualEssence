#ifndef GRAPHICS_CONTEXTD3D9_H
#define GRAPHICS_CONTEXTD3D9_H

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include "context.h"

namespace graphics {

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
  int CreateDisplay(core::windows::Window* window);
  int Resize(uint32_t width, uint32_t height);
  int Render();
  int ClearTarget();
  int Begin();
  int End();
  int CreateBuffer(Buffer& buffer, void* initial_data);
  int DestroyBuffer(Buffer& buffer);
  int UpdateBuffer(const Buffer& buffer, void* data_pointer, void* box, uint32_t row_size, uint32_t depth_size);
  int CopyToVertexBuffer(const Buffer& buffer, void* data_pointer, uint32_t type_size, uint32_t offset , uint32_t count) {
    D3D11_BOX box;
    ZeroMemory(&box,sizeof(box));
    box.left = type_size*offset;
    box.right = type_size*(offset+count);
    box.bottom = box.back = 1;
    return UpdateBuffer(buffer,data_pointer,&box,type_size,0);
  }
  int SetConstantBuffers(ShaderType shader_type, uint32_t start_slot, uint32_t buffer_count, Buffer* buffer_array);
  int SetVertexBuffers(uint32_t start_slot, uint32_t buffer_count, Buffer* buffer_array, const uint32_t * strides,const uint32_t *offsets);
  int SetIndexBuffer(const Buffer& buffer, const uint32_t offset);
  int ClearIndexBuffer();
  int LockBuffer(void* buffer, uint32_t index, uint32_t type, BufferSubresource& subresource); 
  int UnlockBuffer(void* buffer, uint32_t index);
  int CompileShaderFromMemory(void* data, uint32_t len, LPCSTR szEntryPoint, LPCSTR szShaderModel, ShaderBlob& blob);
  int CreateVertexShader(void* data, uint32_t length, VertexShader& vs);
  int CreatePixelShader(void* data, uint32_t length, PixelShader& ps);
  int CreateGeometryShader(void* data, uint32_t length, GeometryShader& gs);
  int DestroyGeometryShader(GeometryShader& gs);
  int DestroyVertexShader(VertexShader& vs);
  int DestroyPixelShader(PixelShader& ps);
  int SetShader(const Shader& shader);
  int Draw(uint32_t vertex_count, uint32_t vertex_start_index);
  int SetPixelShaderResources(uint32_t start_slot,uint32_t count,void** resources_pointer);
  int SetPrimitiveTopology(uint32_t topology);
  ID3D11Device* device() { return device_; }
  ID3D11DeviceContext* device_context() { return device_context_; }
private:
  ID3D11DeviceContext*    device_context_;
  ID3D11Device*           device_;
  IDXGISwapChain*         swap_chain_;
  ID3D11RenderTargetView* render_target_view_;
  ID3D11Texture2D*        depth_stencil_;
  ID3D11DepthStencilView* depth_stencil_view_;
  /*ID2D1RenderTarget *d2d_render_target_;
  ID2D1Factory *d2d_factory_;
  IWICImagingFactory *wic_factory_;
  IDWriteFactory *dwrite_factory_;*/

};

}


#endif