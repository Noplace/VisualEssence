#ifndef SHADER2D_HELPER_H
#define SHADER2D_HELPER_H

#include "../component.h"
#include "../camera/camera.h"
#include "../effect/effect.h"

namespace graphics {
namespace shader {

struct ConstantBuffer2Type {
    XMMATRIX world;
    bool enable_texture;
    XMVECTOR ps_color;
};

class Shader2DHelper : public graphics::Component {
 public:
  Shader2DHelper();
  ~Shader2DHelper();
  int Initialize(Context* context);
  int Deinitialize();
  void CreateFromMemory(void* data_pointer, uint32_t data_length);
  int PrepareDraw();
  int UpdateChangesEveryFrame(ConstantBuffer2Type* cef);
  int SetColorShader();
  int SetTextureShader();
  int SetTexturePagesShader();
 private:
  graphics::Camera camera_;
  graphics::Effect effect_;
  graphics::Buffer cef_buffer;
};

}
}

#endif