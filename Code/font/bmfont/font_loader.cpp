/*****************************************************************************************************************
* Copyright (c) 2012 Khalid Ali Al-Kooheji                                                                       *
*                                                                                                                *
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and              *
* associated documentation files (the "Software"), to deal in the Software without restriction, including        *
* without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        *
* copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the       *
* following conditions:                                                                                          *
*                                                                                                                *
* The above copyright notice and this permission notice shall be included in all copies or substantial           *
* portions of the Software.                                                                                      *
*                                                                                                                *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT          *
* LIMITED TO THE WARRANTIES OF MERCHANTABILITY, * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.          *
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, * DAMAGES OR OTHER LIABILITY,      *
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE            *
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                                         *
*****************************************************************************************************************/

#include "../../ve.h"
#include "../../util/WICTextureLoader.h"

using namespace std;

namespace acGraphics {

FontLoader::FontLoader(const wchar_t *fontFile) {
	this->f = NULL;
	this->fontFile = fontFile;
  font_ = NULL;
	outlineThickness = 0;
}

ID3D11Resource* FontLoader::LoadPage(int id, const wchar_t *pageFile, const wchar_t *fontFile) {
	wstring str;

	// Load the texture from the same directory as the font descriptor file

	// Find the directory
	str = fontFile;
	for( size_t n = 0; (n = str.find('/', n)) != string::npos; ) str.replace(n, 1, L"\\");
	size_t i = str.rfind('\\');
	if( i != string::npos )
		str = str.substr(0, i+1);
	else
		str = L"";

	// Load the font textures
	str += pageFile;



  ID3D11Resource* texture;
  ID3D11Device* d = ((ve::ContextD3D11*)context())->device();
  ID3D11DeviceContext* dc = ((ve::ContextD3D11*)context())->device_context();
  int hr = DirectX::CreateWICTextureFromFile(d,str.c_str(),&texture,NULL);
  if (hr == S_OK)
    return texture;

  return NULL;
	/*IDirect3DTexture9 *texture = 0;
	UINT mipLevels = 0; // 0 = all
	HRESULT hr = D3DXCreateTextureFromFileEx(font->render->GetDevice(), str.c_str(), 
		D3DX_DEFAULT, D3DX_DEFAULT, mipLevels, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
		D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &texture);
	if( FAILED(hr) )
	{
		LOG(("Failed to load font page '%s' (%X)", str.c_str(), hr));
	}*/

	//font_->pages[id] = 0;
}

void FontLoader::SetFontInfo(int outlineThickness) {
	this->outlineThickness = outlineThickness;
}

void FontLoader::SetCommonInfo(int fontHeight, int base, int scaleW, int scaleH, int pages, bool isPacked) {
	font_->fontHeight = fontHeight;
	font_->base = base;
	font_->scaleW = scaleW;
	font_->scaleH = scaleH;

	/*font_->pages.resize(pages);
	for( int n = 0; n < pages; n++ )
		font_->pages[n] = 0;*/
  font_->pages = NULL;

	if( isPacked && outlineThickness )
		font_->hasOutline = true;
}

void FontLoader::AddChar(int id, int x, int y, int w, int h, int xoffset, int yoffset, int xadvance, int page, int chnl) {
	// Convert to a 4 element vector
	// TODO: Does this depend on hardware? It probably does
	if     ( chnl == 1 ) chnl = 0x00010000;  // Blue channel
	else if( chnl == 2 ) chnl = 0x00000100;  // Green channel
	else if( chnl == 4 ) chnl = 0x00000001;  // Red channel
	else if( chnl == 8 ) chnl = 0x01000000;  // Alpha channel
	else chnl = 0;

	if( id >= 0 )	{
		SCharDescr *ch = new SCharDescr;
		ch->srcX = x;
		ch->srcY = y;
		ch->srcW = w;
		ch->srcH = h;
		ch->xOff = xoffset;
		ch->yOff = yoffset;
		ch->xAdv = xadvance;
		ch->page = page;
		ch->chnl = chnl;

		font_->chars.insert(std::map<int, SCharDescr*>::value_type(id, ch));
	}

	if( id == -1 )	{
		font_->defChar.srcX = x;
		font_->defChar.srcY = y;
		font_->defChar.srcW = w;
		font_->defChar.srcH = h;
		font_->defChar.xOff = xoffset;
		font_->defChar.yOff = yoffset;
		font_->defChar.xAdv = xadvance;
		font_->defChar.page = page;
		font_->defChar.chnl = chnl;
	}
}

void FontLoader::AddKerningPair(int first, int second, int amount) {
	if( first >= 0 && first < 256 && font_->chars[first] )	{
		font_->chars[first]->kerningPairs.push_back(second);
		font_->chars[first]->kerningPairs.push_back(amount);
	}
}




FontLoaderBinaryFormat::FontLoaderBinaryFormat(const wchar_t *fontFile) : FontLoader(fontFile) {
}

int FontLoaderBinaryFormat::Load() {

  if (font_ == NULL)
    return S_FALSE;

  _wfopen_s(&f,fontFile, L"rb");

	// Read and validate the tag. It should be 66, 77, 70, 2, 
	// or 'BMF' and 2 where the number is the file version.
	char magicString[4];
	fread(magicString, 4, 1, f);
	if( strncmp(magicString, "BMF\003", 4) != 0 )
	{
		//LOG(("Unrecognized format for '%s'", fontFile));
		fclose(f);
		return -1;
	}

	// Read each block
	char blockType;
	int blockSize;
	while( fread(&blockType, 1, 1, f) )	{
		// Read the blockSize
		fread(&blockSize, 4, 1, f);

		switch( blockType )	{
		case 1: // info
			ReadInfoBlock(blockSize);
			break;
		case 2: // common
			ReadCommonBlock(blockSize);
			break;
		case 3: // pages
			ReadPagesBlock(blockSize);
			break;
		case 4: // chars
			ReadCharsBlock(blockSize);
			break;
		case 5: // kerning pairs
			ReadKerningPairsBlock(blockSize);
			break;
		default:
			//LOG(("Unexpected block type (%d)", blockType));
			fclose(f);
			return -1;
		}
	}

	fclose(f);

	// Success
	return 0;
}

void FontLoaderBinaryFormat::ReadInfoBlock(int size) {
#pragma pack(push)
#pragma pack(1)
struct infoBlock {
    WORD fontSize;
    BYTE reserved:4;
    BYTE bold    :1;
    BYTE italic  :1;
    BYTE unicode :1;
    BYTE smooth  :1;
    BYTE charSet;
    WORD stretchH;
    BYTE aa;
    BYTE paddingUp;
    BYTE paddingRight;
    BYTE paddingDown;
    BYTE paddingLeft;
    BYTE spacingHoriz;
    BYTE spacingVert;
    BYTE outline;         // Added with version 2
    char fontName[1];
};
#pragma pack(pop)

	char *buffer = new char[size];
	fread(buffer, size, 1, f);

	// We're only interested in the outline thickness
	infoBlock *blk = (infoBlock*)buffer;
	SetFontInfo(blk->outline);

	delete[] buffer;
}

void FontLoaderBinaryFormat::ReadCommonBlock(int size) {
#pragma pack(push)
#pragma pack(1)
struct commonBlock {
    WORD lineHeight;
    WORD base;
    WORD scaleW;
    WORD scaleH;
    WORD pages;
    BYTE packed  :1;
    BYTE reserved:7;
	BYTE alphaChnl;
	BYTE redChnl;
	BYTE greenChnl;
	BYTE blueChnl;
}; 
#pragma pack(pop)

	char *buffer = new char[size];
	fread(buffer, size, 1, f);

	commonBlock *blk = (commonBlock*)buffer;

	SetCommonInfo(blk->lineHeight, blk->base, blk->scaleW, blk->scaleH, blk->pages, blk->packed ? true : false);

	delete[] buffer;
}

void FontLoaderBinaryFormat::ReadPagesBlock(int size) {
#pragma pack(push)
#pragma pack(1)
struct pagesBlock {
    char pageNames[1];
};
#pragma pack(pop)

	char *buffer = new char[size];
	fread(buffer, size, 1, f);

	pagesBlock *blk = (pagesBlock*)buffer;
  int texture_count = 0;
  {
    char* ptr = buffer;
    for (int i =0;i<size;++i) {
      if (ptr[i] == '\0')
        ++texture_count;
    }
  }
  D3D11_TEXTURE2D_DESC tex_desc;
  tex_desc,
  tex_desc.Width = 256;
  tex_desc.Height = 256;
  tex_desc.MipLevels = 1;
  tex_desc.ArraySize = texture_count;
  tex_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  tex_desc.SampleDesc.Count = 1;
  tex_desc.SampleDesc.Quality = 0;
  tex_desc.Usage = D3D11_USAGE_DEFAULT;
  tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  tex_desc.CPUAccessFlags = 0;
  tex_desc.MiscFlags = 0;

  ID3D11Texture2D* temp_texture;
  ID3D11Device* d = ((ve::ContextD3D11*)context())->device();
  ID3D11DeviceContext* dc = ((ve::ContextD3D11*)context())->device_context();
  d->CreateTexture2D(&tex_desc,NULL,&temp_texture);


  ID3D11Resource** page_textures = new ID3D11Resource*[size];
	for( int id = 0, pos = 0; pos < size; id++ ) {
    std::string pagename_str(&blk->pageNames[pos]);
    std::wstring pagename_wstr(pagename_str.begin(), pagename_str.end());
		page_textures[id] = LoadPage(id, pagename_wstr.c_str(), fontFile);
    dc->CopySubresourceRegion(temp_texture,id,0,0,0,page_textures[id],0,NULL);
		pos += 1 + (int)strlen(&blk->pageNames[pos]);
	}

	delete[] buffer;

  D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
  view_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
  view_desc.Texture2DArray.MostDetailedMip = 0;
  view_desc.Texture2DArray.MipLevels = tex_desc.MipLevels;
  view_desc.Texture2DArray.FirstArraySlice = 0;
  view_desc.Texture2DArray.ArraySize = tex_desc.ArraySize;
  d->CreateShaderResourceView(temp_texture,&view_desc,&font_->pages);

  for (int i=0;i<texture_count;++i) {
    SafeRelease(&page_textures[i]);
  }
  SafeRelease(&temp_texture);
}

void FontLoaderBinaryFormat::ReadCharsBlock(int size) {
#pragma pack(push)
#pragma pack(1)
struct charsBlock {
    struct charInfo {
        DWORD id;
        WORD  x;
        WORD  y;
        WORD  width;
        WORD  height;
        short xoffset;
        short yoffset;
        short xadvance;
        BYTE  page;
        BYTE  chnl;
    } chars[1];
};
#pragma pack(pop)

	char *buffer = new char[size];
	fread(buffer, size, 1, f);
	charsBlock *blk = (charsBlock*)buffer;

	for( int n = 0; int(n*sizeof(charsBlock::charInfo)) < size; n++ )	{
		AddChar(blk->chars[n].id,
		        blk->chars[n].x,
				blk->chars[n].y,
				blk->chars[n].width,
				blk->chars[n].height,
				blk->chars[n].xoffset,
				blk->chars[n].yoffset,
				blk->chars[n].xadvance,
				blk->chars[n].page,
				blk->chars[n].chnl);
	}

	delete[] buffer;
}

void FontLoaderBinaryFormat::ReadKerningPairsBlock(int size)
{
#pragma pack(push)
#pragma pack(1)
struct kerningPairsBlock {
    struct kerningPair  {
        DWORD first;
        DWORD second;
        short amount;
    } kerningPairs[1];
};
#pragma pack(pop)

	char *buffer = new char[size];
	fread(buffer, size, 1, f);

	kerningPairsBlock *blk = (kerningPairsBlock*)buffer;

	for( int n = 0; int(n*sizeof(kerningPairsBlock::kerningPair)) < size; n++ )	{
		AddKerningPair(blk->kerningPairs[n].first,
		               blk->kerningPairs[n].second,
					   blk->kerningPairs[n].amount);
	}

	delete[] buffer;
}

} // end namespace



