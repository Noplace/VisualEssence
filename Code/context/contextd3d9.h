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
#ifndef GRAPHICS_CONTEXTD3D9_H
#define GRAPHICS_CONTEXTD3D9_H


namespace graphics {

class Component;
class Camera2D;
template<typename T>
class VertexBuffer;

class ContextD3D9 : public Context {
 public:
  static const D3DVERTEXELEMENT9 ve_xyzc[3];
  static const D3DVERTEXELEMENT9 ve_xyzcuv[4];

  ContextD3D9();
  ~ContextD3D9();
  int Initialize();
  int Deinitialize();
  int CreateDisplay(core::windows::Window* window);
  int Render();
  int ClearTarget();
  int Begin();
  int End();
  int CreateInputLayout(const InputElement inputs[], InputLayout& input_layout);
  int DestoryInputLayout(InputLayout&);
  int SetInputLayout(InputLayout&);
  int CreateBuffer(Buffer& buffer, void* initial_data);
  int DestroyBuffer(Buffer& );
  int UpdateSubresource(const Buffer&, void*, void*, uint32_t , uint32_t);
  int CopyToVertexBuffer(const Buffer& buffer, void* data_pointer, uint32_t type_size, uint32_t offset , uint32_t count);
  int SetConstantBuffers(ShaderType, uint32_t, uint32_t, Buffer*);
  int SetVertexBuffers(uint32_t , uint32_t , Buffer* , const uint32_t * , const uint32_t *);
  int SetIndexBuffer(const Buffer& , const uint32_t );
  int ClearIndexBuffer();
  int LockBuffer(void*, uint32_t,uint32_t, BufferSubresource&); 
  int UnlockBuffer(void*, uint32_t);
  int CompileShaderFromMemory(void*, uint32_t, LPCSTR, LPCSTR, ShaderBlob&);
  int CreateVertexShader(void*, uint32_t, VertexShader&);
  int CreatePixelShader(void*, uint32_t, PixelShader&);
  int CreateGeometryShader(void*, uint32_t, GeometryShader&);
  int DestroyShader(Shader&);
  int SetShader(const Shader&);
  int ClearShader(ShaderType);
  int Draw(uint32_t, uint32_t);
  int SetShaderResources(ShaderType, uint32_t, uint32_t, void**);
  int SetPrimitiveTopology(uint32_t);
  int CreateTextureFromMemory(void*, uint32_t, Texture&);
  int DestroyTexture(Texture&);
  int CreateResourceView(Texture&,ResourceView&);
  int DestroyResourceView(ResourceView&);
  int SetCamera(Camera* camera);
  /*int CreateBuffer(BufferDescription& buffer_description,void** buffer_pointer);
  int SetVertexBuffers(uint32_t start_slot, uint32_t buffer_count, void *const * buffers, const uint32_t * strides,const uint32_t *offsets);*/
  LPDIRECT3DDEVICE9 device() { return device_; }
private:
  D3DDISPLAYMODE* display_modes_;
  LPDIRECT3D9 direct3d_;
  LPDIRECT3DDEVICE9 device_;
  D3DPRESENT_PARAMETERS d3dpp;
};

}


#endif