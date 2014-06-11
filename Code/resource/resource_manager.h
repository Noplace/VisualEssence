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

#include "../ve.h"

namespace ve {

class ResourceManager : public Component {
 public:
  ResourceManager();
  ~ResourceManager();
  int UnloadAll();
  bool LoadXmlFromFile(wchar_t* filename);
  bool LoadXmlFromMemory(FileData& fd);
  template<class T>
  T* GetResourceById(uint32_t uid) {
    std::vector<Resource*>::iterator i;
    for (i = resource_list_.begin(); i!= resource_list_.end(); ++i) {
      T* res = (T*)(*i);
      if (res->uid() == uid) {
        if (auto_load == true)
          res->Load();
        return res;
      }
    }

    return NULL;
  }
  Resource* GetResourceById(uint32_t uid) {
    return GetResourceById<Resource>(uid);
  }
  template<class T>
  T* GetResourceByFilename(const wchar_t* filename) {
    std::vector<Resource*>::iterator i;
    for (i = resource_list_.begin(); i!= resource_list_.end(); ++i) {
      T* res = (T*)(*i);

      if (wcscmp(res->filename(),filename)==0) {
        if (auto_load == true)
          res->Load();
        return res;
      }
    }

    return NULL;
  }
  Resource* GetResourceByFilename(const wchar_t* filename) {
    return GetResourceByFilename<Resource>(filename);
  }
  HANDLE heap() { return heap_; }
 private:
  unsigned int resource_count_;
  std::vector<Resource*>  resource_list_;
  std::map<int,std::vector<Resource*> > scope_list_;
  HANDLE heap_;
  bool auto_load;
};

}
