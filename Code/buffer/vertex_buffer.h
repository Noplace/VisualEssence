#ifndef GRAPHICS_VERTEX_BUFFER_H
#define GRAPHICS_VERTEX_BUFFER_H

namespace graphics {

class Context;

template <typename T>
class VertexBuffer {
public:
  VertexBuffer() : context_(NULL),internal_buffer_(NULL)  {}
  ~VertexBuffer() { Release(); }
  int Initialize(Context* context,int vertex_count) { 
    Release();
    context_ = context; 
    int byte_count;
    HRESULT hr;
    byte_count=vertex_count*sizeof(T);
    BufferDescription desc;
    desc.byte_width = byte_count;
    desc.cpu_access_flags = 0;
    desc.bind_flags = D3D11_BIND_VERTEX_BUFFER;
    desc.usage = D3D11_USAGE_DEFAULT;
    hr = context_->CreateBuffer(desc,(void**)&internal_buffer_);

    return hr; 
    
  }

  int Release() {
    if (internal_buffer_ != NULL)
      return context_->DestroyBuffer(internal_buffer_);
    return S_OK;
  }

  T* Lock(int start,int count) {
    T* output;
    //internal_buffer_->Lock(start*sizeof(T),count*sizeof(T),(void**)&output,0);
    BufferSubresource sub;
    context_->LockBuffer(internal_buffer_,0,3,&sub);
    output = sub.data;
    return output;
  }

  int Unlock() {
    return context_->UnlockBuffer(internal_buffer_,0);
  }

  int Use() {
    uint32_t offset[1] = { 0 };
    uint32_t stride[1] = {sizeof(T)};
    return context_->SetVertexBuffers(0,1,(void**)&internal_buffer_,stride, offset);
    //return context_-> device()->SetStreamSource(0,internal_buffer_,0,sizeof(T));
  }

  Context* context() { return context_; }
private:
  Context* context_;
  void* internal_buffer_;
};

}

#endif