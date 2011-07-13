#ifndef GRAPHICS_BUFFER_H
#define GRAPHICS_BUFFER_H

#include <WinCore/types.h>

namespace graphics {

struct BufferDescription {
  uint32_t  byte_width;
  uint32_t  usage;
  uint32_t  bind_flags;
  uint32_t  cpu_access_flags;
  uint32_t  misc_flags;
  uint32_t  _byte_stride;
  BufferDescription():byte_width(0),usage(0),bind_flags(0),cpu_access_flags(0),misc_flags(0),_byte_stride(0) {}
  BufferDescription(const BufferDescription& other) {
    this->bind_flags = other.bind_flags;
    this->byte_width = other.byte_width;
    this->cpu_access_flags = other.cpu_access_flags;
    this->misc_flags = other.misc_flags;
    this->usage = other.usage;
    this->_byte_stride = other._byte_stride;
  }
};

struct BufferSubresource {
  void* data;
  uint32_t row_pitch;
  uint32_t depth_pitch;
};

class Buffer {
 public:
  Buffer() : internal_pointer(NULL) { }
  BufferDescription description;
  void* internal_pointer;
};

}

#endif 