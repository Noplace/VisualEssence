#include "../ve.h"
#include "../util/WICTextureLoader.h"

namespace ve {

bool TextureResource::Load() {
  if (loaded_ == true) 
    return false;

  auto gfx = (ContextD3D11*)manager_->context();
  auto res = DirectX::CreateWICTextureFromFile(gfx->device(),filepath_,&texture_,&srv_);


  if (res == S_OK) {
    loaded_ = true;
    return true;
  }
 
  return false;
}

bool TextureResource::Unload() {
  if (loaded_ == false) 
    return false;
  SafeRelease(&srv_);
  SafeRelease(&texture_);
  loaded_ = false;
  return S_OK;
}

}
