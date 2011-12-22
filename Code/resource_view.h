#ifndef GRAPHICS_RESOURCE_VIEW_H
#define GRAPHICS_RESOURCE_VIEW_H

namespace graphics {

class ResourceView {
 public:
  ResourceView() : data_pointer(nullptr),data_length(0) {}
  virtual ~ResourceView() {
    //SafeRelease(&data_pointer);
  }
  void* data_pointer;
  uint32_t data_length;
};

}

#endif