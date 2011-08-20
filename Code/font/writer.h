#ifndef GRAPHICS_FONT_WRITER_H
#define GRAPHICS_FONT_WRITER_H

#include "../component.h"
#include "../context/context.h"
#include "../effect/effect.h"
#include "../camera/camera.h"
#include "bmfont/font.h"

namespace graphics {
namespace font {


struct Vertex {
    XMFLOAT2 pos;
    XMFLOAT2 tex;
    XMFLOAT4 col;
    UINT     ch;
    UINT     page;
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
};

class Writer : public graphics::Component {
 public:
  struct ShaderMiscBuffer {
    XMMATRIX transform;
    float global_alpha;
    XMFLOAT4 Color;
  };
  Writer();
  ~Writer();
  int Initialize(Context* context);
  int Deinitialize();
  int PrepareWrite(int count);
  int Write(float x, float y, float z, const char *text, int count, unsigned int mode);
  int WriteML(float x, float y, float z, const char *text, int count, unsigned int mode);
  int WriteBox(float x, float y, float z, float width, const char *text, int count, unsigned int mode);
  int UpdateConstantBuffer();
  int Draw(int count);
  acGraphics::Font* font() { return font_; }
  void set_font(acGraphics::Font* font) { font_ = font; }
  void set_effect(graphics::Effect* effect) { effect_ = effect; }
  void set_global_alpha(float global_alpha) { misc_buffer_shader_.global_alpha = global_alpha; }
 protected:
  int InternalWrite(float x, float y, float z, const char *text, int count, float spacing);
  acGraphics::Font* font_;
  graphics::Buffer vertex_buffer_;
  Vertex* vertex_array_;
  graphics::Effect* effect_;
  graphics::Camera camera_;
  int vcount;
  ShaderMiscBuffer misc_buffer_shader_;
  graphics::Buffer misc_buffer_;

};

}
}

#endif