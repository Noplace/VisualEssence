#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

namespace graphics {

class Context;

class Component {
 public:
  Component() : context_(NULL)  {}
  virtual ~Component() {}
  virtual int Initialize(Context* context) { context_ = context; return S_OK; }
  virtual int Deinitialize() { return S_OK; }
  Context* context() { return context_; }
protected:
  Context* context_;
};

}

#endif