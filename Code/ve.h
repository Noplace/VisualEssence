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
#pragma once

#pragma warning( push )             //Suppress MSVC warning spam
#pragma warning( disable : 4275 )

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

//forwards
namespace ve {
class Camera;
class Context;
class ActionManager;
}


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinCore/types.h>
#include <WinCore/math/math.h>
#include <eh.h>
#include <ppltasks.h>
#include <future>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
namespace dx = DirectX; 
namespace dxp = DirectX::PackedVector;
#include "util/timer.h"
#include "util/asyncdataread.h"
#include "util/easing.h"
#include "component.h"
#include "render_object.h"
#include "drawable.h"
#include "action/action_manager.h"
#include "input_layout.h"
#include "texture.h"
#include "resource_view.h"
#include "buffer/buffer.h"
#include "shader/shader.h"
#include "shape/shape.h"
#include "context/context.h"
#include "context/contextd3d11.h"
#include "buffer/vertex_buffer.h"
#include "camera/camera.h"
#include "camera/camera2d.h"
#include "shape/rectangle.h"
#include "shape/arc.h"
#include "sprite/sprite.h"
#include "effect/effect.h"
#include "shader/shader_manager.h"
#include "shader/shader2d_helper.h"
#include "scene/scene.h"
#include "font/font_sprite.h"
#include "font/bmfont/font.h"
#include "font/bmfont/font_loader.h"
#include "font/writer.h"

#pragma warning( pop )

