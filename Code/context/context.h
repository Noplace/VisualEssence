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
#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

namespace graphics {

class Component;
class Camera2D;
template<typename T>
class VertexBuffer;

inline float Unit(float x) {
  return x*0.001f;
}



class Context {
 public:
  Context() : window_(NULL) { }
  virtual ~Context() { }
  virtual int Initialize() = 0;
  virtual int Deinitialize() = 0;
  virtual int CreateDisplay(core::windows::Window*) = 0;
  virtual int Render() = 0;
  virtual int ClearTarget() = 0;
  virtual int Begin() = 0;
  virtual int End() = 0;
  virtual int CreateInputLayout(const InputElement[],InputLayout&) = 0;
  virtual int DestoryInputLayout(InputLayout&) = 0;
  virtual int SetInputLayout(InputLayout&) = 0;
  virtual int CreateBuffer(Buffer& , void* ) = 0;
  virtual int DestroyBuffer(Buffer& ) = 0;
  virtual int UpdateSubresource(const Buffer&, void*, void*, uint32_t , uint32_t) = 0;
  virtual int CopyToVertexBuffer(const Buffer& buffer, void* data_pointer, uint32_t type_size, uint32_t offset , uint32_t count) { return S_FALSE; };
  virtual int SetConstantBuffers(ShaderType, uint32_t, uint32_t, Buffer*) = 0;
  virtual int SetVertexBuffers(uint32_t , uint32_t , Buffer* , const uint32_t * , const uint32_t *) = 0;
  virtual int SetIndexBuffer(const Buffer& , const uint32_t ) = 0;
  virtual int ClearIndexBuffer() = 0;
  virtual int LockBuffer(void*, uint32_t,uint32_t, BufferSubresource&) = 0; 
  virtual int UnlockBuffer(void*, uint32_t) = 0;
  virtual int CompileShaderFromMemory(void*, uint32_t, LPCSTR, LPCSTR, ShaderBlob&) = 0;
  virtual int CreateVertexShader(void*, uint32_t, VertexShader&) = 0;
  virtual int CreatePixelShader(void*, uint32_t, PixelShader&) = 0;
  virtual int CreateGeometryShader(void*, uint32_t, GeometryShader&) = 0;
  virtual int DestroyShader(Shader&) = 0;
  virtual int SetShader(const Shader&) = 0;
  virtual int ClearShader(ShaderType) = 0;
  virtual int Draw(uint32_t, uint32_t) = 0;
  virtual int SetShaderResources(ShaderType, uint32_t, uint32_t, void**) = 0;
  virtual int SetPrimitiveTopology(uint32_t) = 0;
  virtual int CreateTexture(uint32_t, uint32_t, uint32_t, uint32_t, Texture&) = 0;
  virtual int CreateTextureFromMemory(void*, uint32_t, Texture&) = 0;
  virtual int DestroyTexture(Texture&) = 0;
  //virtual int LockTexture(Texture&) = 0;
  //virtual int UnlockTexture(Texture&) = 0;
  virtual int CreateResourceView(Texture&,ResourceView&) = 0;
  virtual int DestroyResourceView(ResourceView&) = 0;
  virtual int SetCamera(Camera*) = 0;
  virtual int SetViewport(float, float, float, float, float, float ) = 0;
  core::windows::Window* window() { return window_; }
  uint32_t width() { return width_; }
  uint32_t height() { return height_; }

 protected:
  Camera* camera_;
  core::windows::Window* window_;
  uint32_t width_;
  uint32_t height_;
};

}



#endif