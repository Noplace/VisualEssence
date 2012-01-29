#include "../ve.h"

namespace graphics {
namespace font {



Writer::Writer() : Shape(), vertex_array_(NULL) {
  memset(&vertex_buffer_,0,sizeof(vertex_buffer_));
}

Writer::~Writer() {
  
}

int Writer::Initialize(Context* context) {
  vertex_array_ = NULL;
  Component::Initialize(context);
  //camera_.Initialize(context);
  //camera_.Ortho2D();
  //camera_.UpdateConstantBuffer();
  memset(&misc_buffer_shader_,0,sizeof(misc_buffer_shader_));
  misc_buffer_shader_.ps_color = XMLoadColor(&XMCOLOR(0xffffffff));
  misc_buffer_shader_.world = XMMatrixTranspose(XMMatrixScaling(1,1,1));
  /*misc_buffer_.description.usage = D3D11_USAGE_DEFAULT;
  misc_buffer_.description.byte_width = sizeof(graphics::shader::ConstantBuffer2Type);
  misc_buffer_.description.bind_flags = D3D11_BIND_CONSTANT_BUFFER;
  misc_buffer_.description.cpu_access_flags = 0;
  HRESULT hr = context_->CreateBuffer(misc_buffer_,NULL);
  if ( hr != S_OK )
    return S_FALSE;

  hr = UpdateConstantBuffer();
  if ( hr != S_OK )
    return S_FALSE;*/

  return S_OK;
}

int Writer::Deinitialize() {
  //context_->DestroyBuffer(misc_buffer_);
  if (context_!=nullptr) {
    context_->DestroyBuffer(vertex_buffer_);
  }
  if (vertex_array_)
    delete [] vertex_array_;
  //camera_.Deinitialize();
  //effect_->Deinitialize();
  Component::Deinitialize();
  return S_OK;
}

int Writer::PrepareWrite(int count) {
  context_->DestroyBuffer(vertex_buffer_);
  vertex_buffer_.description.bind_flags = D3D11_BIND_VERTEX_BUFFER;
  vertex_buffer_.description.usage = D3D11_USAGE_DEFAULT;
  vertex_buffer_.description.byte_width = sizeof( graphics::shape::Vertex ) * count * 6;
  vertex_buffer_.description.cpu_access_flags = 0;
  context_->CreateBuffer(vertex_buffer_,NULL);
  if (vertex_array_ != NULL)
    delete [] vertex_array_;
  vertex_array_ = NULL;
  vertex_array_ = new graphics::shape::Vertex[count * 6];
  ZeroMemory(vertex_array_,sizeof(graphics::shape::Vertex)*count*6);
  vcount = 0;
  return S_OK;
}

int Writer::Write(const char *text, int count, unsigned int mode) {
  char_count = 0;
	if( count <= 0 )
		count = font_->GetTextLength(text);
  float x = 0;
	if( mode == acGraphics::FONT_ALIGN_CENTER ) 	{
		float w = font_->GetTextWidth(text, count);
		x -= w/2;
	}
	else if( mode == acGraphics::FONT_ALIGN_RIGHT ) 	{
		float w = font_->GetTextWidth(text, count);
		x -= w;
	}
  vcount = 0;
	return InternalWrite(x, 0, 0, text, count,0);

}


int Writer::WriteML(const char *text, int count, unsigned int mode) {
  char_count = 0;
  float x = 0;
  float y = 0;
	if( count <= 0 )
		count = font_->GetTextLength(text);
  vcount = 0;
	// Get first line
	int pos = 0;
	int len = font_->FindTextChar(text, pos, count, '\n'); 
	if( len == -1 ) len = count;
	while( pos < count ) {
		float cx = x;
		if( mode == acGraphics::FONT_ALIGN_CENTER )	{
			float w = font_->GetTextWidth(&text[pos], len);
			cx -= w/2;
		}
		else if( mode == acGraphics::FONT_ALIGN_RIGHT )	{
			float w = font_->GetTextWidth(&text[pos], len);
			cx -= w;
		}

		InternalWrite(cx, y, 0, &text[pos], len,0);

		y += font_->scale * float(font_->fontHeight);

		// Get next line
		pos += len;
		int ch = font_->GetTextChar(text, pos, &pos);
		if( ch == '\n' ) {
			len = font_->FindTextChar(text, pos, count, '\n');
			if( len == -1 )
				len = count - pos;
			else 
				len = len - pos;
		}
	}

  return S_OK;
}

int Writer::WriteBox(const char *text, int count, unsigned int mode, float width) {
  char_count = 0;
  float x = 0;
  float y = 0;
	if( count <= 0 )
		count = font_->GetTextLength(text);
  vcount = 0;
	float currWidth = 0, wordWidth;
	int lineS = 0, lineE = 0, wordS = 0, wordE = 0;
	int wordCount = 0;

	const char *s = " ";
	float spaceWidth = font_->GetTextWidth(s, 1);
	bool softBreak = false;
	
	for(; lineS < count;)	{
		// Determine the extent of the line
		for(;;) {
			// Determine the number of characters in the word
			while( wordE < count && 
				font_->GetTextChar(text,wordE) != ' ' &&
				font_->GetTextChar(text,wordE) != '\n' )
				// Advance the cursor to the next character
				font_->GetTextChar(text,wordE,&wordE);

			// Determine the width of the word
			if( wordE > wordS ) {
				wordCount++;
				wordWidth = font_->GetTextWidth(&text[wordS], wordE-wordS);
			}
			else
				wordWidth = 0;

			// Does the word fit on the line? The first word is always accepted.
			if( wordCount == 1 || currWidth + (wordCount > 1 ? spaceWidth : 0) + wordWidth <= width )	{
				// Increase the line extent to the end of the word
				lineE = wordE;
				currWidth += (wordCount > 1 ? spaceWidth : 0) + wordWidth;

				// Did we reach the end of the line?
				if( wordE == count || font_->GetTextChar(text,wordE) == '\n' )	{
					softBreak = false;

					// Skip the newline character
					if( wordE < count )
            // Advance the cursor to the next character
						font_->GetTextChar(text,wordE,&wordE);
					break;
				}				

				// Skip the trailing space
				if( wordE < count && font_->GetTextChar(text,wordE) == ' ' )
					// Advance the cursor to the next character
					font_->GetTextChar(text,wordE,&wordE);

				// Move to next word
				wordS = wordE;
			}
			else{
				softBreak = true;

				// Skip the trailing space
				if( wordE < count && font_->GetTextChar(text,wordE) == ' ' )
					// Advance the cursor to the next character
					font_->GetTextChar(text,wordE,&wordE);

				break;
			}
		}

		// Write the line
		if( mode == acGraphics::FONT_ALIGN_JUSTIFY )	{
			float spacing = 0;
			if( softBreak )	{
				if( wordCount > 2 )
					spacing = (width - currWidth) / (wordCount-2);
				else
					spacing = (width - currWidth);
			}
			
            InternalWrite(x, y, 0, &text[lineS], lineE - lineS, spacing);
		}	else	{
			float cx = x;
			if( mode == acGraphics::FONT_ALIGN_RIGHT )
				cx = x + width - currWidth;
			else if( mode == acGraphics::FONT_ALIGN_CENTER )
				cx = x + 0.5f*(width - currWidth);

			InternalWrite(cx, y, 0, &text[lineS], lineE - lineS,0);
		}

		if( softBreak )	{
			// Skip the trailing space
			if( lineE < count && font_->GetTextChar(text,lineE) == ' ' )
				// Advance the cursor to the next character
				font_->GetTextChar(text,lineE,&lineE);

			// We've already counted the first word on the next line
			currWidth = wordWidth;
			wordCount = 1;
		}	else {
			// Skip the line break
			if( lineE < count && font_->GetTextChar(text,lineE) == '\n' )
				// Advance the cursor to the next character
				font_->GetTextChar(text,lineE,&lineE);

			currWidth = 0;
			wordCount = 0;
		}
		
		// Move to next line
		lineS = lineE;
		wordS = wordE;
		y += font_->scale * float(font_->fontHeight);
	}

  return S_OK;
}

inline void SetVertex() {
}

int Writer::InternalWrite(float x, float y, float z, const char *text, int count, float spacing) {
  
	//y -= font_->scale * float(font_->base);
  //y += font_->scale * float(font_->fontHeight);
  XMFLOAT4 color = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
  char_count += count;
	for( int n = 0; n < count; ) {
		int charId = font_->GetTextChar(text, n, &n);
		acGraphics::SCharDescr *ch = font_->GetChar(charId);
		if( ch == 0 ) ch = &font_->defChar;

		// Map the center of the texel to the corners
		// in order to get pixel perfect mapping
    //+0.5f
    //NOT

		float u = (float(ch->srcX)) / font_->scaleW;
		float v = (float(ch->srcY)) / font_->scaleH;
		float u2 = u + float(ch->srcW) / font_->scaleW;
		float v2 = v + float(ch->srcH) / font_->scaleH;
    
		float a = font_->scale * float(ch->xAdv);
		float w = font_->scale * float(ch->srcW);
		float h = -font_->scale * float(ch->srcH);
		float ox = font_->scale * float(ch->xOff);
		float oy = -font_->scale * float(ch->yOff);

    float dy = -oy;
    vertex_array_[vcount++] = graphics::shape::Vertex(XMFLOAT3(x+ox, y-oy,z),XMFLOAT2(u,v),color,ch->page);
    vertex_array_[vcount++] = graphics::shape::Vertex(XMFLOAT3(x+w+ox, y-oy,z),XMFLOAT2(u2,v),color,ch->page);
    vertex_array_[vcount++] = graphics::shape::Vertex(XMFLOAT3(x+ox, y-oy-h,z),XMFLOAT2(u,v2),color,ch->page);

    vertex_array_[vcount++] = graphics::shape::Vertex(XMFLOAT3(x+ox, y-oy-h,z),XMFLOAT2(u,v2),color,ch->page);
    vertex_array_[vcount++] = graphics::shape::Vertex(XMFLOAT3(x+w+ox, y-oy,z),XMFLOAT2(u2,v),color,ch->page);
    vertex_array_[vcount++] = graphics::shape::Vertex(XMFLOAT3(x+w+ox, y-oy-h,z),XMFLOAT2(u2,v2),color,ch->page);

		x += a;
		if( charId == ' ' )
			x += spacing;

		if( n < count )
			x += font_->AdjustForKerningPairs(charId, font_->GetTextChar(text,n));
	}
  context_->CopyToVertexBuffer(vertex_buffer_,vertex_array_,sizeof(graphics::shape::Vertex),0,vcount);

  return S_OK;
}

int Writer::GetOutput(graphics::shape::Vertex* vertex_array,int* vertex_count,int* char_count) {
  *vertex_count = vcount;
  *char_count = this->char_count;
  vertex_array = this->vertex_array_;
  return S_OK;
}

int Writer::UpdateConstantBuffer() {
  return S_FALSE;//context_->UpdateSubresource(misc_buffer_,&misc_buffer_shader_,NULL,0,0);
}


int Writer::Construct() {

  return S_OK;
}

int Writer::BuildTransform() {
  graphics::shape::Shape::BuildTransform();
  misc_buffer_shader_.world = XMMatrixTranspose( world_ );
  return S_OK;
}

int Writer::Draw() {

  //effect_->Begin();

  UINT stride = sizeof( graphics::shape::Vertex );
  UINT offset = 0;
  context_->SetVertexBuffers(0,1,&vertex_buffer_,&stride,&offset);
  context_->ClearIndexBuffer();
  context_->SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  //camera_.SetConstantBuffer(0);

  //set contant buffer;
  //context_->SetConstantBuffers(kShaderTypeVertex,2,1,&misc_buffer_);
  //context_->SetConstantBuffers(kShaderTypePixel,2,1,&misc_buffer_);
  context_->SetShaderResources(kShaderTypePixel,1,1,(void**)&font_->pages);

  context_->Draw(char_count*6,0);
  return S_OK;
}

}
}