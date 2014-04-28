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
  

/*
class VertexFormat {
 public:
  static void GetDecleration(Context* context) {
    context->device()->CreateVertexDeclaration(,&decleration);
    return decleration;
  }
 protected:
  Vertex() {};
  static IDirect3DVertexDeclaration9* decleration = NULL;
};*/

struct GameVertex {
	FLOAT x, y, z;
	DWORD color;
	FLOAT u, v;

};

class GameVertexDecl {
 public:
  GameVertexDecl():decleration(NULL) {}
  ~GameVertexDecl() { decleration->Release(); }
  void Initialize(ContextD3D9* context) {
    if (decleration == NULL) {
      context->device()->CreateVertexDeclaration(ContextD3D9::ve_xyzcuv,&decleration);
    }
  }
  
  void Use(ContextD3D9* context) {
    context->device()->SetVertexDeclaration(decleration);
  }
 private:
  IDirect3DVertexDeclaration9* decleration;
};

}

