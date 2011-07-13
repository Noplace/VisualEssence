#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

#include <WinCore/windows/windows.h>
#include <xnamath.h>
#include "component.h"
#include "buffer.h"
#include "shader.h"

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
  virtual int CreateBuffer(Buffer& , void* ) = 0;
  virtual int DestroyBuffer(Buffer& ) = 0;
  virtual int UpdateBuffer(const Buffer&, void*, void*, uint32_t , uint32_t) = 0;
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
  virtual int DestroyVertexShader(VertexShader&) = 0;
  virtual int DestroyPixelShader(PixelShader&) = 0;
  virtual int DestroyGeometryShader(GeometryShader&) = 0;
  virtual int SetShader(const Shader&) = 0;
  core::windows::Window* window() { return window_; }
  uint32_t width() { return width_; }
  uint32_t height() { return height_; }

 protected:
  core::windows::Window* window_;
  uint32_t width_;
  uint32_t height_;
};

}



#endif