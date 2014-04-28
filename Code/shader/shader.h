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
  void* data() { return data_;/*internal_->GetBufferPointer();*/ }
  void set_data(void* data) { data_ = data; }
  size_t size() { return size_;/*internal_->GetBufferSize();*/ }
  void set_size(size_t size) { size_ = size; }
  IUnknown* internal_;
private:
  void* data_;
  size_t size_;
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
  virtual ~VertexShader() {  }

 protected:


};

class PixelShader : public Shader {
 public:
  PixelShader() : Shader() { type = kShaderTypePixel;  }
};

class GeometryShader : public Shader {
 public:
  GeometryShader() : Shader() { type = kShaderTypeGeometry;  }
};

class HullShader : public Shader {
 public:
  HullShader() : Shader() { type = kShaderTypeHull;  }
};

class DomainShader : public Shader {
 public:
  DomainShader() : Shader() { type = kShaderTypeDomain;  }
};




}

