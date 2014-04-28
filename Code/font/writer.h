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
namespace font {

/*
struct Vertex2 {
    dx::XMFLOAT4 rect;
    dx::XMFLOAT2 tex;
    dx::XMFLOAT4 col;
    UINT     ch;
    UINT     page;
 };*/


/*struct Vertex {
    dx::XMFLOAT2 pos;
    dx::XMFLOAT2 tex;
    dx::XMFLOAT4 col;
    uint32_t     ch;
    uint32_t     page;
    public: 
     Vertex() : pos(dx::XMFLOAT2(0,0)),tex(dx::XMFLOAT2(0,0)),col(dx::XMFLOAT4(0,0,0,0)),ch(0),page(0) {
     }
     Vertex(dx::XMFLOAT2 pos,dx::XMFLOAT2 tex,dx::XMFLOAT4 col,UINT ch,UINT page) {
      this->pos = pos;
      this->tex = tex;
      this->col = col;
      this->ch = ch;
      this->page = page;
    }
};*/

class Writer : public ve::shape::Shape {
 public:
  /*struct ShaderMiscBuffer {
    dx::XMMATRIX transform;
    float global_alpha;
    dx::XMFLOAT4 Color;
  };*/
  Writer();
  ~Writer();
  int Initialize(Context* context);
  int Deinitialize();
  int PrepareWrite(int count);
  int Write(const char *text, int count, unsigned int mode);
  int WriteML(const char *text, int count, unsigned int mode);
  int WriteBox(const char *text, int count, unsigned int mode, float width);
  int GetOutput(ve::shape::Vertex* vertex_array,int* vertex_count,int* char_count);
  int UpdateConstantBuffer();
  int Construct(); 
  int Update();
  int Draw();
  acGraphics::Font* font() { return font_; }
  void set_font(acGraphics::Font* font) { font_ = font; }
  //void set_effect(ve::Effect* effect) { effect_ = effect; }
  void set_global_alpha(float global_alpha) { /*misc_buffer_shader_.global_alpha = global_alpha;*/ }
  ve::shader::ConstantBuffer2Type misc_buffer_shader_;
 protected:
  int InternalWrite(float x, float y, float z, const char *text, int count, float spacing);
  acGraphics::Font* font_;
  ve::Buffer vertex_buffer_;
  ve::shape::Vertex* vertex_array_;
  //ve::Effect* effect_;
  //ve::Camera camera_;
  int vcount;
  int char_count;
  //ve::Buffer misc_buffer_;

};

}
}

