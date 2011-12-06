#ifndef GRAPHICS_CAMERA_H
#define GRAPHICS_CAMERA_H

#include "../component.h"
#include "../context/context.h"

namespace graphics {

class Camera : public Component {
 public:
  struct ConstantBuffer {
    XMMATRIX mView;
    XMMATRIX mProjection;
  };

  Camera() : Component(),ratio_(0) {

  }
  virtual ~Camera() {

  }

  virtual int Initialize(Context* context) {
    int hr = S_OK;
    Component::Initialize(context);
    // Create the camera constant buffer
    cam_buffer_.description.usage = D3D11_USAGE_DEFAULT;
    cam_buffer_.description.byte_width = sizeof(ConstantBuffer);
    cam_buffer_.description.bind_flags = D3D11_BIND_CONSTANT_BUFFER;
    cam_buffer_.description.cpu_access_flags = 0;
    hr = context_->CreateBuffer(cam_buffer_,NULL);
    if( FAILED( hr ) )
        return hr;

    return hr;
  }


  virtual int Deinitialize() {
    if (context_ == NULL) 
      return -1;
    return context_->DestroyBuffer(cam_buffer_);
  }

  void Ortho2D() {
    //RECT rect;
	  //GetClientRect(context()->window()->handle(),&rect);
    //D3DXMatrixOrthoLH(&Ortho2D, (FLOAT)rect.right, (FLOAT)rect.bottom, 0.0f, 1.0f);
    //D3DXMatrixOrthoOffCenterLH(&Ortho2D, 0.0f,(FLOAT)rect.right,(FLOAT)rect.bottom,0.0f,0.0f,1.0f);
    ratio_ = (FLOAT)context()->width()/context()->height();
    //D3DXMatrixOrthoOffCenterLH(&projection_, 0.0f,ratio,1.0f,0.0f,0.0f,1.0f);
    projection_ = XMMatrixOrthographicOffCenterLH(0.0f,(FLOAT)context()->width(),(FLOAT)context()->height(),0.0f,-10000.0f,10000.0f);
	  //D3DXMatrixIdentity(&view_);
    view_ = XMMatrixIdentity();
  }

  inline XMFLOAT2 OrthoPoint(FLOAT rx, FLOAT ox, FLOAT ry, FLOAT oy) {
    return XMFLOAT2((rx*(FLOAT)context()->width()) + ox,(ry*(FLOAT)context()->height()) + oy);
  }

  void Perspective() {
    ratio_ = (FLOAT)context()->width()/context()->height();
     projection_ = XMMatrixPerspectiveFovLH( XM_PIDIV4, ratio_, 0.01f, 100.0f );
     //view_ = XMMatrixIdentity();
  }

  void UpdateConstantBuffer() {
    ConstantBuffer cbCamera;
    cbCamera.mView = view_transposed();
    cbCamera.mProjection = projection_transposed();
    context_->UpdateSubresource(cam_buffer_,&cbCamera,NULL,0,0);
  }

  void SetConstantBuffer(uint32_t slot) {
    //send to vertex shader only
    context_->SetConstantBuffers(kShaderTypeVertex,slot,1,&cam_buffer_);
  }

  float ratio() { return ratio_; }
  XMMATRIX& projection() { return projection_;  }
  XMMATRIX& view() { return view_;  }
  XMMATRIX projection_transposed() { return XMMatrixTranspose(projection_);  }
  XMMATRIX view_transposed() { return XMMatrixTranspose(view_);  }
 private:
  float ratio_;
  XMMATRIX view_;	
  XMMATRIX projection_;
  graphics::Buffer cam_buffer_;
};

}

#endif