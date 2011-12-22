#ifndef FONT_FONT_H
#define FONT_FONT_H

namespace graphics {
namespace font {

class FontSprite : public graphics::shape::Shape  {
 public:
  int Initialize(Context* context);
  int Deinitialize();
  int Draw();
 protected:
  int char_count;
};

}
}

#endif