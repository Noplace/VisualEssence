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

#ifndef FONT_H
#define FONT_H

namespace ve {
namespace font {
class Writer;
}
}

namespace acGraphics {

class FontLoader;

struct SCharDescr {
	SCharDescr() : srcX(0), srcY(0), srcW(0), srcH(0), xOff(0), yOff(0), xAdv(0), page(0) {}

	short srcX;
	short srcY;
	short srcW;
	short srcH;
	short xOff;
	short yOff;
	short xAdv;
	short page;
	unsigned int chnl;

	std::vector<int> kerningPairs;
};

enum EFontTextEncoding {
	NONE,
	UTF8,
	UTF16
};

class Font {
public:
	Font();
	~Font();

	int Init(const char *fontFile, const char *fxFile);

	void SetTextEncoding(EFontTextEncoding encoding);

	float GetTextWidth(const char *text, int count);
	void Write(float x, float y, float z, const char *text, int count, unsigned int mode);
	void WriteML(float x, float y, float z, const char *text, int count, unsigned int mode);
	void WriteBox(float x, float y, float z, float width, const char *text, int count, unsigned mode);

	void SetHeight(float h);
	float GetHeight();

	float GetBottomOffset();
	float GetTopOffset();

	void PrepareEffect();
	void PreparePixelPerfectOutput();
  //std::vector<ID3D11ShaderResourceView*> pages;
  ID3D11ShaderResourceView* pages;
protected:
	friend class FontLoader;
  friend class ve::font::Writer;

	void InternalWrite(float x, float y, float z, const char *text, int count, float spacing = 0);

	float AdjustForKerningPairs(int first, int second);
	SCharDescr *GetChar(int id);

	int GetTextLength(const char *text);
	int GetTextChar(const char *text, int pos, int *nextPos = 0);
	int FindTextChar(const char *text, int start, int length, int ch);

	short fontHeight; // total height of the font
	short base;       // y of base line
	short scaleW;
	short scaleH;
	SCharDescr defChar;
	bool hasOutline;

	float scale;
	EFontTextEncoding encoding;


	std::map<int, SCharDescr*> chars;
	
	std::string fxFile;
};

const int FONT_ALIGN_LEFT    = 0;
const int FONT_ALIGN_CENTER  = 1;
const int FONT_ALIGN_RIGHT   = 2;
const int FONT_ALIGN_JUSTIFY = 3;

}

// 2008-05-11 Storing the characters in a map instead of an array
// 2008-05-17 Added support for writing text with UTF8 and UTF16 encoding

#endif