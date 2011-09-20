#ifndef GRAPHICS_EFFECT_H
#define GRAPHICS_EFFECT_H

#include "../context/contextd3d11.h"

namespace graphics {



class Effect : public Component {
 public:
  struct Entry {
    char* name;
    char* version;
  };
  Effect();
  virtual ~Effect();
  int Deinitialize();
  int CreateFromMemory(void* data, uint32_t size);
  int CreateFromMemory2(Entry* entry_list, void* data, uint32_t size);
  int Begin();
  VertexShader& vertex_shader() { return vertex_shader_; }
  PixelShader&  pixel_shader() { return pixel_shader_; }
  InputLayout&  input_layout() { return input_layout_; }
  VertexShader& vs(int index) { return vs_list_[index]; }
  PixelShader& ps(int index) { return ps_list_[index]; }
  GeometryShader& gs(int index) { return gs_list_[index]; }
 protected:
  int CreateInputLayoutFromShader(ShaderBlob& vs_blob);
  VertexShader vertex_shader_;
  PixelShader  pixel_shader_;
  GeometryShader geometry_shader_;
  //for each vertex shader
  InputLayout input_layout_;
  VertexShader* vs_list_;
  PixelShader* ps_list_;
  GeometryShader* gs_list_;
  int ps_count;
  int vs_count;
  int gs_count;
};

}

#endif