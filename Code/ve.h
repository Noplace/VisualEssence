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
#ifndef VE_H
#define VE_H

#pragma warning( push )             //Suppress MSVC warning spam
#pragma warning( disable : 4275 )

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

//forwards
namespace graphics {
class Camera;
}

#include <WinCore/windows/windows.h>
#include <WinCore/math/math.h>
#include <vector>
//#include <string>
#include <map>
#ifdef VE_USE_D3D11_ENGINE
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#endif
#ifdef VE_USE_D3D9_ENGINE
#include <d3d9.h>
#include <d3dx9.h>
#endif
#ifdef VE_USE_OPENGL_ENGINE
#endif
#include <xnamath.h>
#include "component.h"
#include "drawable.h"
#include "input_layout.h"
#include "texture.h"
#include "resource_view.h"
#include "buffer/buffer.h"
#include "shader/shader.h"
#include "shape/shape.h"
#include "context/context.h"
#include "context/contextd3d11.h"
#include "context/contextd3d9.h"
#include "camera/camera.h"
#include "camera/camera2d.h"
#include "shape/rectangle.h"
#include "shape/arc.h"
#include "sprite/sprite.h"
#include "effect/effect.h"
#include "shader/shader2d_helper.h"
#include "scene/scene.h"
#include "font/font_sprite.h"
#include "font/bmfont/font.h"
#include "font/bmfont/font_loader.h"
#include "font/writer.h"

#pragma warning( pop )

#endif