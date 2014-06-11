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
#include <fstream>
#include "../ve.h"

namespace ve {

int Resource::ReadWholeFile(bool binary) {
   
  std::ios::openmode mode = std::ios::beg | std::ios::in;
  if (binary) {
    mode |= std::ios::binary;
  }
  std::ifstream ifs( filepath_,mode);
  if (type_ != 4) {
    ifs.seekg(0,std::ios::end);
    data_length = ifs.tellg();
    ifs.seekg(0,std::ios::beg);
    data_pointer = HeapAlloc(manager_->heap(),0,data_length);
    ifs.read((char*)data_pointer,data_length);
  }
  else {
    const std::string file_content( (std::istreambuf_iterator<char>( ifs )), std::istreambuf_iterator<char>() );
    data_length = file_content.length();
    data_pointer = HeapAlloc(manager_->heap(),0,data_length);
    memcpy((char*)data_pointer,file_content.c_str(),data_length);
  }
  
  ifs.close();

  return S_OK;
}

int Resource::DeallocateMemory() {
  if (data_pointer == NULL)
    return S_FALSE;
  if (HeapFree(manager_->heap(),0,data_pointer)!=0) {
    data_pointer = NULL;
    return S_OK;
  }
  else {
    //int a = GetLastError();
    return S_FALSE;
  }
}

bool Resource::Load() {
  if (loaded_ == true) 
    return false;
 
  if (ReadWholeFile(true) == S_OK) {
    loaded_ = true;
    return true;
  } else {
    return false;
  }
}

bool Resource::Unload() {
  if (loaded_ == false) 
    return false;
  if (DeallocateMemory() == S_OK) {
    loaded_ = false;
    return true;
  } else {
    return false;
  }

}

void Resource::set_filename(const wchar_t* filename) { 
  wcscpy_s<256>(filename_,filename); 
}

void Resource::set_filepath(const wchar_t* filepath) { 
  wcscpy_s<1024>(filepath_,filepath); 
}


}
