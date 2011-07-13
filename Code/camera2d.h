#ifndef GRAPHICS_CAMERA2D_H
#define GRAPHICS_CAMERA2D_H

namespace graphics {

class Camera2D : public Component {
 public:
  Camera2D() {

  }
  ~Camera2D() {

  }

  void SetupDisplay() {
    RECT rect;
	  GetClientRect(context()->window()->handle(),&rect);
    //D3DXMatrixOrthoLH(&Ortho2D, (FLOAT)rect.right, (FLOAT)rect.bottom, 0.0f, 1.0f);
    //D3DXMatrixOrthoOffCenterLH(&Ortho2D, 0.0f,(FLOAT)rect.right,(FLOAT)rect.bottom,0.0f,0.0f,1.0f);
    FLOAT ratio = (FLOAT)640/480;
    //D3DXMatrixOrthoOffCenterLH(&projection_, 0.0f,ratio,1.0f,0.0f,0.0f,1.0f);
    projection_ = XMMatrixOrthographicOffCenterLH( 0.0f,ratio,1.0f,0.0f,0.0f,1.0f);
	  //D3DXMatrixIdentity(&view_);
    view_ = XMMatrixIdentity();
  }

  void Use() {
    /*ContextD3D9* c = (ContextD3D9*)context();
    c->device()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    c->device()->SetRenderState( D3DRS_LIGHTING, FALSE );
    c->device()->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&projection_);
    c->device()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&view_);
	  c->device()->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&view_);*/
  }
 private:
  XMMATRIX view_;	
  XMMATRIX projection_;
};

}

#endif