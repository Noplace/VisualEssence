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
  vcount = 0;
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
  vcount = 0;
	return InternalWrite(x, y, z, text, count,0);

}


int Writer::WriteML(float x, float y, float z, const char *text, int count, unsigned int mode) {
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

		InternalWrite(cx, y, z, &text[pos], len,0);

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

int Writer::WriteBox(float x, float y, float z, float width, const char *text, int count, unsigned int mode) {
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
			
            InternalWrite(x, y, z, &text[lineS], lineE - lineS, spacing);
		}	else	{
			float cx = x;
			if( mode == acGraphics::FONT_ALIGN_RIGHT )
				cx = x + width - currWidth;
			else if( mode == acGraphics::FONT_ALIGN_CENTER )
				cx = x + 0.5f*(width - currWidth);

			InternalWrite(cx, y, z, &text[lineS], lineE - lineS,0);
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


int Writer::InternalWrite(float x, float y, float z, const char *text, int count, float spacing) {
	int page = -1;
	//render->Begin(RENDER_QUAD_LIST);
  
	//y += font_->scale * float(font_->base);
  //y -= font_->scale * float(font_->fontHeight);
  
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
    vertex_array_[vcount].Page = ch->page;
    vertex_array_[vcount].Color = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
    vertex_array_[vcount].Channel = ch->chnl;
    vertex_array_[vcount].Tex = XMFLOAT2(u,v);
    vertex_array_[vcount++].Pos = XMFLOAT2(x+ox, y+oy);
    vertex_array_[vcount].Page = ch->page;
    vertex_array_[vcount].Color = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
    vertex_array_[vcount].Channel = ch->chnl;
    vertex_array_[vcount].Tex = XMFLOAT2(u2,v);
    vertex_array_[vcount++].Pos = XMFLOAT2(x+w+ox, y+oy);
    vertex_array_[vcount].Page = ch->page;
    vertex_array_[vcount].Color = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
    vertex_array_[vcount].Channel = ch->chnl;
    vertex_array_[vcount].Tex = XMFLOAT2(u,v2);
    vertex_array_[vcount++].Pos = XMFLOAT2(x+ox, y+h+oy);
    vertex_array_[vcount].Page = ch->page;
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
  context_->SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
  camera_.SetConstantBuffer(0);

  //((graphics::ContextD3D11*)context_)->device_context()->PSSetShaderResources(0,2,&font_->pages[0]);
  //((graphics::ContextD3D11*)context_)->device_context()->PSSetShaderResources(1,1,&font_->pages[1]);
  context_->SetPixelShaderResources(0,2,(void**)&font_->pages[0]);

  context_->Draw(count*4,0);
  return S_OK;
}

}
}