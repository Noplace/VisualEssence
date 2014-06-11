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
#include "../ve.h"

#define MAX_SPRITE_COUNT 2000

namespace ve {

__declspec(align(16))
struct SpriteBatchVSConstantBuffer {
  void *operator new( size_t stAllocateBlock) {
    return _aligned_malloc(sizeof(SpriteBatchVSConstantBuffer),16);
  }

  void   operator delete (void* p)  {
    return _aligned_free(p);
  }
  struct SpriteInfo{
    float x,y,z,scale;
    float angle;
    dx::XMFLOAT2 rot_origin;
    float opacity;
  } sprite_info[MAX_SPRITE_COUNT];
};

struct SpriteBatchVertex
{
	dx::XMFLOAT3 pos;
	dx::XMFLOAT3 color;
  dx::XMFLOAT2 uv;
  uint32_t index;
};

const D3D11_INPUT_ELEMENT_DESC SpriteBatchVertexElementDesc[] = 
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "BLENDINDICES", 0, DXGI_FORMAT_R32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};



__declspec(align(16))
class SpriteBatchSprite : public ve::RenderObject {

 public:
  void *operator new( size_t stAllocateBlock) {
    return _aligned_malloc(sizeof(SpriteBatchSprite),16);
  }

  void   operator delete (void* p)  {
    return _aligned_free(p);
  }

  SpriteBatchSprite() : ve::RenderObject(),x_(0),y_(0),zorder_(0),w_(1),h_(1),u0_(0),v0_(0),u1_(1.0f),v1_(1.0f),scale_(1.0f),color_(dx::XMFLOAT3(1, 1, 1)),angle_(0) {
    
  }

  int Initialize(ve::Context* context) {
    ve::RenderObject::Initialize(context);
    world_ = dx::XMMatrixIdentity(); //not used atm
    dirty_ = 0xffffffff;
    return S_OK;
  }

   int Deinitialize() {
    return ve::RenderObject::Deinitialize();
  }

  int UpdateVerticies() {
    if (dirty_&ve::kRenderObjectDirtySize) {
      dirty_ &= ~ve::kRenderObjectDirtySize;
     return S_OK;
    } else return S_FALSE;
  }

  int UpdateTransform() {
	  if (dirty_&ve::kRenderObjectDirtyTransform) {
        dirty_ &= ~ve::kRenderObjectDirtyTransform;
        //world_ = dx::XMMatrixAffineTransformation2D(dx::XMVectorSet(scale_,scale_,1,0),
        //        dx::XMVectorSet(w_/2.0f,h_/2.0f,0,0),angle_,dx::XMVectorSet(x_,y_,0,0));
      return S_OK;
    } else return S_FALSE;
  }
 

  int Update(float timeTotal, float timeDelta) {
    UpdateVerticies();
    UpdateTransform();
    return S_OK;
  }

  int Render() {
    return S_OK;
  }

  int GenerateVertices(SpriteBatchVertex* v) {
    if ( (dirty_&ve::kRenderObjectDirtySize) || (parent_->dirty_flag()&ve::kRenderObjectDirtySize) ) {
      dirty_ &= ~ve::kRenderObjectDirtySize;
      v[0].pos = dx::XMFLOAT3(0.0f,0.0f,0.0f);
      v[1].pos = dx::XMFLOAT3(w_,0.0f,0.0f);
      v[2].pos = dx::XMFLOAT3(0.0f,h_,0.0f);
      v[3].pos = dx::XMFLOAT3(w_,h_,0.0f);
      v[0].uv = dx::XMFLOAT2(u0_,v0_);
      v[0].uv = dx::XMFLOAT2(u0_,v0_);
      v[1].uv = dx::XMFLOAT2(u1_,v0_);
      v[2].uv = dx::XMFLOAT2(u0_,v1_);
      v[3].uv = dx::XMFLOAT2(u1_,v1_);
      v[0].color = v[1].color = v[2].color = v[3].color = color_;
      v[0].index = v[1].index = v[2].index = v[3].index = index;
     return S_OK;
    } else return S_FALSE;
  }

  int GenerateTransform(SpriteBatchVSConstantBuffer::SpriteInfo* t) {
	  if ( (dirty_&ve::kRenderObjectDirtyTransform) || (parent_->dirty_flag()&ve::kRenderObjectDirtyTransform) ) {
      dirty_ &= ~ve::kRenderObjectDirtyTransform;
      t->x = x_;
      t->y = y_;
      t->z = 0;
      t->angle = angle_;
      t->scale = scale_;
      t->rot_origin = rotation_origin_;
      t->opacity = opacity_;
      return S_OK;
    } else 
      return S_FALSE;
  }

  dx::XMFLOAT2 center() {
    return dx::XMFLOAT2(x_+w_*0.5f,y_+h_*0.5f) ;
  }

  dx::XMFLOAT2 size() const { return dx::XMFLOAT2(w_,h_); }
  void set_size(float w,float h) { w_ = w; h_ = h; dirty_ |= ve::kRenderObjectDirtySize; parent_->set_dirty_flag(ve::kRenderObjectDirtySize); }
  void set_scale(float scale) {scale_ = scale; dirty_ |= ve::kRenderObjectDirtyTransform; parent_->set_dirty_flag(ve::kRenderObjectDirtyTransform);  }
  void set_angle(float angle) { angle_ = angle; dirty_ |= ve::kRenderObjectDirtyTransform; parent_->set_dirty_flag(ve::kRenderObjectDirtyTransform); }
  void set_rotation_origin(dx::XMFLOAT2 rotation_origin) { rotation_origin_ = rotation_origin;  dirty_ |= ve::kRenderObjectDirtyTransform; parent_->set_dirty_flag(ve::kRenderObjectDirtyTransform);  }
  void set_position(float x, float y) { x_ = x; y_ = y; dirty_ |= ve::kRenderObjectDirtyTransform; parent_->set_dirty_flag(ve::kRenderObjectDirtyTransform); }
  void set_opacity(float opacity) { opacity_ = opacity;  dirty_ |= ve::kRenderObjectDirtyTransform; parent_->set_dirty_flag(ve::kRenderObjectDirtyTransform);  }
  void set_color(dx::XMFLOAT3 color) { color_ = color; dirty_ |= ve::kRenderObjectDirtySize; parent_->set_dirty_flag(ve::kRenderObjectDirtySize); }
  int zorder() const { return zorder_; }
  void set_zorder(int zorder) { zorder_ = zorder; set_dirty_flag(ve::kRenderObjectDirtySize); parent_->set_dirty_flag(ve::kRenderObjectDirtySize); }
  void set_uv(float u0, float v0, float u1, float v1) { u0_ = u0; v0_ = v0; u1_ = u1; v1_ = v1; set_dirty_flag(ve::kRenderObjectDirtySize); parent_->set_dirty_flag(ve::kRenderObjectDirtySize); }
  int index;
 private:
  int zorder_;
  float x_,y_,w_,h_,angle_,scale_;
  float u0_,v0_,u1_,v1_;
  dx::XMFLOAT3 color_;
  dx::XMFLOAT2 rotation_origin_;
};

class SpriteBatch : public ve::RenderObject {
 public:

  SpriteBatch() : RenderObject() , max_sprite_count_(MAX_SPRITE_COUNT),sprite_count_(0) {

  }

  int Initialize(ve::Context* context);
  int Deinitialize();
  int Render();
  int AddSprite(SpriteBatchSprite* sprite);
  int Compile();
  int UpdateVerticies();
  int UpdateTransform();
 

  int Update(float timeTotal, float timeDelta) {
    UpdateVerticies();
    UpdateTransform();
    return S_OK;
  }

  int max_sprite_count() const { return max_sprite_count_; }
  void set_max_sprite_count(int max_sprite_count) { max_sprite_count_ = max_sprite_count; }
 protected:
  int max_sprite_count_,sprite_count_;
  std::vector<SpriteBatchSprite*> render_list_;
  SpriteBatchVertex* vertices_data;
  SpriteBatchVSConstantBuffer* vs_cb_data_;
  ID3D11Buffer *vb_,*ib_,*vs_cb_;
  VertexShader vs_;
  PixelShader ps_;
  InputLayout input_layout_;
  ID3D11DepthStencilState* depth_state_;
};


}