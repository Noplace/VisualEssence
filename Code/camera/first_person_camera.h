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

namespace ve {

class FirstPersonCamera : public ve::PrespectiveCamera {
 public:
  dx::XMVECTOR DefaultCamTarget = dx::XMVectorSet(0.0f,0.0f,-1.0f, 0.0f);
  dx::XMVECTOR DefaultForward = dx::XMVectorSet(0.0f,0.0f,-1.0f, 0.0f);
  dx::XMVECTOR DefaultRight = dx::XMVectorSet(1.0f,0.0f,0.0f, 0.0f);
  dx::XMVECTOR DefaultUp = dx::XMVectorSet(0.0f,1.0f,0.0f, 0.0f);
  dx::XMVECTOR camForward;
  dx::XMVECTOR camRight = dx::XMVectorSet(1.0f,0.0f,0.0f, 0.0f);
  dx::XMVECTOR camUp,camTarget,camPosition ;
  dx::XMMATRIX camRotationMatrix;
  dx::XMMATRIX groundWorld;

  float moveLeftRight = 0.0f;
  float moveBackForward = 0.0f;
  float moveBottomTop = 0.0f;
  float camYaw = 0.0f;
  float camPitch = 0.0f;

  FirstPersonCamera() : ve::PrespectiveCamera() {
    camUp =  dx::XMVectorSet(0.0f,1.0f,0.0f, 0.0f);
   
    view_ = dx::XMMatrixIdentity();
  }


  int Update(float dt);

};

}

