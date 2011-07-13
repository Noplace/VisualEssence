#ifndef GRAPHICS_VERTEX_H
#define GRAPHICS_VERTEX_H

namespace graphics {
  

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

#endif