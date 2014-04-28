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
