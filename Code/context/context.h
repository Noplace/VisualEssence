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

class Component;
class Camera2D;
template<typename T>
class VertexBuffer;

inline float Unit(float x) {
  return x*0.001f;
}



class Context {
 public:
  Context() : window_handle_(NULL) { }
  virtual ~Context() { }
  virtual int Initialize() = 0;
  virtual int Deinitialize() = 0;
  virtual int CreateDisplay(HWND window) = 0;
  virtual int OnWindowSizeChange(uint32_t width, uint32_t height) = 0;
  virtual int Render() = 0;
  //virtual int Clear(int target, int zbuffer) = 0;
  virtual int ClearTarget() = 0;
  virtual int Begin() = 0;
  virtual int End() = 0;
  virtual int CreateInputLayout(const InputElement inputs[], InputLayout& input_layout) = 0;
  virtual int CreateInputLayout(const void* elements, size_t count, FileData vs_byte_code, InputLayout& input_layout) = 0;
  virtual int DestoryInputLayout(InputLayout&) = 0;
  virtual int SetInputLayout(InputLayout&) = 0;
  //virtual int CreateBuffer(Buffer& buffer, void* initial_data) = 0;
  virtual int CreateBuffer(const void* buffer_desc, void* initial_data, void** buffer) = 0;
  virtual int DestroyBuffer(void** buffer) = 0;
  virtual int UpdateSubresource(const void*, void*, void*, uint32_t , uint32_t) = 0;
  virtual int CopyToVertexBuffer(const void* buffer, void* data_pointer, uint32_t type_size, uint32_t offset , uint32_t count) = 0;
  virtual int SetConstantBuffers(ShaderType, uint32_t, uint32_t, const void**) = 0;
  virtual int SetVertexBuffers(uint32_t , uint32_t , const void** , const uint32_t * , const uint32_t *) = 0;
  virtual int SetIndexBuffer(const Buffer& , const uint32_t ) = 0;
  virtual int SetIndexBuffer(const void* buffer, int format, const uint32_t offset) = 0;
  virtual int ClearIndexBuffer() = 0;
  virtual int LockBuffer(void*, uint32_t,uint32_t, BufferSubresource&) = 0;
  virtual int UnlockBuffer(void*, uint32_t) = 0;
  virtual int CopyBufferFast(void* buffer, void* data, size_t length, size_t offset) = 0;
  virtual int CompileShaderFromMemory(void*, uint32_t, LPCSTR, LPCSTR, ShaderBlob&) = 0;
  virtual int CreateVertexShader(void*, size_t, VertexShader&) = 0;
  virtual int CreatePixelShader(void*, size_t, PixelShader&) = 0;
  virtual int CreateGeometryShader(void*, size_t, GeometryShader&) = 0;
  virtual int DestroyShader(Shader&) = 0;
  virtual int SetShader(const Shader&) = 0;
  virtual int ClearShader(ShaderType) = 0;
  virtual int Draw(uint32_t vertex_count, uint32_t start_vertex_index) = 0;
  virtual int DrawIndexed(uint32_t index_count, uint32_t vertex_start_index, int32_t base) = 0;
  virtual int SetShaderResources(ShaderType, uint32_t, uint32_t, void**) = 0;
  virtual int SetPrimitiveTopology(uint32_t) = 0;
  virtual int SetDepthState(void* ptr, UINT stencil) = 0;
  //virtual int GetRenderTarget(ResourceView& resource_view) = 0;
  //virtual int SetRenderTarget(ResourceView& resource_view) = 0;
  virtual int CreateTexture(uint32_t width, uint32_t height, uint32_t format, uint32_t type, Texture& texture) = 0;
  virtual int CreateTextureFromMemory(void* data_pointer, size_t data_length, Texture& texture) = 0;
  virtual int DestroyTexture(Texture&) = 0;
  //virtual int CopyToTexture(Texture& texture, void* data_pointer, uint32_t data_format, uint32_t data_pitch, const TexturePoint src_pos, const TexturePoint dest_pos, uint32_t w, uint32_t h) = 0;
  virtual int CreateResourceView(Texture& texture, ResourceView& resource_view) = 0;
  virtual int DestroyResourceView(ResourceView& resource_view) = 0;
  virtual int SetCamera(Camera* camera) = 0;
  virtual int SetViewport(float x,float y,float w,float h,float min_depth,float max_depth) = 0;

  virtual int PushDepthState(void* ptr) = 0;
  virtual int PopDepthState() = 0;
  virtual int PushRasterizerState(void* ptr) = 0;
  virtual int PopRasterizerState() = 0;

  virtual int PushVertexShader(VertexShader* ptr) = 0;
  virtual int PopVertexShader() = 0;
  virtual int PushPixelShader(PixelShader* ptr) = 0;
  virtual int PopPixelShader() = 0;
  

  //virtual int CreateEffectInterface(uint8_t* data_pointer, size_t data_length, void** interface_) = 0;
  //virtual int DestroyEffectInterface(void** interface_) = 0;
  HWND window_handle() { return window_handle_; }
  uint32_t width() { return width_; }
  uint32_t height() { return height_; }
  ShaderManager& shader_manager() { return shader_manager_; }
  ActionManager& action_manager() { return action_manager_; }
  ResourceManager& resource_manager() { return resource_manager_; }
 protected:
  Camera* camera_;
  HWND window_handle_;
  uint32_t width_;
  uint32_t height_;
  ShaderManager shader_manager_;
  ActionManager action_manager_;
  ResourceManager resource_manager_;
  struct  DSState{
    void* ptr;
    UINT stencil;
  } ;
  struct {
    std::vector<void*> rs_list;
    std::vector<DSState> ds_list;
    DSState ds;
    void* rs;
  } states_;
  struct {
    std::vector<VertexShader*> vs_list;
    std::vector<PixelShader*> ps_list;
    VertexShader* vs;
    PixelShader* ps;
  }shaders_;
};

}



