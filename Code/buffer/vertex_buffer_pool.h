
namespace ve {

template<typename T>
class VertexBufferPool : public Component {
 public:
  VertexBufferPool() : initialized(false), Component()  {
    memset(&buffer,0,sizeof(buffer));
  }
  ~VertexBufferPool() { 
    Deinitialize(); 
  }
  int Initialize(Context* context,int vertex_count) { 
    if (initialized == true)
      return S_FALSE;
    Component::Initialize(context);

    buffer.description.byte_width = sizeof(T)*vertex_count;
    buffer.description.cpu_access_flags = 0;
    buffer.description.bind_flags = D3D11_BIND_VERTEX_BUFFER;
    buffer.description.usage = D3D11_USAGE_DEFAULT;
    auto hr = context_->CreateBuffer(buffer,nullptr);
    initialized = true;
    return hr; 
  }
  int Deinitialize() { 
    if (initialized == false)
      return S_FALSE;
    context_->DestroyBuffer(buffer);
    initialized = false;
    return S_OK; 
  }

  int Allocate(int vertex_count) {
    uint32_t offset = 0;
    int id = 0;
    for (auto i = table.begin(); i != table.end(); ++i) {
      offset += (*i).count;
      ++id;
    }
    table.push_back(TableItem(offset,vertex_count));
    return id;
  }
  int CopyData(int id,void* data,uint32_t vertex_count) {
    if (vertex_count > table[id].count)
      return S_FALSE;
    return context_->CopyToVertexBuffer(buffer,data,sizeof(T),table[id].offset,table[id].count);
  }
  int Set(int id,int slot) {
    UINT stride = sizeof(T);
    uint32_t offset = table[id].offset;
    return context_->SetVertexBuffers(slot,1,&buffer,&stride,&offset);
  }
 private:
  struct TableItem {
    uint32_t offset,count;
    TableItem(uint32_t offset,uint32_t count) {
      this->offset = offset;
      this->count = count;
    }
    TableItem(const TableItem& other) {
      this->offset = other.offset;
      this->count = other.count;
    }

    TableItem& operator=(const TableItem& other) {
      this->offset = other.offset;
      this->count = other.count;
      return *this;
    }
  };
  int Initialize(Context* context) { context_ = context; return S_OK; }
  bool initialized;
  Buffer buffer;
  std::vector<TableItem> table;
};


/*VertexBufferPool<int> tj;

void test() {
  tj.Allocate(20);
}*/

}