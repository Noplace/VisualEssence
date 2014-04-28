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

#include "component.h"

namespace ve {

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
  Drawable() : Component() {}
  virtual ~Drawable() {}
  void set_id(DrawableId id) { id_ = id; }
  DrawableId id() { return id_; }
  virtual int Draw() = 0;
 protected:
  DrawableId id_;

};

}

