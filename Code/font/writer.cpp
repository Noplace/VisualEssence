#include "writer.h"

namespace graphics {
namespace font {



Writer::Writer() {
  
}

Writer::~Writer() {
  
}

int Writer::Initialize(Context* context) {
  vertex_array_ = NULL;
  Component::Initialize(context);
  camera_.Initialize(context);
  camera_.Ortho2D();
  camera_.UpdateConstantBuffer();
  return S_OK;
}

int Writer::Deinitialize() {
  context_->DestroyBuffer(vertex_buffer_);
  if (vertex_array_)
    delete [] vertex_array_;
  camera_.Deinitialize();
  effect_->Deinitialize();
  delete font_;
  Component::Deinitialize();
  return S_OK;
}

int Writer::PrepareWrite(int count) {
  context_->DestroyBuffer(vertex_buffer_);
  vertex_buffer_.description.bind_flags = D3D11_BIND_VERTEX_BUFFER;
  vertex_buffer_.description.usage = D3D11_USAGE_DEFAULT;
  vertex_buffer_.description.byte_width = sizeof( Vertex ) * count * 4;
  vertex_buffer_.description.cpu_access_flags = 0;
  context_->CreateBuffer(vertex_buffer_,NULL);
  if (vertex_array_ != NULL)
    delete [] vertex_array_;
  vertex_array_ = NULL;
  vertex_array_ = new Vertex[count * 4];
  ZeroMemory(vertex_array_,sizeof(Vertex)*count*4);
  return S_OK;
}

int Writer::Write(float x, float y, float z, const char *text, int count, unsigned int mode) {
	if( count <= 0 )
		count = font_->GetTextLength(text);

	if( mode == acGraphics::FONT_ALIGN_CENTER ) 	{
		float w = font_->GetTextWidth(text, count);
		x -= w/2;
	}
	else if( mode == acGraphics::FONT_ALIGN_RIGHT ) 	{
		float w = font_->GetTextWidth(text, count);
		x -= w;
	}

	return InternalWrite(x, y, z, text, count,0);

}

int Writer::InternalWrite(float x, float y, float z, const char *text, int count, float spacing) {
	int page = -1;
	//render->Begin(RENDER_QUAD_LIST);
  
	//y += font_->scale * float(font_->base);
  //y -= font_->scale * float(font_->fontHeight);
  int vcount = 0;
	for( int n = 0; n < count; ) {
		int charId = font_->GetTextChar(text, n, &n);
		acGraphics::SCharDescr *ch = font_->GetChar(charId);
		if( ch == 0 ) ch = &font_->defChar;

		// Map the center of the texel to the corners
		// in order to get pixel perfect mapping
		float u = (float(ch->srcX)+0.5f) / font_->scaleW;
		float v = (float(ch->srcY)+0.5f) / font_->scaleH;
		float u2 = u + float(ch->srcW) / font_->scaleW;
		float v2 = v + float(ch->srcH) / font_->scaleH;

		float a = font_->scale * float(ch->xAdv);
		float w = font_->scale * float(ch->srcW);
		float h = font_->scale * float(ch->srcH);
		float ox = font_->scale * float(ch->xOff);
		float oy = font_->scale * float(ch->yOff);

		if( ch->page != page )
		{
			//render->End();
			page = ch->page;
			//render->GetGraphics()->SetTexture(pages[page]);
			//render->Begin(RENDER_QUAD_LIST);
		}
    
    vertex_array_[vcount].Color = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
    vertex_array_[vcount].Channel = ch->chnl;
    vertex_array_[vcount].Tex = XMFLOAT2(u,v);
    vertex_array_[vcount++].Pos = XMFLOAT2(x+ox, y+oy);
    vertex_array_[vcount].Color = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
    vertex_array_[vcount].Channel = ch->chnl;
    vertex_array_[vcount].Tex = XMFLOAT2(u2,v);
    vertex_array_[vcount++].Pos = XMFLOAT2(x+w+ox, y+oy);
    vertex_array_[vcount].Color = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
    vertex_array_[vcount].Channel = ch->chnl;
    vertex_array_[vcount].Tex = XMFLOAT2(u,v2);
    vertex_array_[vcount++].Pos = XMFLOAT2(x+ox, y+h+oy);
    vertex_array_[vcount].Color = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
    vertex_array_[vcount].Channel = ch->chnl;
    vertex_array_[vcount].Tex = XMFLOAT2(u2,v2);
    vertex_array_[vcount++].Pos = XMFLOAT2(x+w+ox, y+h+oy);

		x += a;
		if( charId == ' ' )
			x += spacing;

		if( n < count )
			x += font_->AdjustForKerningPairs(charId, font_->GetTextChar(text,n));
	}
  context_->CopyToVertexBuffer(vertex_buffer_,vertex_array_,sizeof(Vertex),0,vcount);
	//render->End();

  return S_OK;
}

int Writer::Draw(int count) {

  effect_->Begin();

  UINT stride = sizeof( Vertex );
  UINT offset = 0;
  context_->SetVertexBuffers(0,1,&vertex_buffer_,&stride,&offset);
  context_->ClearIndexBuffer();
  ((ContextD3D11*)context_)->device_context()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
  camera_.SetConstantBuffer(0);
  ((ContextD3D11*)context_)->device_context()->Draw(count*4,0);

  return S_OK;
}

}
}