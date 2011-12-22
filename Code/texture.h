#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

namespace graphics {

class Texture {
 public:
  Texture() : data_pointer(nullptr),data_length(0) {}
  virtual ~Texture() {
    //SafeRelease(&data_pointer);
  }
  void* data_pointer;
  uint32_t data_length;
};

}

#endif