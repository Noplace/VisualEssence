#ifndef GRAPHICS_DRAWABLE_H
#define GRAPHICS_DRAWABLE_H

#include "component.h"

namespace graphics {

#pragma pack(push,1)
struct DrawableId {
  union {
    uint64_t id;
    struct {
      unsigned fullscreen_layer:2;
      unsigned viewport:3;
      unsigned viewport_layer:3;
      unsigned translucency_type:2;
      unsigned command_flag : 1;
      union {
        struct {
          unsigned seq:21;
          unsigned id:32;
        }command;
        struct {
          unsigned depth:24;
          unsigned material:26;
          unsigned pass:3;
        }item;
      };
    } rec;
  };
};
#pragma pack(pop)

class Drawable : public Component {
 public:
  void set_id(DrawableId id) { id_ = id; }
  DrawableId id() { return id_; }
  virtual int Draw() = 0;
 protected:
  DrawableId id_;

};

}

#endif