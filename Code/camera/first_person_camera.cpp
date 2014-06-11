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
#include "../ve.h"

namespace ve {

int FirstPersonCamera::Update(float dt) {
     
  //testing code
  {
    float speed = 2.0f * dt;
      if (GetAsyncKeyState('W'))
        moveBackForward += speed;
      if (GetAsyncKeyState('S'))
        moveBackForward -= speed;

      if (GetAsyncKeyState('A'))
        moveLeftRight -= speed;
      if (GetAsyncKeyState('D'))
        moveLeftRight += speed;

      if (GetAsyncKeyState(VK_UP))
        camPitch += speed;
      if (GetAsyncKeyState(VK_DOWN))
        camPitch -= speed;

      if (GetAsyncKeyState(VK_LEFT))
        camYaw += speed;
      if (GetAsyncKeyState(VK_RIGHT))
        camYaw -= speed;

    if (GetAsyncKeyState(VK_SPACE))
        moveBottomTop += speed;

      /*GetCursorPos(&mouse_current);
      if ((mouse_current.x != mouse_last.x) || (mouse_current.y != mouse_last.y)) {
        camera_.camYaw  += -(mouse_current.x- mouse_last.x)   * speed * 0.01f;
        camera_.camPitch += (mouse_current.y- mouse_last.y)  * speed * 0.01f;
        mouse_last = mouse_current;
      }*/

  
      //if (GetAsyncKeyState('T'))
      //  DefaultCamTarget = dx::XMVectorAdd(DefaultCamTarget,dx::XMVectorSet(0,speed,0,0));
     // if (GetAsyncKeyState('G'))
      //  DefaultCamTarget = dx::XMVectorAdd(DefaultCamTarget,dx::XMVectorSet(0,-speed,0,0));
  }

  camRotationMatrix = dx::XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget = dx::XMVector3TransformCoord(DefaultForward, camRotationMatrix );
	camTarget = dx::XMVector3Normalize(camTarget);

	/*dx::XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = dx::XMMatrixRotationY(camPitch);
  dx::XMMATRIX RotateXTempMatrix;
	RotateYTempMatrix = dx::XMMatrixRotationY(camPitch);*/

	camRight = dx::XMVector3TransformCoord(DefaultRight, camRotationMatrix);
	camUp = dx::XMVector3TransformCoord(DefaultUp, camRotationMatrix);
	camForward = dx::XMVector3TransformCoord(DefaultForward, camRotationMatrix);

  //camPosition = dx::XMVectorAdd(camPosition,dx::XMVectorMultiply(dx::XMVectorSet(moveBottomTop,moveBottomTop,moveBottomTop,moveBottomTop),camUp));
  camPosition = dx::XMVectorAdd(camPosition,dx::XMVectorSet(0,moveBottomTop,0,0));
	camPosition = dx::XMVectorAdd(camPosition,dx::XMVectorMultiply(dx::XMVectorSet(moveLeftRight,moveLeftRight,moveLeftRight,moveLeftRight),camRight));
	camPosition = dx::XMVectorAdd(camPosition,dx::XMVectorMultiply(dx::XMVectorSet(moveBackForward,moveBackForward,moveBackForward,moveBackForward),camForward));

  moveLeftRight = 0.0f;
  moveBackForward = 0.0f;
  moveBottomTop  = 0.0f;
	camTarget = dx::XMVectorAdd(camPosition,camTarget);	

	  
  BuildViewMatrix(camPosition,camTarget,camUp);
  return S_OK;
}


}