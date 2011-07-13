#ifndef GRAPHICS_EFFECT_H
#define GRAPHICS_EFFECT_H

#include "contextd3d11.h"

namespace graphics {

class Effect : public Component {
 public:
  Effect();
  virtual ~Effect();
  int Deinitialize();
  int CreateFromMemory(void* data,uint32_t size);
  int Begin();
  VertexShader& vertex_shader() { return vertex_shader_; }
  PixelShader&  pixel_shader() { return pixel_shader_; }
 protected:
  VertexShader vertex_shader_;
  PixelShader  pixel_shader_;
  GeometryShader geometry_shader_;
  ID3D11InputLayout*  input_layout_;
};

}

#endif