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
#ifndef GRAPHICS_FONT_WRITER_H
#define GRAPHICS_FONT_WRITER_H


namespace graphics {
namespace font {

/*
struct Vertex2 {
    XMFLOAT4 rect;
    XMFLOAT2 tex;
    XMFLOAT4 col;
    UINT     ch;
    UINT     page;
 };*/


/*struct Vertex {
    XMFLOAT2 pos;
    XMFLOAT2 tex;
    XMFLOAT4 col;
    uint32_t     ch;
    uint32_t     page;
    public: 
     Vertex() : pos(XMFLOAT2(0,0)),tex(XMFLOAT2(0,0)),col(XMFLOAT4(0,0,0,0)),ch(0),page(0) {
     }
     Vertex(XMFLOAT2 pos,XMFLOAT2 tex,XMFLOAT4 col,UINT ch,UINT page) {
      this->pos = pos;
      this->tex = tex;
      this->col = col;
      this->ch = ch;
      this->page = page;
    }
};*/

class Writer : public graphics::shape::Shape {
 public:
  /*struct ShaderMiscBuffer {
    XMMATRIX transform;
    float global_alpha;
    XMFLOAT4 Color;
  };*/
  Writer();
  ~Writer();
  int Initialize(Context* context);
  int Deinitialize();
  int PrepareWrite(int count);
  int Write(const char *text, int count, unsigned int mode);
  int WriteML(const char *text, int count, unsigned int mode);
  int WriteBox(const char *text, int count, unsigned int mode, float width);
  int GetOutput(graphics::shape::Vertex* vertex_array,int* vertex_count,int* char_count);
  int UpdateConstantBuffer();
  int Construct(); 
  int Update();
  int Draw();
  acGraphics::Font* font() { return font_; }
  void set_font(acGraphics::Font* font) { font_ = font; }
  //void set_effect(graphics::Effect* effect) { effect_ = effect; }
  void set_global_alpha(float global_alpha) { /*misc_buffer_shader_.global_alpha = global_alpha;*/ }
  graphics::shader::ConstantBuffer2Type misc_buffer_shader_;
 protected:
  int InternalWrite(float x, float y, float z, const char *text, int count, float spacing);
  acGraphics::Font* font_;
  graphics::Buffer vertex_buffer_;
  graphics::shape::Vertex* vertex_array_;
  //graphics::Effect* effect_;
  //graphics::Camera camera_;
  int vcount;
  int char_count;
  //graphics::Buffer misc_buffer_;

};

}
}

#endif