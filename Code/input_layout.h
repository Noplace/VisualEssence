#ifndef GRAPHICS_INPUT_LAYOUT_H
#define GRAPHICS_INPUT_LAYOUT_H

namespace graphics {

class InputLayout {
 public:
  InputLayout() : pointer_(0) {}
  void* pointer() { return pointer_; }
  void set_pointer(void* pointer) { pointer_ = pointer; }
 private:
  void* pointer_;

};

}

#endif