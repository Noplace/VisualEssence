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
  int CreateFromMemory2(Entry* entry_list, void* data, size_t size);
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

