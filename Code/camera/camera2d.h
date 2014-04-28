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

class Camera2D : public Component {
 public:
  Camera2D() {

  }
  ~Camera2D() {

  }

  void SetupDisplay() {
    RECT rect;
	  GetClientRect(context()->window_handle(),&rect);
    //D3Ddx::XMatrixOrthoLH(&Ortho2D, (FLOAT)rect.right, (FLOAT)rect.bottom, 0.0f, 1.0f);
    //D3Ddx::XMatrixOrthoOffCenterLH(&Ortho2D, 0.0f,(FLOAT)rect.right,(FLOAT)rect.bottom,0.0f,0.0f,1.0f);
    FLOAT ratio = (FLOAT)640/480;
    //D3Ddx::XMatrixOrthoOffCenterLH(&projection_, 0.0f,ratio,1.0f,0.0f,0.0f,1.0f);
    projection_ = dx::XMMatrixOrthographicOffCenterLH( 0.0f,ratio,1.0f,0.0f,0.0f,1.0f);
	  //D3Ddx::XMatrixIdentity(&view_);
    view_ = dx::XMMatrixIdentity();
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
  dx::XMMATRIX view_;	
  dx::XMMATRIX projection_;
};

}

