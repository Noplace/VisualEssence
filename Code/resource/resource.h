/*****************************************************************************************************************
* Copyright (c) 2014 Khalid Ali Al-Kooheji                                                                       *
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

class ResourceManager;

enum Type {
  RESOURCE_TYPE_NULL = 0,
  RESOURCE_TYPE_GRAPHICS = 1,
  RESOURCE_TYPE_MOVIE = 2,
  RESOURCE_TYPE_AUDIO = 3,
  RESOURCE_TYPE_TEXT = 4,
  RESOURCE_TYPE_BMFONT = 5,
  RESOURCE_TYPE_EFFECT = 6,
  RESOURCE_TYPE_OTHER = 100
};

class Resource  {
 public:
  Resource() : type_(RESOURCE_TYPE_NULL),scope_(0),uid_(0),loaded_(false),data_pointer(null),data_length(0),manager_(null) {}
  Resource(const Resource& src) {
    set_scope(src.scope_);
    set_uid(src.uid_);
    set_type(src.type_);
    set_filename(src.filename_);
    set_manager(src.manager_);
    loaded_ = src.loaded_;
    data_pointer = src.data_pointer;
    data_length = src.data_length;
  }
  virtual ~Resource()  {
    DeallocateMemory();
  }
  int ReadWholeFile(bool binary);
  int DeallocateMemory();
  virtual bool Load();
  virtual bool Unload();
  uint32_t scope() { return scope_; }
  void set_scope(uint32_t scope) { scope_ = scope; }
  void set_uid(uint32_t uid) { uid_ = uid; }
  void set_filename(const wchar_t* filename);
  void set_filepath(const wchar_t* filepath);
  ResourceManager* manager() { return manager_; }
  void set_manager(ResourceManager* manager) { manager_ = manager; }
  void set_type(Type type) { type_ = type; }
  Type type() { return type_; }
  const wchar_t* filename() { return filename_; }
  const wchar_t* filepath() { return filepath_; }
  uint32_t uid() { return uid_; }
  bool loaded() { return loaded_; }
  void* data_pointer;
  size_t data_length; 
 protected:
  ResourceManager* manager_;
  Type type_;
  uint32_t scope_;
  uint32_t uid_;
  wchar_t filename_[256];
  wchar_t filepath_[1024];
  bool loaded_;

};

}
