/*
   AngelCode Tool Box Library
   Copyright (c) 2007-2008 Andreas Jonsson
  
   This software is provided 'as-is', without any express or implied 
   warranty. In no event will the authors be held liable for any 
   damages arising from the use of this software.

   Permission is granted to anyone to use this software for any 
   purpose, including commercial applications, and to alter it and 
   redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you 
      must not claim that you wrote the original software. If you use
      this software in a product, an acknowledgment in the product 
      documentation would be appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and 
      must not be misrepresented as being the original software.

   3. This notice may not be removed or altered from any source 
      distribution.
  
   Andreas Jonsson
   andreas@angelcode.com
*/
#include "../../ve.h"
#include "acutil_unicode.h"

namespace acGraphics {

Font::Font() {
	fontHeight = 0;
	base = 0;
	scaleW = 0;
	scaleH = 0;
	scale = 1.0f;
	hasOutline = false;
	encoding = NONE;
  pages = NULL;
}

Font::~Font() {
	std::map<int, SCharDescr*>::iterator it = chars.begin();
	while( it != chars.end() )
	{
		delete it->second;
		it++;
	}

  SafeRelease(&pages);
	//for( UINT n = 0; n < pages.size(); n++ )
		//if( pages[n] )
		//	pages[n]->Release();
}

int Font::Init(const char *fontFile, const char *fxFile)
{
	this->fxFile = fxFile;
  int r = 0;
	//// Load the effects file
	//CGraphics *gfx = render->GetGraphics();
	//int r = gfx->LoadEffect(fxFile);
	//if( r < 0 )
	//{
	//	LOG(("Failed to load '%s' (%d)", fxFile, r));
	//	return -1;
	//}

	// Load the font
	//FILE *f = fopen(fontFile, "rb");
	
	// Determine format by reading the first bytes of the file
	//char str[4] = {0};
	//fread(str, 3, 1, f);
	//fseek(f, 0, SEEK_SET);

	//FontLoader *loader = 0;
	//if( strcmp(str, "BMF") == 0 )
	//	loader = new FontLoaderBinaryFormat(f, fontFile);
	//else
	//	loader = new FontLoaderTextFormat(f, this, fontFile);

	//r = loader->Load();
	//delete loader;

	return r;
}

void Font::SetTextEncoding(EFontTextEncoding encoding) {
	this->encoding = encoding;
}

// Internal
//optimized by khalid
SCharDescr *Font::GetChar(int id) {
	//std::map<int, SCharDescr*>::iterator it = chars.find(id);
  
	//if( it == chars.end() ) return 0;

	return chars[id];//it->second;
}

// Internal
float Font::AdjustForKerningPairs(int first, int second) {	
	SCharDescr *ch = GetChar(first);
	if( ch == 0 ) return 0;
	for( UINT n = 0; n < ch->kerningPairs.size(); n += 2 ) {
		if( ch->kerningPairs[n] == second )
			return ch->kerningPairs[n+1] * scale;
	}

	return 0;
}

float Font::GetTextWidth(const char *text, int count) {
	if( count <= 0 )
		count = GetTextLength(text);

	float x = 0;

	for( int n = 0; n < count; ) {
		int charId = GetTextChar(text,n,&n);

		SCharDescr *ch = GetChar(charId);
		if( ch == 0 ) ch = &defChar;

		x += scale * (ch->xAdv);

		if( n < count )
			x += AdjustForKerningPairs(charId, GetTextChar(text,n));
	}

	return x;
}

void Font::SetHeight(float h) {
	scale = h / float(fontHeight);
}

float Font::GetHeight() {
	return scale * float(fontHeight);
}

float Font::GetBottomOffset() {
	return scale * (base - fontHeight);
}

float Font::GetTopOffset() {
	return scale * (base - 0);
}

// Internal
// Returns the number of bytes in the string until the null char
int Font::GetTextLength(const char *text) {
	if( encoding == UTF16 )	{
		int textLen = 0;
    for(;;)	{
			unsigned int len;
			int r = acUtility::DecodeUTF16(&text[textLen], &len);
			if( r > 0 )
				textLen += len;
			else if( r < 0 )
				textLen++;
			else
				return textLen;
		}
	}

	// Both UTF8 and standard ASCII strings can use strlen
	return (int)strlen(text);
}

// Internal
int Font::GetTextChar(const char *text, int pos, int *nextPos) {
	int ch;
	unsigned int len;
	if( encoding == UTF8 )	{
		ch = acUtility::DecodeUTF8(&text[pos], &len);
		if( ch == -1 ) len = 1;
	}
	else if( encoding == UTF16 )	{
		ch = acUtility::DecodeUTF16(&text[pos], &len);
		if( ch == -1 ) len = 2;
	}
	else	{
		len = 1;
		ch = (unsigned char)text[pos];
	}

	if( nextPos ) *nextPos = pos + len;
	return ch;
}

// Internal
int Font::FindTextChar(const char *text, int start, int length, int ch) {
	int pos = start;
	int nextPos;
	int currChar = -1;
  while( pos < length )	{
		currChar = GetTextChar(text, pos, &nextPos);
		if( currChar == ch )
			return pos;
		pos = nextPos;
	} 

	return -1;
}

void Font::InternalWrite(float x, float y, float z, const char *text, int count, float spacing) {
	int page = -1;
	//render->Begin(RENDER_QUAD_LIST);

	y += scale * float(base);

	for( int n = 0; n < count; ) {
		int charId = GetTextChar(text, n, &n);
		SCharDescr *ch = GetChar(charId);
		if( ch == 0 ) ch = &defChar;

		// Map the center of the texel to the corners
		// in order to get pixel perfect mapping
		float u = (float(ch->srcX)+0.5f) / scaleW;
		float v = (float(ch->srcY)+0.5f) / scaleH;
		float u2 = u + float(ch->srcW) / scaleW;
		float v2 = v + float(ch->srcH) / scaleH;

		float a = scale * float(ch->xAdv);
		float w = scale * float(ch->srcW);
		float h = scale * float(ch->srcH);
		float ox = scale * float(ch->xOff);
		float oy = scale * float(ch->yOff);

		if( ch->page != page )
		{
			//render->End();
			page = ch->page;
			//render->GetGraphics()->SetTexture(pages[page]);
			//render->Begin(RENDER_QUAD_LIST);
		}
    /*
		render->VtxColor(0xFFFFFFFF);
		render->VtxData(ch->chnl);
		render->VtxTexCoord(u, v);
		render->VtxPos(x+ox, y-oy, z);
		render->VtxTexCoord(u2, v);
		render->VtxPos(x+w+ox, y-oy, z);
		render->VtxTexCoord(u2, v2);
		render->VtxPos(x+w+ox, y-h-oy, z);
		render->VtxTexCoord(u, v2);
		render->VtxPos(x+ox, y-h-oy, z);
    */
		x += a;
		if( charId == ' ' )
			x += spacing;

		if( n < count )
			x += AdjustForKerningPairs(charId, GetTextChar(text,n));
	}

	//render->End();
}

void Font::Write(float x, float y, float z, const char *text, int count, unsigned int mode) {
	if( count <= 0 )
		count = GetTextLength(text);

	if( mode == FONT_ALIGN_CENTER ) 	{
		float w = GetTextWidth(text, count);
		x -= w/2;
	}
	else if( mode == FONT_ALIGN_RIGHT ) 	{
		float w = GetTextWidth(text, count);
		x -= w;
	}

	InternalWrite(x, y, z, text, count);
}

void Font::WriteML(float x, float y, float z, const char *text, int count, unsigned int mode) {
	if( count <= 0 )
		count = GetTextLength(text);

	// Get first line
	int pos = 0;
	int len = FindTextChar(text, pos, count, '\n'); 
	if( len == -1 ) len = count;
	while( pos < count ) {
		float cx = x;
		if( mode == FONT_ALIGN_CENTER )	{
			float w = GetTextWidth(&text[pos], len);
			cx -= w/2;
		}
		else if( mode == FONT_ALIGN_RIGHT )	{
			float w = GetTextWidth(&text[pos], len);
			cx -= w;
		}

		InternalWrite(cx, y, z, &text[pos], len);

		y -= scale * float(fontHeight);

		// Get next line
		pos += len;
		int ch = GetTextChar(text, pos, &pos);
		if( ch == '\n' ) {
			len = FindTextChar(text, pos, count, '\n');
			if( len == -1 )
				len = count - pos;
			else 
				len = len - pos;
		}
	}
}

void Font::WriteBox(float x, float y, float z, float width, const char *text, int count, unsigned int mode) {
	if( count <= 0 )
		count = GetTextLength(text);

	float currWidth = 0, wordWidth;
	int lineS = 0, lineE = 0, wordS = 0, wordE = 0;
	int wordCount = 0;

	const char *s = " ";
	float spaceWidth = GetTextWidth(s, 1);
	bool softBreak = false;
	
	for(; lineS < count;)	{
		// Determine the extent of the line
		for(;;) {
			// Determine the number of characters in the word
			while( wordE < count && 
				GetTextChar(text,wordE) != ' ' &&
				GetTextChar(text,wordE) != '\n' )
				// Advance the cursor to the next character
				GetTextChar(text,wordE,&wordE);

			// Determine the width of the word
			if( wordE > wordS ) {
				wordCount++;
				wordWidth = GetTextWidth(&text[wordS], wordE-wordS);
			}
			else
				wordWidth = 0;

			// Does the word fit on the line? The first word is always accepted.
			if( wordCount == 1 || currWidth + (wordCount > 1 ? spaceWidth : 0) + wordWidth <= width )	{
				// Increase the line extent to the end of the word
				lineE = wordE;
				currWidth += (wordCount > 1 ? spaceWidth : 0) + wordWidth;

				// Did we reach the end of the line?
				if( wordE == count || GetTextChar(text,wordE) == '\n' )	{
					softBreak = false;

					// Skip the newline character
					if( wordE < count )
            // Advance the cursor to the next character
						GetTextChar(text,wordE,&wordE);
					break;
				}				

				// Skip the trailing space
				if( wordE < count && GetTextChar(text,wordE) == ' ' )
					// Advance the cursor to the next character
					GetTextChar(text,wordE,&wordE);

				// Move to next word
				wordS = wordE;
			}
			else{
				softBreak = true;

				// Skip the trailing space
				if( wordE < count && GetTextChar(text,wordE) == ' ' )
					// Advance the cursor to the next character
					GetTextChar(text,wordE,&wordE);

				break;
			}
		}

		// Write the line
		if( mode == FONT_ALIGN_JUSTIFY )	{
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
			if( mode == FONT_ALIGN_RIGHT )
				cx = x + width - currWidth;
			else if( mode == FONT_ALIGN_CENTER )
				cx = x + 0.5f*(width - currWidth);

			InternalWrite(cx, y, z, &text[lineS], lineE - lineS);
		}

		if( softBreak )	{
			// Skip the trailing space
			if( lineE < count && GetTextChar(text,lineE) == ' ' )
				// Advance the cursor to the next character
				GetTextChar(text,lineE,&lineE);

			// We've already counted the first word on the next line
			currWidth = wordWidth;
			wordCount = 1;
		}	else {
			// Skip the line break
			if( lineE < count && GetTextChar(text,lineE) == '\n' )
				// Advance the cursor to the next character
				GetTextChar(text,lineE,&lineE);

			currWidth = 0;
			wordCount = 0;
		}
		
		// Move to next line
		lineS = lineE;
		wordS = wordE;
		y -= scale * float(fontHeight);
	}
}

void Font::PrepareEffect() {
	//CGraphics *gfx = render->GetGraphics();

	//gfx->SetEffect(fxFile.c_str());

	//if( hasOutline )
	//	gfx->SetEffectTechnique("RenderWithOutline");
	//else
	//	gfx->SetEffectTechnique("RenderWithoutOutline");
}

void Font::PreparePixelPerfectOutput()
{
	//IDirect3DDevice9 *dev = render->GetDevice();
	//CGraphics *gfx = render->GetGraphics();

	//// Determine size of view port
	//D3DVIEWPORT9 vp;
	//dev->GetViewport(&vp);

	//// Clear world matrix
	//D3Ddx::XMATRIX mtx;
	//D3Ddx::XMatrixIdentity(&mtx);
	//gfx->SetMatrix(D3DTS_WORLD, &mtx);

	//// Setup orthogonal view
	//// Origin is in lower-left corner
	//D3Ddx::XMatrixOrthoOffCenterLH(&mtx, 0, (float)vp.Width, 0, (float)vp.Height, vp.MinZ, vp.MaxZ);
	//gfx->SetMatrix(D3DTS_VIEW, &mtx);

	//D3Ddx::XMatrixIdentity(&mtx);
	//gfx->SetMatrix(D3DTS_PROJECTION, &mtx);

	//// Adjust the scale of the font so that the 
	//// resolution of texture is the same as the screen
	//scale = 1.0f;
}

}