#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include "component.h"
#include "../camera/camera.h"
#include "../shader/shader.h"

namespace graphics {

class Scene : public Component {
 public:
  Scene() { }
  virtual ~Scene() { }
  virtual int Initialize(Context* context) {
    int hr = S_OK;
    hr = Component::Initialize(context);
    hr = camera_.Initialize(context);
    return hr;
  }
  virtual int Deinitialize() {
    int hr = S_OK;
    hr = camera_.Deinitialize();
    return hr;
  }
  virtual void Draw() = 0;
 protected:

  Camera camera_;
  VertexShader vertex_shader_;
  PixelShader pixel_shader_;
};

}

#endif