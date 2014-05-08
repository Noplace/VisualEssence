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

/*class Sprite : public ve::shape::Shape {
 public:
  Sprite() {}
  ~Sprite() {}
  int Initialize(Context* context);
  int Deinitialize();
  int SetSize(float width,float height);
  int SetColor(dxp::XMCOLOR color);
  int SetUV(float u0,float v0,float u1,float v1);
  int Construct();
  int Update();
  int Draw();
  dx::XMVECTOR color() { return dxp::XMLoadColor(&color_); }
 private:
  ve::Buffer vertex_buffer_;
  float scale_,angle_;
  float width_,height_;
  float u0,v0,u1,v1;
  dxp::XMCOLOR color_;
};*/


__declspec(align(16))
class Sprite : public ve::RenderObject {

 public:
  void *operator new( size_t stAllocateBlock) {
    return _aligned_malloc(sizeof(Sprite),16);
  }

  void   operator delete (void* p)  {
    return _aligned_free(p);
  }

  Sprite() : ve::RenderObject(),x_(0),y_(0),w_(1),h_(1),u0_(0),v0_(0),u1_(1.0f),v1_(1.0f),scale_(1.0f),color_(dx::XMFLOAT3(1, 1, 1)),angle_(0) {
    
  }

  int Initialize(ve::Context* context) {
    ve::RenderObject::Initialize(context);
		D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
		vertexBufferData.pSysMem = nullptr;//verticies;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPositionColorTexture)*4, D3D11_BIND_VERTEX_BUFFER);
		context->CreateBuffer(&vertexBufferDesc,nullptr,(void**)&vb);

    world_ = dx::XMMatrixIdentity();
    dirty_ = 0xffffffff;
    UpdateVerticies();
    UpdateTransform();
    return S_OK;
  }

   int Deinitialize() {
    SafeRelease(&vb);
    return ve::RenderObject::Deinitialize();
  }

  int UpdateVerticies() {
    if (dirty_&ve::kRenderObjectDirtySize) {
      dirty_ &= ~ve::kRenderObjectDirtySize;
      VertexPositionColorTexture verticies[] = 
		  {
			  {dx::XMFLOAT3( 0.0f,  0.0f, 0.0f) , dx::XMFLOAT3(1, 1, 1) , dx::XMFLOAT2(u0_,v0_)},
			  {dx::XMFLOAT3( w_,    0.0f, 0.0f) , dx::XMFLOAT3(1, 1, 1) , dx::XMFLOAT2(u1_,v0_)},
			  {dx::XMFLOAT3( 0.0f,  h_,   0.0f) , dx::XMFLOAT3(1, 1, 1) , dx::XMFLOAT2(u0_,v1_)},
			  {dx::XMFLOAT3( w_,    h_,   0.0f) , color_ , dx::XMFLOAT2(u1_,v1_)},

		  };
      return context_->CopyToVertexBuffer((void*)vb,verticies,sizeof(VertexPositionColorTexture),0,4);
    } else return S_FALSE;
  }

  int UpdateTransform() {
	  if (dirty_&ve::kRenderObjectDirtyTransform) {
        dirty_ &= ~ve::kRenderObjectDirtyTransform;
        world_ = dx::XMMatrixAffineTransformation2D(dx::XMVectorSet(scale_,scale_,1,0),
                dx::XMVectorSet(w_/2.0f,h_/2.0f,0,0),angle_,dx::XMVectorSet(x_,y_,0,0));
      return S_OK;
    } else return S_FALSE;
  }
 

  int Update(float timeTotal, float timeDelta) {
    UpdateVerticies();
    UpdateTransform();
    return S_OK;
  }

  int Render() {
	  UINT stride = sizeof(VertexPositionColorTexture);
	  UINT offset = 0;
    context_->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    context_->SetVertexBuffers(0,1,(const void**)&vb,&stride,&offset);
    context_->Draw(4,0);
    return S_OK;
  }

  void set_angle(float angle) { angle_ = angle; dirty_ |= ve::kRenderObjectDirtyTransform; }
  void set_position(float x, float y) { x_ = x; y_ = y; dirty_ |= ve::kRenderObjectDirtyTransform; }
 private:
	ID3D11Buffer* vb;
  float x_,y_,w_,h_,angle_,scale_;
  float u0_,v0_,u1_,v1_;
  dx::XMFLOAT3 color_;

};


}
