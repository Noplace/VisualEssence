#ifndef GRAPHICS_FONT_WRITER_H
#define GRAPHICS_FONT_WRITER_H

#include <Windows.h>
#include <xnamath.h>
#include "../component.h"
#include "../context/context.h"
#include "../effect/effect.h"
#include "../camera/camera.h"
#include "bmfont/font.h"

namespace graphics {
namespace font {


struct Vertex {
    XMFLOAT2 Pos;
    XMFLOAT2 Tex;
    XMFLOAT4 Color;
    UINT     Channel;
    UINT     Page;
};

class Writer : public graphics::Component {
 public:
  Writer();
  ~Writer();
  int Initialize(Context* context);
  int Deinitialize();
  int PrepareWrite(int count);
  int Write(float x, float y, float z, const char *text, int count, unsigned int mode);
  int WriteML(float x, float y, float z, const char *text, int count, unsigned int mode);
  int WriteBox(float x, float y, float z, float width, const char *text, int count, unsigned int mode);
  int Draw(int count);
  acGraphics::Font* font() { return font_; }
  void set_font(acGraphics::Font* font) { font_ = font; }
  void set_effect(graphics::Effect* effect) { effect_ = effect; }
 protected:
  int InternalWrite(float x, float y, float z, const char *text, int count, float spacing);
  acGraphics::Font* font_;
  graphics::Buffer vertex_buffer_;
  Vertex* vertex_array_;
  graphics::Effect* effect_;
  graphics::Camera camera_;
  int vcount;
};

}
}

#endif