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
  virtual int CreateTextureFromMemory(void*, uint32_t, Texture&) = 0;
  virtual int DestroyTexture(Texture&) = 0;
  virtual int CreateResourceView(Texture&,ResourceView&) = 0;
  virtual int DestroyResourceView(ResourceView&) = 0;
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