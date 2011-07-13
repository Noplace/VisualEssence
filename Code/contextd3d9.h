#ifndef GRAPHICS_CONTEXTD3D9_H
#define GRAPHICS_CONTEXTD3D9_H

#include "context.h"
#include <d3d9.h>
#include <d3dx9.h>


namespace graphics {

class Component;
class Camera2D;
template<typename T>
class VertexBuffer;

class ContextD3D9 : public Context {
 public:
  static const D3DVERTEXELEMENT9 ve_xyzc[3];
  static const D3DVERTEXELEMENT9 ve_xyzcuv[4];

  ContextD3D9();
  ~ContextD3D9();
  int Initialize();
  int CreateDisplay(core::windows::Window* window);
  int Render();
  int ClearTarget();
  int Begin();
  int End();
  int CreateBuffer(BufferDescription& buffer_description,void** buffer_pointer);
  int SetVertexBuffers(uint32_t start_slot, uint32_t buffer_count, void *const * buffers, const uint32_t * strides,const uint32_t *offsets);
  LPDIRECT3DDEVICE9 device() { return device_; }
private:
  D3DDISPLAYMODE* display_modes_;
  LPDIRECT3D9 direct3d_;
  LPDIRECT3DDEVICE9 device_;
  D3DPRESENT_PARAMETERS d3dpp;
};

}


#endif