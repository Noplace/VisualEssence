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
#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

namespace graphics {

struct TexturePoint {
  uint32_t x,y;
  TexturePoint() : x(0),y(0) {
  }
  TexturePoint(uint32_t x,uint32_t y) : x(x),y(y) {
  }

  TexturePoint& operator=(const TexturePoint& other) {
    this->x = other.x;
    this->y = other.y;
    return *this;
  }
};

struct TextureRegion {
  uint32_t x,y,w,h;
  TextureRegion() : x(0),y(0),w(0),h(0) {
  }
  TextureRegion(uint32_t x,uint32_t y,uint32_t w,uint32_t h) : x(x),y(y),w(w),h(h) {
  }

  TextureRegion& operator=(const TextureRegion& other) {
    this->x = other.x;
    this->y = other.y;
    this->w = other.w;
    this->h = other.h;
    return *this;
  }
};

class Texture {
 public:
  Texture() : data_pointer(nullptr),data_length(0) {}
  virtual ~Texture() {
    //SafeRelease(&data_pointer);
  }
  void* data_pointer;
  size_t data_length;
  uint32_t format,type;
};

}

#endif