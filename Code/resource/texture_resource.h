#pragma once

namespace ve {

class ResourceManager;

class TextureResource : public Resource {
 public:
  TextureResource() : Resource() { }
  virtual ~TextureResource()  {}
  virtual bool Load();
  virtual bool Unload();
  ID3D11ShaderResourceView* srv() { return srv_; }
  ID3D11Resource* texture() { return texture_; }
 protected:
  ID3D11Resource* texture_;
  ID3D11ShaderResourceView* srv_;

};

}

