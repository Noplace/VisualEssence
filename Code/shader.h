#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include <d3d11.h>

namespace graphics {

enum ShaderType {
  kShaderTypeVertex = 0,
  kShaderTypePixel = 1,
  kShaderTypeCompute = 2,
  kShaderTypeHull = 3,
  kShaderTypeGeometry = 4,
  kShaderTypeDomain = 5,
  kShaderTypeUnknown = -1,
};

class ShaderBlob {
 public:
  ShaderBlob() : internal_(NULL) { }
  ~ShaderBlob() { SafeRelease(&internal_); }
  void* data() { return internal_->GetBufferPointer(); }
  size_t size() { return internal_->GetBufferSize(); }
  ID3DBlob* internal_;
private:

};

class Shader {
 public:
  Shader() : internal_pointer_(NULL),uid_(0) { type = kShaderTypeUnknown; }
  virtual ~Shader() { }
  uint32_t uid() { return uid_; }
  void set_uid(uint32_t uid) { uid_ = uid; }
  void* internal_pointer() const { return internal_pointer_; }
  void set_internal_pointer(void* internal_pointer) { internal_pointer_ = internal_pointer; }
  ShaderType type;
protected:
  uint32_t uid_;
  void* internal_pointer_;
};

class VertexShader : public Shader {
 public:
  VertexShader()  : Shader() { type = kShaderTypeVertex; }

};

class PixelShader : public Shader {
 public:
  PixelShader() : Shader() { type = kShaderTypePixel;  }
};

class GeometryShader : public Shader {
 public:
  GeometryShader() : Shader() { type = kShaderTypeGeometry;  }
};
}

#endif