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
#include "../../libs/rapidxml/rapidxml.hpp"
#include "../ve.h"

namespace ve {

ResourceManager::ResourceManager() {
  heap_ = HeapCreate(0,1024*1024*2,1024*1024*200);
  auto_load = true;
  //HeapAlloc(heap_,
}

ResourceManager::~ResourceManager() {
  HeapDestroy(heap_);
}

int ResourceManager::UnloadAll() {
  std::vector<Resource*>::iterator i;
  for (i = resource_list_.begin(); i!= resource_list_.end(); ++i) {
    Resource* res = (*i);
    res->Unload();
    delete res;
  }
  return S_OK;
}

bool ResourceManager::LoadXmlFromFile(wchar_t* filename) {
  std::wstring filename_str(filename);
  std::wstring path = filename_str.substr(0,filename_str.find_last_of('\\')+1);
  
  std::wifstream ifs( filename );
  if ( ! ifs ) {
    return false;
  }

  const std::wstring file_content( (std::istreambuf_iterator<wchar_t>( ifs )), std::istreambuf_iterator<wchar_t>() );
  wchar_t* text = const_cast<wchar_t*>(file_content.c_str());

  FileData fd={0};
  fd.data = text;
  return LoadXmlFromMemory(fd);
}

bool ResourceManager::LoadXmlFromMemory(FileData& fd) {
  wchar_t* text = const_cast<wchar_t*>((wchar_t*)fd.data);
  rapidxml::xml_document<wchar_t> doc;
  doc.parse<0>(text);

  rapidxml::xml_node<wchar_t> *node = doc.first_node();

  if (wcscmp(node->name(),L"resources")) {
    return false;
  }

  for (rapidxml::xml_node<wchar_t> *cnode = node->first_node();
      cnode; cnode = cnode->next_sibling()) {

      if (wcscmp(cnode->name(),L"resource")==0) {

        Type type = RESOURCE_TYPE_NULL;
        Resource* resource = NULL;
        for (rapidxml::xml_attribute<wchar_t> *attr = cnode->first_attribute(); attr; attr = attr->next_attribute()) {
          if (!wcscmp(attr->name(),L"type")) {
            type = (Type)_wtoi(attr->value());

          }
        }

        switch (type) {
          case RESOURCE_TYPE_GRAPHICS:
            resource = nullptr;//new TextureResource();
            break;
          case RESOURCE_TYPE_BMFONT:
            resource = nullptr;//new FontResource();
            break;
          case RESOURCE_TYPE_EFFECT:
            resource = nullptr;//new EffectResource();
            break;
          default:
            resource = new Resource();
            break;
        }

        for (rapidxml::xml_attribute<wchar_t> *attr = cnode->first_attribute(); attr; attr = attr->next_attribute()) {

          if (!wcscmp(attr->name(),L"uid")) {
            resource->set_uid(_wtoi(attr->value()));
          }

          if (!wcscmp(attr->name(),L"scope")) {
            resource->set_scope(_wtoi(attr->value()));

          }

          if (!wcscmp(attr->name(),L"filename")) {
            auto filename = wcsrchr(attr->value(),'\\');
            ++filename;
            resource->set_filepath(attr->value());
            resource->set_filename(filename);
          }

          if (!wcscmp(attr->name(),L"type")) {
            resource->set_type((Type)_wtoi(attr->value()));
          }
        }
        resource->set_manager(this);
        resource_list_.push_back(resource);
        //scope_list_[resource.scope()].push_back(&resource_list_.back());


      }
  }
 
  return true;
}

/*
template<class T>
T* ResourceManager::GetResourceById(uint32_t uid) {

}*/

}